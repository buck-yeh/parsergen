#include "BNFContext.h"
//--------------------------------------------------------------------
#include "bux/FsUtil.h"     // bux::search_dirs()
#include "bux/LogStream.h"  // HRTN()
#include "bux/StrUtil.h"    // bux::expand_env()
#include "bux/XException.h" // RUNTIME_ERROR()
#include <cstring>          // memcmp()
#include <fstream>          // std::ifstream
#include <fmt/core.h>       // fmt::print(), fmt::format()
#include <limits>           // std::numeric_limits<>

namespace {

enum
{
    BNM_KEYWORD     =1<<0,
    BNM_OPERATOR    =1<<1
};

} // namespace

namespace ParserGen {

using namespace Main;

//
//      Implement Classes
//
C_BNFContext::~C_BNFContext()
{
    for (auto &i: m_PriorRaws)
        delete i.m_pAttr;
}

bool C_BNFContext::activateBuiltinNonterminal(const std::string &id)
{
    bool ret =false;
    if (id == "keyword")
    {
        m_BuilinNonterminalMask |=BNM_KEYWORD;
        ret =true;
    }
    else if (id == "operator")
    {
        m_BuilinNonterminalMask |=BNM_OPERATOR;
        ret =true;
    }
    return ret;
}

bool C_BNFContext::addGenId(const std::string &id)
{
    if (m_GeneTokens.find(id) == m_GeneTokens.end())
    {
        m_GeneTokens.insert(id);
        return true;
    }
    return false;
}

bool C_BNFContext::addLexId(const std::string &id)
{
    if (m_Lex2ID.find(id) == m_Lex2ID.end())
    {
        m_Lex2ID[id];
        return true;
    }
    return false;
}

void C_BNFContext::addIdFromToken(I_LexAttr *attr)
{
    if (!attr)
        return;

    std::string id;
    bool generated = false;
    if (auto term =dynamic_cast<I_ProductionTerm*>(attr))
    {
        if (term->generateId(id))
        {
            generated = true;
            addLiteral(term, id);
        }
    }
    else if (auto i =dynamic_cast<C_IntegerLex*>(attr))
        generated = getGeneratedID(i->str(),id);

    if (generated)
        addGenId(id);
}

void C_BNFContext::addLiteral(I_ProductionTerm *term, const std::string &id)
{
    if (auto s =dynamic_cast<C_StrLiteral*>(term))
        m_Literal2IdName[s->m_Str] = id;
}

void C_BNFContext::addOption(const std::string &key, C_Semantic *value)
{
    auto &dst = m_OptMap[key];
    if (value)
    {
        if (!dst.empty() && key == "EXTRA_TOKENS")
        {
            auto t = new C_BracketedLex("|");
            dst.emplace_back(t);
        }
        dst.splice(dst.end(), *value);
    }
}

bool C_BNFContext::addProduction(C_Production &prod, C_Semantic *semantic)
{
    for (auto &i: m_Productions)
        if (i == prod)
            return false;

    m_Productions.emplace_back();
    auto &key = m_Productions.back();
    key.m_Lval.swap(prod.m_Lval);
    key.m_Rval.swap(prod.m_Rval);
    key.m_Index = m_OntoJumps.size();
    m_OntoPool.emplace_back();
    auto &value = m_OntoPool.back();
    value.m_Productions.emplace_back(&key);
    m_OntoJumps.emplace_back(&value);
    if (semantic)
        value.semantic(*semantic);
    else
        m_Semanticless.emplace_back(&key);

    return true;
}

void C_BNFContext::addPriority(E_Associativity assoc, bux::LR1::C_LexPtr &src)
{
    C_PriorityRaw p;
    p.m_Weight = m_PriorWeight;
    p.m_Assoc = assoc;
    p.m_pAttr = src.disown();
    m_PriorRaws.emplace_back(p);
}

bool C_BNFContext::attr2id(I_LexAttr *attr, T_LexID &id, std::string &errMsg)
{
    if (auto const term = dynamic_cast<I_ProductionTerm*>(attr))
    {
        if (prodTerm2id(term, id, errMsg))
            return true;
        if (!errMsg.empty())
            return false;
    }
    if (auto const ilex = dynamic_cast<C_IntegerLex*>(attr))
    {
        std::string idstr;
        if (getGeneratedID(ilex->str(), idstr))
        {
            auto found = m_GenLex2Id.find(idstr);
            if (found != m_GenLex2Id.end() && m_IdSet)
            {
                id = found->second;
                return true;
            }
            errMsg.assign("User-defined id ").append(idstr).append(" not generated");
            return false;
        }
        else
        {
            id = ilex->str()[0];
            return true;
        }
    }

    errMsg.assign("Unknown lex attr of type ").append(HRTN(*attr));
    return false;
}

bool C_BNFContext::checkSemanticlessProductions() const
{
    if (!m_Semanticless.empty())
    {
        fmt::print("WARNING: The following productions are declared with no semantic!\n");
        for (auto i: m_Semanticless)
             fmt::print("\t{}\n", i->str());

        return true;
    }
    return false;
}

bool C_BNFContext::checkUnusedOptions() const
{
    bool found = false;
    for (auto &i: m_OptMap)
        if (!i.second.m_Used)
        {
            fmt::print("WARNING: Option %{} defined but not used !\n", i.first);
            found = true;
        }

    return found;
}

bool C_BNFContext::equal(const C_Semantic &a, const C_Semantic &b) const
{
    return ensureNoConcat(a) == ensureNoConcat(b);
    /*
    if (a.size() != b.size())
        return false;

    for (auto ia = a.begin(), ib = b.begin(); ia != a.end(); ++ia, ++ib)
    {
        if (auto sa =dynamic_cast<C_BracketedLex*>(*ia))
        {
            if (auto sb =dynamic_cast<C_BracketedLex*>(*ib))
            {
                if (sa->m_Str != sb->m_Str)
                    return false;
            }
            else
                return false;
        }
        else
            RUNTIME_ERROR("Unknown semantic lex type {}", HRTN(*ia));
    }
    return true;*/
}

std::string C_BNFContext::expand_include(const std::string &org_path) const
{
    return bux::search_dirs(bux::expand_env(org_path.c_str()), m_IncDirs).string();
}

void C_BNFContext::issueError(bux::E_LogLevel level, const C_SourcePos &pos, const std::string &message)
{
    static const char *const KIND[] ={
        "FATAL",
        "ERROR",
        "WARNING",
        "INFO",
        "VERBOSE"
    };
    const auto kind = KIND[level];
    fmt::print("{}({},{}) {}: {}\n", pos.m_Source, pos.m_Line, pos.m_Col, kind, message);
    static const size_t ERR_LIMIT[] ={
        0,
        10,
        100,
        std::numeric_limits<size_t>::max(),
        std::numeric_limits<size_t>::max()
    };
    if (++m_ErrorTotal[level] > ERR_LIMIT[level])
        RUNTIME_ERROR("Too many ({}) {}s!", m_ErrorTotal[level] ,kind);
}

void C_BNFContext::normalize(const C_Semantic &src, C_Semantic &dst) const
{
    std::string remains;
    normalize(src, dst, remains);
    if (!remains.empty())
    {
        auto *const lex = new C_BracketedLex(remains);
        dst.emplace_back(lex);
    }
}

void C_BNFContext::normalize(
    const C_Semantic        &src,
    C_Semantic              &dst,
    std::string             &remains    ) const
{
    for (auto &i: src)
    {
        if (!i)
            ;
        else if (auto s =dynamic_cast<C_BracketedLex*>(i))
            remains += s->m_Str;
        else if (auto o =dynamic_cast<C_OptionLex*>(i))
        {
            auto found = m_OptMap.find(o->m_Var);
            if (found == m_OptMap.end())
                RUNTIME_ERROR("Option \"{}\" not found", o->m_Var);

            found->second.m_Used = true;
            normalize(found->second, dst, remains);
        }
        else if (auto l =dynamic_cast<C_LexSymbol*>(i))
        {
            if (m_Lex2ID.find(l->m_Var) == m_Lex2ID.end())
                RUNTIME_ERROR("Lex \"{}\" not found", l->m_Var);

            remains.append("TID_LEX_").append(l->m_Var);
        }
        else
            RUNTIME_ERROR("Unknown semantic lex type {}", HRTN(i));
    }
}

bool C_BNFContext::setClassName(
    C_StringList            &qualified,
#ifdef __TEMPLATE_OUTPUT
    C_TemplateArgs          &targs,
#else
    C_TemplateArgs          &,
#endif // __TEMPLATE_OUTPUT
    std::string             &errMsg )
{
    // Preconditions
    if (!m_ClassName.empty())
    {
        errMsg ="Class name already defined";
        return false;
    }
#ifdef __TEMPLATE_OUTPUT
    if (!m_TemplateArgs.empty())
    {
        errMsg ="Template args already defined";
        return false;
    }
#endif // __TEMPLATE_OUTPUT
    if (qualified.empty())
    {
        errMsg ="Empty class name assigned";
        return false;
    }

    // Do it
    m_ClassName =qualified.back();
    qualified.pop_back();
    m_Namespace.swap(qualified);
#ifdef __TEMPLATE_OUTPUT
    for (auto j =targs.begin(); j != targs.end(); ++j)
    {
        m_TemplateArgs.emplace_back();
        auto &t =m_TemplateArgs.back();
        t.m_ArgDummy =j->back();
        j->pop_back();
        std::string &s =t.m_TypeName;
        for (auto k: *j)
        {
            if (!s.empty() && isalnum(s[s.size()-1]) && isalnum(k.front()))
                s += ' ';

            s += k;
        }
        if (!s.empty() && isalnum(s[s.size()-1]))
            s += ' ';
    }
#endif // __TEMPLATE_OUTPUT
    return true;
}

size_t C_BNFContext::tempReductionIndex(const C_IndexedProd &prod) const
{
    return m_OntoJumps[prod.m_Index]->m_Index;
}

void C_BNFContext::wrapup(const bux::C_SourcePos &pos)
{
    C_ParserInfo::wrapup(); // MUST be called first

    // Condition stack not emptied
    for (auto i = m_CondStack.rbegin(); i != m_CondStack.rend(); ++i)
        issueError(LL_ERROR, i->m_pos, "No matching #endif");

    // Ensure every new token is added to m_GeneTokens
    for (auto &i: m_Productions)
    {
        std::string id;
        if (getNonterminalID(i.m_Lval, id))
            addGenId(id);

        for (auto &j: i.m_Rval)
            if (j->generateId(id))
            {
                addGenId(id);
                addLiteral(j, id); // terminal only
            }
    }

    for (auto &i: m_PriorRaws)
        addIdFromToken(i.m_pAttr);

    if (auto found = getOption("IDDEF_SOURCE"))
        // Parse the source
    {
        // Extract & assign ids for m_Lex2ID & m_GenLex2Id
        std::ifstream in(found->expand());
        unsigned long tid_max = 0;
        for (std::string s; getline(in, s);)
        {
            const auto posName = s.find_first_not_of(" \t");
            if (posName == std::string::npos ||
                memcmp(s.data()+posName, "TID_", 4))
                continue;

            static const char PRENUM[] = "= bux::TOKENGEN_LB";
            const auto posULong = s.find(PRENUM, posName);
            if (posULong != std::string::npos)
                // Found!
            {
                const auto t = strtoul(s.data()+posULong+sizeof PRENUM, nullptr, 10);
                if (tid_max + 1 == t)
                    tid_max = t;
                else if (!tid_max && !t)
                    ; // First TID_xxxx
                else
                    RUNTIME_ERROR("tid_max={} but next id is {}", tid_max, t);

                const auto posEndName = s.find_first_of(" \t", posName);
                if (posName == std::string::npos)
                    RUNTIME_ERROR("End of name not found \"{}\"", s.substr(posName));
                if (!memcmp(s.data()+posName, "TID_LEX_", 8))
                {
                    const auto off = posName + 8;
                    m_Lex2ID[s.substr(off, posEndName-off)] = T_LexID(bux::TOKENGEN_LB + t);
                }
                else
                    m_GenLex2Id[s.substr(posName, posEndName-posName)] = T_LexID(bux::TOKENGEN_LB + t);
            }
            else
            {
                s = s.substr(posName);
                if (s != "TID_UB_")
                    RUNTIME_ERROR("Not expected final {}", s);
            }
        }
        fmt::print("MAX(IdDef) = {}\n", tid_max);

        // Build m_Nonterm2Id from m_GeneTokens
        tid_max += bux::TOKENGEN_LB;
        for (auto &i: m_GeneTokens)
            if (memcmp(i.data(), "TID_", 4))
                m_Nonterm2Id[i] = T_LexID(++tid_max);
    }
    else
        // Assign from scratch
    {
        // Assign id part of m_Lex2ID
        bux::T_LexID count = bux::TOKENGEN_LB;
        for (auto &i: m_Lex2ID)
            i.second = count++;

        // Build m_GenLexId & m_Nonterm2Id from m_GeneTokens
        C_StringSet nonterms;
        for (auto &i: m_GeneTokens)
        {
            if (!memcmp(i.data(), "TID_", 4))
                m_GenLex2Id[i] = count++;
            else
                nonterms.insert(i);
        }
        for (auto &i: nonterms)
            m_Nonterm2Id[i] = count++;
    }
    m_IdSet =true;

    // Build m_PriorityMap from m_PriorRaws and m_Lex2ID
    size_t count =0;
    for (auto i = m_PriorRaws.begin(); i != m_PriorRaws.end(); ++i, ++count)
    {
        T_LexID id;
        std::string errMsg;
        if (attr2id(i->m_pAttr, id, errMsg))
        {
            auto found = m_PriorityMap.find(id);
            if (found == m_PriorityMap.end())
            {
                auto &dst = m_PriorityMap[id];
                dst.m_Weight = i->m_Weight;
                dst.m_Assoc = i->m_Assoc;
            }
            else
            {
                auto out = fmt::format("Duplicate priority assignments on id {}", id);
                if (id < 127)
                    out += fmt::format("(\'{}\')", asciiLiteral(char(id)));
                issueError(LL_ERROR, pos, out);
            }
        }
        else
        {
            issueError(LL_ERROR, pos, errMsg);
            issueError(LL_WARNING, pos,
                fmt::format("Fail to get id from the {}th priority record with token type {}", count, HRTN(*i->m_pAttr)));
        }
    }

    // Preconditions for building m_Productions and m_Reductions
    if (m_Productions.size() != m_OntoPool.size() ||
        m_Productions.size() != m_OntoJumps.size())
    {
        issueError(LL_FATAL, pos, "Inconsistent data for production optimization!");
        return;
    }

    // Add the root production if not found
    size_t rootCount{};
    for (auto &i: m_Productions)
        if (i.m_Lval == "@")
            ++rootCount;

    if (!rootCount)
        // Supply missing <@> production for user
    {
        bux::C_NewNode<C_ProductionLex> const t;
        t->m_Lval = "@";
        C_Semantic s;
        if (!m_Productions.empty())
        {
            const auto pseudoRoot = m_Productions.front().m_Lval;
            t->m_Rval.emplace_back(new C_Nonterminal{pseudoRoot});
            for (auto &i: m_OntoPool)
            {
                for (auto j: i.m_Productions)
                    if (j->m_Lval == pseudoRoot)
                    {
                        if (i.m_Reduction.expand().find("$r") != std::string::npos)
                        {
                            s.emplace_back(new C_BracketedLex{"$r = $1;"});
                            goto AddProd;
                        }
                        break;
                    }
            }
        }
    AddProd:
        addProduction(*t, &s);
    }
    else if (rootCount > 1)
    {
        issueError(LL_FATAL, pos, "<@> productions defined more than once!");
        return;
    }

    // Add productions of activated builtin nonterminals
    if (m_BuilinNonterminalMask)
        for (auto &i: m_Literal2IdName)
        {
            bux::C_NewNode<C_ProductionLex> const t;

            if ((m_BuilinNonterminalMask &BNM_KEYWORD) &&
                !i.second.compare(0, 12, "TID_KEYWORD_"))
            {
                t->m_Lval ="@keyword";
                t->m_Rval.emplace_back(new C_Id(i.first));
            }
            else if ((m_BuilinNonterminalMask &BNM_OPERATOR) &&
                !i.second.compare(0, 12, "TID_LITERAL_"))
            {
                t->m_Lval ="@operator";
                t->m_Rval.emplace_back(new C_StrLiteral(i.first));
            }

            C_Semantic s;
            s.emplace_back(new C_BracketedLex(fmt::format("$r = createLex<std::string>(\"{}\");",asciiLiteral(i.first))));
            addProduction(*t, &s);
        } // for (auto &i: m_Literal2IdName)

    // Build m_Productions and m_Reductions from
    // m_OntoPool, m_OntoJumps, m_Lex2ID and m_PriorityMap
    //
    for (auto &i: m_OntoPool)
    {
        C_Semantic t;
        normalize(i.m_Reduction, t);
        t.swap(i.m_Reduction);
    }
    bool changed;
    do
    {
        changed =false;
        count =0;
        C_OntoPool::iterator i;
        for (i =m_OntoPool.begin(); i != m_OntoPool.end(); (i++)->m_Index =count++);
        for (i =m_OntoPool.begin(); i != m_OntoPool.end();)
        {
            for (auto j =m_OntoPool.begin(); j != i; ++j)
                if (equal(i->m_Reduction,j->m_Reduction))
                    // Merge them
                {
                    changed =true;
                    for (auto &k: i->m_Productions)
                    {
                        j->m_Productions.emplace_back(k);
                        m_OntoJumps[k->m_Index] = &*j;
                    }
                    i = m_OntoPool.erase(i);
                    goto iNext;
                }
            ++i;
        iNext:;
        }
    } while (changed);
    for (auto &i: m_Productions)
        i.m_Index = m_OntoJumps[i.m_Index]->m_Index;
    for (auto &i: m_OntoPool)
         m_Reductions.emplace_back(i.m_Reduction.expand());

    // Normalize options
    for (auto &i: m_OptMap)
    {
        C_Semantic t;
        normalize(i.second, t);
        t.swap(i.second);
    }
}

void C_BNFContext::ifCond(bool y, const C_SourcePos &pos)
{
    m_CondStack.emplace_back(y, pos);
}

void C_BNFContext::elseCond(const C_SourcePos &pos)
{
    if (m_CondStack.empty() ||
        pos.m_Source != m_CondStack.back().m_pos.m_Source)
        issueError(LL_ERROR, pos, "No matching #if");
    else
    {
        auto &dst = m_CondStack.back().m_yes;
        dst = !dst;
    }
}

void C_BNFContext::endifCond(const C_SourcePos &pos)
{
    if (m_CondStack.empty() ||
        pos.m_Source != m_CondStack.back().m_pos.m_Source)
        issueError(LL_ERROR, pos, "No matching #if");
    else
        m_CondStack.pop_back();
}

bool C_BNFContext::testCond() const
{
    for (auto &i: m_CondStack)
        if (!i.m_yes)
            return false;

    return true;
}

} // namespace ParserGen
