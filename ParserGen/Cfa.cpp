/*
 *      CFA stands for Context-Free Automata, I think ...
 */
#include "Cfa.h"
//-------------------------------------------------------
#include "bux/StrUtil.h"    // HRTN()
#include "bux/XException.h" // RUNTIME_ERROR()
#include <fmt/core.h>       // fmt::print()

namespace {

//
//      In-Module Types
//
typedef C_LexSetT<I_ProductionTerm> C_Alphabet;

class FC_CreateClosure
{
public:

    // Nonvirtuals
    FC_CreateClosure(const C_ParserInfo &parsed);
    void operator()(C_StateItems &dst) const;
    operator bool() const { return m_Ready; }

private:

    // Types
    typedef C_LexSeq::const_iterator C_LexIter;
    typedef std::pair<C_LexIter,const C_LexIter> C_FirstsDep;
    typedef std::list<C_FirstsDep> C_FirstsDeps;
    typedef std::list<C_StringList> C_NullableDeps;

    struct C_FunctionalAttrs
    {
        // Data
        C_FirstsDeps        m_FirstsDeps;
        C_NullableDeps      m_NullableDeps;
        C_Lookahead         m_Firsts;
        bool                m_Nullable;   // nullable <=> e-productive

        // Nonvirtuals
        C_FunctionalAttrs(const C_ParserInfo &parsed): m_Firsts(parsed), m_Nullable(false)
            {}
    };
    typedef std::map<std::string,C_FunctionalAttrs> C_NtAttrs;

    // Data
    const C_ParserInfo      &m_Parsed;
    C_NtAttrs               m_AttrMap;
    bool                    m_Ready;

