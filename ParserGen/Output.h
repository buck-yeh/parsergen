#pragma once

#include "ActionMap.h"      // C_ActionMap
#include "ParserGenBase.h"  // C_ParserInfo
//---------------------------------------------------------------------------
#include "bux/XException.h" // RUNTIME_ERROR()
#include <optional>         // std::optional<>

//
//      Major.Minor.Release
//
constinit const int VERSION_MAJOR   = 1;
constinit const int VERSION_MINOR   = 7;
constinit const int VERSION_RELEASE = 5;

enum
{
    //
    //      Commmand-Line Flags
    //
    ALWAYS_OVERWRITE        = 1<<0,
    WITH_BOM                = 1<<1,
};

namespace bux { // reusability to be assessed

template<class T>
class C_PrimeBelts
{
public:

    // Types
    typedef std::vector<T> C_Belt;

    // Nonvirtuals
    C_PrimeBelts(): m_wrappedUp(false) {}
    void add(const C_Belt &belt);
    bool empty() const { return m_primes.empty(); }
    std::optional<std::pair<size_t,size_t>> find(const C_Belt &belt);
    void visit(std::function<void(const C_Belt &belt, size_t ind)> apply);

private:

    // Types
    typedef std::map<C_Belt,size_t> C_Belt2Ind;

    // Data
    C_Belt2Ind              m_primes;
    bool                    m_wrappedUp;

    // Nonvirtuals
    void wrapup();
};

//
//      Function Templates
//
template<class T>
int findSubseq(const std::vector<T> &super, const std::vector<T> &sub)
{
    int ret = -1;
    if (!super.empty())
    {
        for (auto i = super.begin(), end = super.end() - ssize(sub); i <= end; ++i)
        {
            bool match = true;
            auto j = i;
            for (auto m: sub)
                if (m != *j++)
                {
                    match = false;
                    break;
                }
            if (match)
            {
                ret = int(i - super.begin());
                break;
            }
        }
    }
    return ret;
}

//
//      Implement Class Templates
//
template<class T>
void C_PrimeBelts<T>::add(const C_Belt &belt)
{
    if (m_wrappedUp)
        RUNTIME_ERROR("Add after wrapup");

    // Find if any existing alphabet contains the new one
    bool found = false;
    for (auto &j: m_primes)
        if (findSubseq(j.first, belt) >= 0)
        {
            found = true;
            break;
        }
    if (!found)
    {
        // Eliminate properly contained alphaets
        for (auto j = m_primes.begin(); j != m_primes.end();)
            if (findSubseq(belt, j->first) >= 0)
                m_primes.erase(j++);
            else
                ++j;

        // Add new prime
        m_primes[belt];
    }
}

template<class T>
std::optional<std::pair<size_t,size_t>> C_PrimeBelts<T>::find(const C_Belt &belt)
{
    wrapup();
    for (auto &i: m_primes)
    {
        auto ind = findSubseq(i.first, belt);
        if (ind >= 0)
            return {{i.second, ind}};
    }
    return {};
}

template<class T>
void C_PrimeBelts<T>::visit(std::function<void(const C_Belt &belt, size_t ind)> apply)
{
    wrapup();
    for (const auto &i: m_primes)
        apply(i.first, i.second);
}

template<class T>
void C_PrimeBelts<T>::wrapup()
{
    if (!m_wrappedUp && !m_primes.empty())
    {
        size_t count = 0;
        for (auto &i: m_primes)
             i.second = count++;

        m_wrappedUp = true;
    }
}

} // namespace bux

struct C_GotoMap;

class FC_Output
{
public:

    // Nonvirtuals
    FC_Output(const C_ParserInfo &parsed, const C_GotoMap &stateMap,
        const C_ActionMap &actionMap, const std::string &grammarPath);
    bool operator()(const char *outputPath, const char *tokenPath, int flags) const;

private:

    // Types
    typedef std::pair<std::string,std::string> C_StrPair;
    struct C_IATableOutput
    {
        std::map<bux::T_LexID,C_Strings>    m_outMapping;
        int                                 m_iaTableInd;   // >=0 as table index; < 0 to compress
    };
    typedef std::map<C_Lex2Action,C_IATableOutput> C_ActionTblRendering;
    typedef std::map<std::string,size_t> C_StringToId;
    typedef std::map<const C_Production*,size_t> C_ProductionSerials;
    typedef bux::C_PrimeBelts<bux::T_LexID> C_AlphaPrimes;
    typedef C_AlphaPrimes::C_Belt C_Alphabet;

    typedef std::vector<std::pair<bux::T_LexID,size_t>> C_Lex2State;
    struct C_ISTableOutput
    {
        int                 m_tableInd;     // >=0 as table index; < 0 to compress
    };
    typedef std::map<C_Lex2State,C_ISTableOutput> C_StateTblRendering;

    class C_RenderReduction;

    // Data
    const C_ParserInfo      &m_Parsed;
    const std::string       m_ContextType;
    const std::string       m_Banner;

    // Deduced from actionMap
    std::map<bux::T_LexID,std::string>  m_lex2str;
    C_ActionTblRendering    m_iatbl2out;
    C_StateTblRendering     m_istbl2out;
    std::vector<C_StrPair>  m_state2iatbl;
    std::map<size_t,C_StrPair>  m_state2next;
    mutable C_AlphaPrimes   m_alphaPrimes;
    C_StringMap             m_findPrimes;
    std::string             m_InputType;
    std::string             m_StateType;
    std::string             m_iaTblSzType, m_isTblSzType;
    std::string             m_ActionType;
    std::string             m_PoplenType;
    bux::T_LexID            m_MinTokenId;

    // Deduced from m_Parsed.productions()
    C_ProductionSerials     m_ProdNums;

    // Deduced from m_Parsed.templateArgs()
#ifdef __TEMPLATE_OUTPUT
    std::string             m_TemplateParams;
    std::string             m_ClassSuffix;
#endif // __TEMPLATE_OUTPUT

    // Flags
    bool                    m_readySoFar{true};
    bool                    m_needGLR{};
    mutable bool            m_isKeyCalled{};

    // Nonvirtuals
    void addParserMap(C_RenderReduction &rr, const std::string &className, size_t argInd) const;
    bool hasContext() const { return !m_ContextType.empty(); }
    std::string outputFindKey(const C_Alphabet &alphabet);
    std::string outputIsKey(bux::T_LexID key) const;
    void outputTokens(std::ostream &out, const std::string &headerBase) const;
    size_t serial(const C_Production*) const;
    void writeUserSection(std::ostream &out, const char *optionKey) const;
};
