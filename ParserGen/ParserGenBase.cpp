#include "ParserGenBase.h"
#include "ParserIdDef.h"    // namespace Main::LexId
//---------------------------------------------------------------------------
#include "bux/LogStream.h"  // HRTN()
#include "bux/XException.h" // RUNTIME_ERROR()
#include <ctype.h>          // isascii(), isalnum()

namespace ParserGen {

using namespace Main;

//
//      Functions
//
std::function<bool(const std::string &str, std::string &dst)> getGeneratedID;

bool getNonterminalID(const std::string &str, std::string &dst)
{
    if (str == "@")
        return false;

    if (str.at(0) == '@')
        dst ="NIDa_"+str.substr(1);
    else
        dst ="NID_"+str;
    return true;
}

bool isKeyword(const std::string &str)
{
    for (auto c: str)
        if (!isascii(c) || c != '_' && !isalnum(c))
            return false;

    return true;
}

std::string literalSuffix(const std::string &s)
{
    std::string ret;
    for (auto i =s.begin(); i != s.end(); ++i)
    {
        unsigned char c =*i;
        if (isascii(c) && isalnum(c))
        {
            ret +=c;
            ret +='_';
        }
        else
            bux::addAsHex(ret, c);
    }
    return ret;
}

std::string ensureNoConcat(std::string s)
{
    if (s.empty())
        // Empty is empty
        return s;

    // Count lines
    size_t lines =0;
    for (size_t i =0; (i =s.find('\n',i)) != std::string::npos; ++lines)
    {
        if (++i == s.size())
            return s;
    }

    // Strip trailing spaces
    const auto stripPos =s.find_last_not_of(" \t\r\n");
    if (stripPos != std::string::npos)
        s.erase(stripPos+1);

    // Special treatment to one-liner
    if (!lines)
        s ="    " +s;

    return s += '\n';
}

std::string ensureNoConcat(const C_Semantic &src)
{
    return ensureNoConcat(src.expand());
}

//
//      Class Implementations
//
I_SemanticChunk::~I_SemanticChunk()
{
}

C_Semantic::~C_Semantic()
{
    for (auto i =begin(); i != end(); ++i)
        delete *i;
}

std::string C_Semantic::expand() const
{
    std::string ret;
    for (auto i: *this)
        if (auto blex = dynamic_cast<C_BracketedLex*>(i))
        {
            for (auto j: blex->m_Str)
                if (j != '\r')
                    ret += j;
        }
        else
            RUNTIME_ERROR("Unknown semantic lex type ", HRTN(i));

    // Post process
    if (!ret.empty() && ret.front() == '\n')
        ret.erase(0, 1);

    return ret;
}

C_Production::~C_Production()
{
    for (auto i =m_Rval.begin(); i != m_Rval.end(); ++i)
        delete *i;
}

bool C_Production::operator==(const C_Production &a) const
{
    if (m_Lval == a.m_Lval && m_Rval.size() == a.m_Rval.size())
    {
        for (auto i =m_Rval.begin(), j =a.m_Rval.begin(); i != m_Rval.end(); ++i, ++j)
            if (**i != **j)
                return false;

        return true;
    }
    return false;
}

std::string C_Production::inputName(size_t pos, const std::string &mark) const
{
    auto ret ="<"+m_Lval+"> ::=";
    size_t ipos =0;
    for (auto i: m_Rval)
    {
        if (ipos++ == pos)
            ret += mark;

        ret += ' ';
        ret += i->displayStr();
    }
    if (ipos <= pos)
        ret +=mark;
    return ret;
}

std::string C_ParserInfo::contextType() const
{
    std::string s;
    if (auto i = getOption("CONTEXT"))
        s = i->expand();

    return s;
}

void C_ParserInfo::enterNamespaces(std::ostream &out) const
{
    if (!m_Namespace.empty())
    {
        out <<'\n';
        for (auto &i: m_Namespace)
            out <<"namespace " <<i <<" {\n";
    }
}

std::string C_ParserInfo::fullNamespace() const
{
    std::string t;
    bool first =true;
    for (auto i =m_Namespace.begin(); i != m_Namespace.end(); ++i)
    {
        if (first)
            first =false;
        else
            t.append("::");
        t.append(*i);
    }
    return t;
}

const C_Semantic *C_ParserInfo::getOption(const std::string &name) const
{
    const C_Semantic *ret = 0;
    auto i = m_OptMap.find(name);
    if (i != m_OptMap.end())
    {
        i->second.m_Used = true;
        ret = &i->second;
    }
    return ret;
}

const std::string *C_ParserInfo::getReduction(size_t index) const
{
    if (index >= m_Reductions.size())
        return 0;

    return &m_Reductions[index];
}

std::string C_ParserInfo::getReduction(const C_IndexedProd &prod) const
{
    if (prod.m_Index >= m_Reductions.size())
        RUNTIME_ERROR("Production index {} > production count {}", prod.m_Index, m_Reductions.size());

    return m_Reductions[prod.m_Index];
}

bool C_ParserInfo::hasLexSymbol(const std::string &id) const
{
    return m_Lex2ID.find(id) != m_Lex2ID.end();
}

void C_ParserInfo::leaveNamespaces(std::ostream &out) const
{
    if (!m_Namespace.empty())
    {
        out <<'\n';
        for (auto &i: m_Namespace)
            out <<"} // namespace " <<i <<"\n";
    }
}

std::string C_ParserInfo::outputId(const I_ProductionTerm *term) const
{
    std::string idstr;
    if (!term)
        idstr ="bux::TID_EOF";
    else if (term->generateId(idstr))
        ;
    else if (auto const lex =dynamic_cast<const C_LexSymbol*>(term))
    {
        if (m_Lex2ID.find(lex->m_Var) == m_Lex2ID.end())
            RUNTIME_ERROR("Lex '{}' not found", lex->m_Var);

        idstr.assign("TID_LEX_").append(lex->m_Var);
    }
    else if (auto const slex =dynamic_cast<const C_StrLiteral*>(term))
        return std::string(1,'\'').append(asciiLiteral(slex->m_Str[0])).append(1,'\'');
    else
        RUNTIME_ERROR("{}Fail to produce output id for {}", idstr, HRTN(*term));

    return std::string("ZIP_TOKEN(").append(idstr).append(1,')');
}

T_LexID C_ParserInfo::prodTerm2id(const I_ProductionTerm *attr) const
{
    T_LexID id;
    std::string errMsg;
    if (!prodTerm2id(attr, id, errMsg))
        RUNTIME_ERROR("{}", errMsg);

    return id;
}

bool C_ParserInfo::prodTerm2id(const I_ProductionTerm *term, T_LexID &id, std::string &errMsg) const
{
    if (!term)
    {
        id =TID_EOF;
        return true;
    }

    std::string idstr;
    if (term->generateId(idstr))
    {
        if (m_IdSet)
        {
            auto found = m_GenLex2Id.find(idstr);
            if (found != m_GenLex2Id.end())
            {
                id = found->second;
                return true;
            }
            found = m_Nonterm2Id.find(idstr);
            if (found != m_Nonterm2Id.end())
            {
                id =found->second;
                return true;
            }
        }
        errMsg
            .assign("User-defined id ")
            .append(idstr)
            .append(" not generated");
        return false;
    }
    if (auto const lex = dynamic_cast<const C_LexSymbol*>(term))
    {
        if (m_IdSet)
        {
            auto found = m_Lex2ID.find(lex->m_Var);
            if (found != m_Lex2ID.end())
            {
                id = found->second;
                return true;
            }
        }
        errMsg
            .assign("Number id of $")
            .append(lex->m_Var)
            .append(" not found");
        return false;
    }
    if (auto const slex = dynamic_cast<const C_StrLiteral*>(term))
    {
        id = slex->m_Str[0];
        return true;
    }
    return false;
}

void C_ParserInfo::wrapup()
{
    if (getGeneratedID)
        LOGIC_ERROR("getGeneratedID already assigned");

    getGeneratedID = [ignoreCases = ignoreKeywordCase()](const std::string &str, std::string &dst) {
        if (str.size() == 1)
            return false;

        if (!isKeyword(str))
            dst = "TID_LITERAL_" + literalSuffix(str);
        else
        {
            dst = "TID_KEYWORD_";
            if (!ignoreCases)
                dst += str;
            else
                for (auto c: str)
                    dst += char(std::toupper(c));
        }
        return true;
    };
}

C_StrLiteral::C_StrLiteral(std::string_view s, char quoteMark):
    m_Str(s),
    m_QuoteMark(quoteMark)
{
}

C_StrLiteral::C_StrLiteral(size_t n, char c, char quoteMark):
    m_Str(n, c),
    m_QuoteMark(quoteMark)
{
}

bool C_StrLiteral::operator==(const I_ProductionTerm &a) const
{
    if (auto const t =dynamic_cast<const C_StrLiteral*>(&a))
        return m_Str == t->m_Str;
    return false;
}

std::string C_StrLiteral::displayStr() const
{
    std::string ret(asciiLiteral(m_Str));
    if (ret.size() != m_Str.size() || ret == "[[")
        ret.insert(0,"\"") += '\"';

    return ret;
}

bool C_StrLiteral::generateId(std::string &id) const
{
    return  getGeneratedID(m_Str, id);
}

bool C_LexSymbol::operator==(const I_ProductionTerm &a) const
{
    if (auto const t =dynamic_cast<const C_LexSymbol*>(&a))
        return m_Var == t->m_Var;
    return false;
}

std::string C_LexSymbol::displayStr() const
{
    return "$" +m_Var;
}

bool C_LexSymbol::generateId(std::string &) const
{
    return false;
}

bool C_Nonterminal::operator==(const I_ProductionTerm &a) const
{
    if (auto const t =dynamic_cast<const C_Nonterminal*>(&a))
        return m_id == t->m_id;
    return false;
}

std::string C_Nonterminal::displayStr() const
{
    return "<"+m_id+">";
}

bool C_Nonterminal::generateId(std::string &id) const
{
    return getNonterminalID(m_id, id);
}

bool FC_LessLex::operator()(const I_ProductionTerm *a, const I_ProductionTerm *b) const
{
    return id(a) < id(b);
}

T_LexID FC_LessLex::id(const I_ProductionTerm *term) const
{
    T_LexID ret;
    std::string reason;
    if (!m_Parsed.prodTerm2id(term, ret, reason))
    {
        std::string msg = "No id for " + term->displayStr();
        if (!reason.empty())
            msg.append("\t(").append(reason) += ')';

        RUNTIME_ERROR("{}", msg);
    }
    return ret;
}

} // namespace ParserGen