    // Nonvirtuals
    const C_FunctionalAttrs &getAttr(const std::string &) const;
};

struct FC_LessState
{
    bool operator()(const C_State *a, const C_State *b) const
        { return a->m_rstep2la < b->m_rstep2la; }
};

//
//      Implement In-Module Classes
//
FC_CreateClosure::FC_CreateClosure(const C_ParserInfo &parsed):
    m_Parsed(parsed),
    m_Ready(true)
{
    // Create dependencies
    for (auto &i: parsed.productions())
    {
        auto &attrs = m_AttrMap.try_emplace(i.m_Lval, parsed).first->second;
        if (!i.m_Rval.empty())
        {
            C_FirstsDep range(i.m_Rval.begin(), i.m_Rval.end());
            attrs.m_FirstsDeps.emplace_back(range);
            if (!attrs.m_Nullable)
            {
                C_StringList names;
                do
                {
                    if (auto nt = dynamic_cast<C_Nonterminal*>(*range.first))
                        names.emplace_back(nt->m_id);
                    else
                        // Terminal found! Impossible to be m_Nullable
                        goto postAddNames;
                } while (++range.first != range.second);

                attrs.m_NullableDeps.emplace_back(names);
            postAddNames:;
            }
        }
        else
        {
            attrs.m_Nullable = true;
            attrs.m_NullableDeps.clear();
        }
    }

    // Deduce nullabilities ( aka eps(), e-productivity )
    bool changed;
    do
    {
        changed = false;
        for (auto &i: m_AttrMap)
        {
            auto &deps = i.second.m_NullableDeps;
            for (auto j = deps.begin(); j != deps.end();)
            {
                for (auto k = j->begin(); k != j->end();)
                {
                    const auto &attrs = getAttr(*k);
                    if (attrs.m_NullableDeps.empty())
                        // Concluded nullity
                    {
                        changed =true;
                        if (attrs.m_Nullable)
                        {
                            j->erase(k++);
                            if (j->empty())
                                // Case closed
                            {
                                i.second.m_Nullable = true;
                                deps.clear();
                                goto next_i;
                            }
                        }
                        else
                        {
                            deps.erase(j++);
                            if (deps.empty())
                                // Case closed
                                goto next_i;

                            goto post_inc_j;
                        }
                    }
                    else
                        ++k;
                } // for (auto k =j->begin(); k != j->end();)

                ++j;
            post_inc_j:;
            } // for (auto j =deps.begin(); j != deps.end();)

#ifdef __BuckDebug
            if (!deps.empty() && i.second.m_Nullable)
                RUNTIME_ERROR("!deps.empty() && i->second.m_Nullable");
#endif
        next_i:;
        } // for (auto &i: m_AttrMap)
    } while (changed);

    // Deduce null-free first terminals ( aka e-ffi(), e-free first function )
    bool complete;
    do
    {
        changed =false;
        complete =true;
        for (auto &i: m_AttrMap)
        {
            auto &deps = i.second.m_FirstsDeps;
            for (auto j = deps.begin(); j != deps.end();)
            {
                bool spareDep = false;
                for (; j->first != j->second; ++j->first)
                {
                    auto const lex = *j->first;
                    if (auto nt = dynamic_cast<C_Nonterminal*>(lex))
                    {
                        const auto name = nt->m_id;
                        const auto &attrs = getAttr(name);
                        if (i.first == name)
                        {
                            if (!attrs.m_Nullable)
                            {
                                spareDep =true;
                                break;
                            }
                        }
                        else if (attrs.m_FirstsDeps.empty())
                            // Case of lex is closed -- Merge nonterminal's lookahead set
                        {
                            changed = true;
                            i.second.m_Firsts.insert(attrs.m_Firsts.begin(), attrs.m_Firsts.end());
                            if (!attrs.m_Nullable)
                            {
                                spareDep = true;
                                break;
                            }
                        }
                        else
                            break;
                    }
                    else if (auto const t = dynamic_cast<I_Terminal*>(lex))
                        // Production case closed
                    {
                        i.second.m_Firsts.insert(t);
                        spareDep = true;
                        break;
                    }
                    else
                        RUNTIME_ERROR("Unknown type {}", HRTN(lex));
                } // for (; j->first != j->second; ++j->first)

                if (spareDep || j->first == j->second)
                {
                    changed = true;
                    j = deps.erase(j);
                }
                else
                    ++j;
            } // for (auto j = deps.begin(); j != deps.end();)

            if (!deps.empty())
                complete = false;
        } // for (auto &i: m_AttrMap)
    } while (changed);

    if (!complete)
    {
        fmt::print("Can't determine lookahead sets of the following:\n");
        for (auto &i: m_AttrMap)
            if (!i.second.m_FirstsDeps.empty())
            {
                fmt::print("<{}>:\n", i.first);
                for (auto &j: i.second.m_FirstsDeps)
                {
                    bool first = true;
                    for (auto k = j.first; k != j.second; ++k, first = false)
                        fmt::print("{}{}", first? '\t': ' ', (**k).displayStr());

                    fmt::print("\n");
                }
            }
        m_Ready =false;
    }
}

void FC_CreateClosure::operator()(C_StateItems &dst) const
{
    bool changed;
    do
    {
        changed =false;
        for (auto &i: dst)
        {
            const auto item = i.first;
            auto &seq = item.first->m_Rval;
            if (item.second >= seq.size())
                // Complete item
                continue;

            if (auto const nt = dynamic_cast<const C_Nonterminal*>(seq[item.second]))
            {
                const std::string id = nt->m_id;
                for (auto &j: m_Parsed.productions())
                    if (j.m_Lval == id)
                    {
                        const C_ReductionStep key(&j,0);
                        auto &la = dst.try_emplace(key,m_Parsed).first->second;
                        if (la.empty())
                            // New item
                            changed = true;

                        const auto n =la.size();
                        for (auto k = seq.begin()+std::ptrdiff_t(item.second); ++k != seq.end();)
                            if (auto const nt = dynamic_cast<C_Nonterminal*>(*k))
                            {
                                auto &attrs =getAttr(nt->m_id);
                                la.insert(attrs.m_Firsts.begin(), attrs.m_Firsts.end());
                                if (!attrs.m_Nullable)
                                    goto afterMergeLookahead;
                            }
                            else if (auto const t = dynamic_cast<I_Terminal*>(*k))
                            {
                                la.insert(t);
                                goto afterMergeLookahead;
                            }
                            else
                                RUNTIME_ERROR("Unknown type {}", HRTN(*k));

                        // Thru nullable prefix
                        la.insert(i.second.begin(), i.second.end());
                        afterMergeLookahead:

                        if (la.size() > n)
                            // New lookaheads
                            changed = true;
                    } // if (j.m_Lval == id)
            } // if (auto const nt = dynamic_cast<const C_Nonterminal*>(seq[item.second]))
        } // for (auto &i: dst)
    } while (changed);
}

const FC_CreateClosure::C_FunctionalAttrs &
    FC_CreateClosure::getAttr(const std::string &key) const
{
    const auto pa = m_AttrMap.find(key);
    if (pa == m_AttrMap.end())
        // Not found
        RUNTIME_ERROR("Key '{}' not found", key);

    return pa->second;
}

} // namespace

