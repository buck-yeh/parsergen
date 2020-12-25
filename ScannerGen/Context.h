#ifndef ContextH
#define ContextH

#include "ParserIdDef.h"    // TID_LEX_Epsilon
//--------------------------------------------------------------------
#include "bux/FA.h"         // bux::C_NFA<>
#include "bux/Intervals.h"  // bux::C_Intervals<>
#include "bux/LexBase.h"    // bux::T_LexID
#include "bux/Logger.h"     // bux::E_LogLevel
#include <filesystem>       // std::filesystem::path
#include <map>              // std::map<>
#include <vector>           // std::vector<>

#undef min

//
//      Constants
//
enum
{
    TOTAL_ERRORLEVEL    =5  // LL_FATAL, LL_ERROR, LL_WARNING, LL_INFO, LL_VERBOSE
};

//
//      Types
//
typedef std::vector<std::string> C_StrList;

struct C_WeightedStrList
{
    // Data
    C_StrList   m_Expr, m_isFinal;
    int         m_Weight{0};
    bool        m_1stFit{false};

    // Nonvirtuals
    C_WeightedStrList() = default;
    C_WeightedStrList(const C_WeightedStrList &a) = default;
    C_WeightedStrList(int w, C_StrList &expr, C_StrList *isFinal = nullptr);
    void operator=(C_WeightedStrList &a);
    C_WeightedStrList &operator=(const C_WeightedStrList &a) = default;
    bool operator<(const C_WeightedStrList &other) const;
    bool operator==(const C_WeightedStrList &other) const;
};

typedef bux::C_Intervals<bux::T_LexID> C_LexSet;
typedef C_LexSet::value_type C_Interval;

typedef bux::C_NFA<C_LexSet,C_WeightedStrList> C_LexNfa;

struct C_NfaLex: bux::I_LexAttr
{
    // Data
    C_LexNfa            m_NFA;
    bool                m_1stFit{false};

    // Nonvirtuals
    C_NfaLex() = default;
    C_NfaLex(const C_LexNfa &nfa): m_NFA(nfa) {}
    C_NfaLex(C_LexNfa &&nfa): m_NFA(nfa) {}
};
typedef std::map<std::string,C_NfaLex*> C_RegExprMap;

typedef std::map<std::string,C_StrList> C_Options;
typedef std::function<void(const std::string&)> FH_ApplyLexTerm;
typedef std::vector<std::filesystem::path> C_Paths;

class C_Context
{
public:

    // Data
    const C_Paths       &m_IncDirs;
    size_t              m_ErrorTotal[TOTAL_ERRORLEVEL]{};

    // Nonvirtuals
    explicit C_Context(const C_Paths &inc_dirs): m_IncDirs(inc_dirs) {}
    ~C_Context();
    void addOption(const std::string &name, C_StrList &value);
    void addRE(const std::string &name, C_NfaLex &val);
    bool eraseRE(const std::string &name);
    std::string expand_include(const std::string &org_path) const;
    const C_LexNfa *finalExpr() const;
    const C_NfaLex *findRE(const std::string &name) const;
    bool forEachOptionTerm(const std::string &name, FH_ApplyLexTerm apply) const;
    bool getOptionString(const std::string &name, std::string &value) const;
    void issueError(bux::E_LogLevel level, const bux::C_SourcePos &pos,
        const std::string &message);

private:

    // Data
    C_Options           m_Options;
    C_RegExprMap        m_PoolRE;
    std::string         m_BuildName;

    // Nonvirtuals
    bool forEachOptionTerm_(const std::string &name, C_StrList &fifo, FH_ApplyLexTerm apply) const;
};

//
//      Functions
//
std::string expand(const C_StrList &src, const C_Context &c);
std::string getOptionString(const C_Context &c, const std::string &name, const std::string &defString);

#endif // ContextH
