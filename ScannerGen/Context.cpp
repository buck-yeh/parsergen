#include "Context.h"
//--------------------------------------------------------------------
#include "bux/FsUtil.h"     // bux::search_dirs()
#include "bux/StrUtil.h"    // bux::expand_env()
#include <limits>           // std::numeric_limits<>
#include <print>            // std::print()

namespace fs = std::filesystem;

//
//      Functions
//
std::string expand(const C_StrList &src, const C_Context &c)
{
    std::string ret;
    for (auto &i :src)
    {
        std::string value;
        if (auto t = c.getOptionString(i))
            value = *t;
        else
            value = i;

        if (!value.empty())
        {
            if (!ret.empty() && isalnum(ret.back()) && isalnum(value.front()))
                ret += ' ';

            ret += value;
        }
    }
    return ret;
}

std::string getOptionString(const C_Context &c, const std::string &name, const std::string &defString)
{
    if (auto ret = c.getOptionString(name))
        return *ret;

    return defString;
}

//
//      Implement Classes
//
C_Context::~C_Context()
{
    for (C_RegExprMap::const_iterator i =m_PoolRE.begin(); i != m_PoolRE.end(); ++i)
        delete i->second;
}

void C_Context::addOption(const std::string &name, C_StrList &value)
{
    if (name == "HEADERS_FOR_CPP" ||
        name == "HEADERS_FOR_HEADER" ||
        name == "LOCAL_ACTION_DEFS")
        // Append value
    {
        auto &dst = m_Options[name];
        dst.insert(dst.end(), value.begin(), value.end());
    }
    else
        // Assign value
        m_Options[name].swap(value);
}

bool C_Context::addRE(const std::string &name, C_NfaLex &val)
{
    const auto ret = m_PoolRE.try_emplace(name, &val);
    if (!ret.second)
    {
        delete ret.first->second;
        ret.first->second = &val;
    }
    m_BuildName = name; // The last added is always assumed as the build root
    return ret.second;
}

bool C_Context::eraseRE(const std::string &name)
{
    if (auto found = m_PoolRE.find(name); found != m_PoolRE.end())
    {
        delete found->second;
        m_PoolRE.erase(found);
        return true;
    }
    return false;
}

std::string C_Context::expand_include(const std::string &org_path) const
{
    return bux::search_dirs(bux::expand_env(org_path.c_str()), m_IncDirs).string();
}

const C_LexNfa *C_Context::finalExpr() const
{
    if (auto found = findRE(m_BuildName))
        return &found->m_NFA;

    return 0;
}

const C_NfaLex *C_Context::findRE(const std::string &name) const
{
    if (auto found = m_PoolRE.find(name); found != m_PoolRE.end())
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

std::optional<std::string> C_Context::getOptionString(const std::string &name) const
{
    std::string value;
    if (forEachOptionTerm(name, [&](auto &t){ value += t; }))
        return value;

    return {};
}

void C_Context::log(bux::E_LogLevel level, const bux::C_SourcePos &pos, std::string_view message)
{
    constinit static const char *const KIND[] ={
        ") FATAL: ",
        ") ERROR: ",
        ") WARNING: ",
        ") INFO: ",
        ") VERBOSE: "
    };
    std::print("{}({},{}{}{}\n", pos.m_Source, pos.m_Line, pos.m_Col, KIND[level], message);
    constinit static const size_t LIMIT[] ={
        0,
        10,
        100,
        std::numeric_limits<size_t>::max(),
        std::numeric_limits<size_t>::max()
    };
    if (++m_ErrorTotal[level] > LIMIT[level])
        RUNTIME_ERROR("Too many errors !");
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