//
//      External Functions
//
size_t makeCfa              (
    const C_ParserInfo      &parsed,
    C_States                &states,
    C_GotoMap               &stateMap )
{
    // Categorize tokens within production rules
    size_t                  ret =0;
    const C_IndexedProd     *initProd =0;
    C_Alphabet              variables(parsed);
    C_Alphabet              nonterminals(parsed);
    C_Alphabet              alphabet(parsed);
    for (auto &i: parsed.productions())
    {
        for (auto &j: i.m_Rval)
            if (dynamic_cast<C_Nonterminal*>(j))
                nonterminals.insert(j);
            else if (dynamic_cast<C_LexSymbol*>(j))
                variables.insert(j);
            else
                alphabet.insert(j);
        if (i.m_Lval == "@")
        {
            if (initProd)
            {
                fmt::print("Root production rule expression <@> ::= ... can only be defined once\n");
                ++ret;
            }
            initProd = &i;
        }
    }
    if (!initProd)
    {
        fmt::print("Can't find start production\n");
        return ++ret;
    }

    // Ensure existency of production rules for each nonterminal
    for (auto &i: nonterminals)
    {
        const auto name = dynamic_cast<const C_Nonterminal&>(*i).m_id;
        bool ok =false;
        for (auto &j: parsed.productions())
            if (j.m_Lval == name)
            {
                ok =true;
                break;
            }
        if (!ok)
        {
            fmt::print("Can't find any production for <{}>\n", name);
            return ++ret;
        }
    }
    for (auto &i: variables)
    {
        const std::string name = dynamic_cast<const C_LexSymbol&>(*i).m_Var;
        if (!parsed.hasLexSymbol(name))
        {
            fmt::print("Can't find macro resolution for ${}\n", name);
            return ++ret;
        }
    }
    alphabet.insert(nonterminals.begin(), nonterminals.end());
    alphabet.insert(variables.begin(), variables.end());

    FC_CreateClosure createClosure(parsed);
    if (!createClosure)
        return ++ret;

    fmt::print("Total {} lex-symbols {} nonterms {} literals\n", variables.size(), nonterminals.size(), alphabet.size());
    size_t stateId{};
    states.clear();
    stateMap.clear();
    C_State &state0 = states.emplace_back();
    state0.m_id = stateId++;
    const C_ReductionStep key0(initProd, 0);
    state0.m_rstep2la.try_emplace(key0, parsed).first->second.insert(0);
    createClosure(state0.m_rstep2la);

    typedef std::set<const C_State*, FC_LessState> C_StateLookup;
    C_StateLookup stateLookup;
    stateLookup.insert(&state0);
    for (const auto &i: states)
    {
        for (auto j: alphabet)
        {
            C_State t;
            for (auto &k: i.m_rstep2la)
            {
                const auto item = k.first;
                const auto &rval = item.first->m_Rval;
                if (item.second < rval.size() && *j == *rval[item.second])
                    t.m_rstep2la.emplace(C_ReductionStep(item.first,item.second+1), k.second);
            }
            if (!t.m_rstep2la.empty())
            {
                createClosure(t.m_rstep2la);
                auto k = stateLookup.find(&t);
                if (stateLookup.end() == k)
                    // New state
                {
                    t.m_id = stateId++;
                    k = stateLookup.insert(&states.emplace_back(std::move(t))).first;
                }
                if (!stateMap.try_emplace({&i, j}, *k).second)
                    fmt::print(" Err #{}: Duplicate state mapping\n", ++ret);

                fmt::print("\rstates = {}\tshifts = {}\x1b[K", states.size(), stateMap.size());
            }
        } // for (auto &j: alphabet)
    } // for (auto &i: states)
    return ret;
}

//
//      Class Implementations
//
FC_LessGotoKey::FC_LessGotoKey(const C_ParserInfo &parsed): m_LessTerm(parsed)
{
}

bool FC_LessGotoKey::operator()(const C_GotoKey &a, const C_GotoKey &b) const
{
    return  a.first->m_id < b.first->m_id || (
            a.first->m_id == b.first->m_id && m_LessTerm(a.second,b.second));
}

C_GotoMap::C_GotoMap(const C_ParserInfo &parsed):
    std::map<C_GotoKey,const C_State*,FC_LessGotoKey>(parsed)
{
}
