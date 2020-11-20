#include "Context.h"
#include <fmt/core.h>   // fmt::print()
#include <limits>       // std::numeric_limits<>

namespace {

//
//      In-Module Types
//
struct FC_AddStr
{
    // Data
    std::string         &m_Value;

    // Nonvirtuals
    FC_AddStr(std::string &value): m_Value(value) {}
    void operator()(const std::string &term) { m_Value +=term; }
};

} // namespace

//
//      Functions
//
std::string expand(const C_StrList &src, const C_Context &c)
{
    std::string ret;
    for (C_StrList::const_iterator i =src.begin(), end =src.end(); i != end; ++i)
    {
        std::string value;
        if (!c.getOptionString(*i, value))
            value =*i;
        if (value.empty())
            continue;
        if (size_t n =ret.size())
        {
            const char lastCh =ret[n-1];
            if (isalnum(lastCh) && isalnum(value[0]))
                ret +=' ';
        }
        ret +=value;
    }
    return ret;
}

std::string getOptionString(const C_Context &c, const std::string &name, const std::string &defString)
{
    std::string ret;
    return c.getOptionString(name, ret)? ret: defString;
}

//
//      Implement Classes
//
C_Context::C_Context()
{
    for (size_t i =0; i < TOTAL_ERRORLEVEL; m_ErrorTotal[i++] =0);
}

C_Context::~C_Context()
{
    for (C_RegExprMap::const_iterator i =m_PoolRE.begin(); i != m_PoolRE.end(); ++i)
        delete i->second;
}

void C_Context::addOption(const std::string &name, C_StrList &value)
{
    if (name == "CPP_INCLUDE")
        // Special treat
    {
        C_StrList &dst = m_Options["HEADERS_FOR_CPP"];
        dst.emplace_back("#include ");
        dst.insert(dst.end(), value.begin(), value.end());
        dst.emplace_back("\n");
    }
    else if (
        name == "HEADERS_FOR_CPP" ||
        name == "HEADERS_FOR_HEADER" ||
        name == "LOCAL_ACTION_DEFS")
        // Append value
    {
        C_StrList &dst = m_Options[name];
        dst.insert(dst.end(), value.begin(), value.end());
    }
    else
        // Assign value
        m_Options[name].swap(value);
}

void C_Context::addRE(const std::string &name, C_NfaLex &val)
{
    m_PoolRE[name] =&val;
    m_BuildName =name; // The lat ladded is assumed as the build root
}

bool C_Context::eraseRE(const std::string &name)
{
    C_RegExprMap::iterator found =m_PoolRE.find(name);
    if (found != m_PoolRE.end())
    {
        delete found->second;
        m_PoolRE.erase(found);
        return true;
    }
    return false;
}

const C_LexNfa *C_Context::finalExpr() const
{
    C_RegExprMap::const_iterator found =m_PoolRE.find(m_BuildName);
    if (found != m_PoolRE.end())
        return &found->second->m_NFA;
    return 0;
}

const C_NfaLex *C_Context::findRE(const std::string &name) const
{
    C_RegExprMap::const_iterator found =m_PoolRE.find(name);
    if (found != m_PoolRE.end())
        return found->second;

    return 0;
}

bool C_Context::forEachOptionTerm(const std::string &name, FH_ApplyLexTerm apply) const
{
    C_StrList fifo;
    return forEachOptionTerm_(name, fifo, apply);
}

bool C_Context::forEachOptionTerm_(const std::string &name, C_StrList &fifo, FH_ApplyLexTerm apply) const
{
    for (auto &i: fifo)
    {
        if (i == name)
            // Recursive visit -- Treat as not found
            return false;
    }

    auto found = m_Options.find(name);
    if (found != m_Options.end())
    {
        fifo.emplace_back(name);
        for (auto &i: found->second)
        {
            if (!forEachOptionTerm_(i, fifo, apply))
                apply(i);
        }
        fifo.pop_back();
        return true;
    }
    return false;
}

bool C_Context::getOptionString(const std::string &name, std::string &value) const
{
    const FC_AddStr apply(value);
    return forEachOptionTerm(name, apply);
}

void C_Context::issueError  (
    bux::E_LogLevel         level,
    const bux::C_SourcePos  &pos,
    const std::string       &message    )
{
    static const char *const KIND[] ={
        ") FATAL: ",
        ") ERROR: ",
        ") WARNING: ",
        ") INFO: ",
        ") VERBOSE: "
    };
    fmt::print("{}({},{}{}{}\n", pos.m_Source, pos.m_Line, pos.m_Col, KIND[level], message);
    static const size_t LIMIT[] ={
        0,
        10,
        100,
        std::numeric_limits<size_t>::max(),
        std::numeric_limits<size_t>::max()
    };
    if (++m_ErrorTotal[level] > LIMIT[level])
        RUNTIME_ERROR("Too many errors !")
}

C_WeightedStrList::C_WeightedStrList(int w, C_StrList &expr, C_StrList *isFinal):
    m_Expr(std::move(expr)),
    m_Weight(w)
{
    if (isFinal)
        m_isFinal = std::move(*isFinal);
}

void C_WeightedStrList::operator=(C_WeightedStrList &a)
{
    m_Weight    = a.m_Weight;
    m_1stFit    = a.m_1stFit;
    m_Expr      = std::move(a.m_Expr);
    m_isFinal   = std::move(a.m_isFinal);
}

bool C_WeightedStrList::operator<(const C_WeightedStrList &other) const
{
    if (int t = m_Weight - other.m_Weight)
        return t < 0;
    if (int t = m_1stFit - other.m_1stFit)
        return t < 0;
    return m_Expr < other.m_Expr;
}

bool C_WeightedStrList::operator==(const C_WeightedStrList &other) const
{
    return  m_Weight == other.m_Weight &&
            m_1stFit == other.m_1stFit &&
            m_Expr   == other.m_Expr;
}
