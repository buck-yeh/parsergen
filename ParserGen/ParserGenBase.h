#ifndef ParserGenBaseH
#define ParserGenBaseH

#include "ParserIdDef.h"    // Main::TID_xxxx
#include <functional>       // std::function<>
#include <list>             // std::list<>
#include <map>              // std::map<>
#include <set>              // std::set<>
#include <vector>           // std::vector<>

#ifdef _WIN32
#define PROGNAME            "parsergen.exe"
#else
#define PROGNAME            "parsergen"
#endif

#define ARG_NAME_PARSER     "_paRSeR_"
#define ARG_NAME_GETLEX     "_geT_"
#define ARG_NAME_PARSER_DOWNCAST(c) "dynamic_cast<"+c+"&>(" ARG_NAME_PARSER ")"
#define ARG_NAME_RESULT     "_reT_"

#define ENCODED_TOKEN_LB_str    "ENCODED_TOKEN_LB"

namespace ParserGen {

using namespace bux;

//
//      Types
//
struct I_ProductionTerm
{
    // Virtuals
    virtual ~I_ProductionTerm() =default;
    virtual bool operator==(const I_ProductionTerm&) const =0;
    virtual std::string displayStr() const =0;
    virtual bool generateId(std::string &id) const =0;

    // Nonvirtuals
    bool operator!=(const I_ProductionTerm &other) const
        { return !operator==(other); }
};

struct I_SemanticChunk
{
    virtual ~I_SemanticChunk() =0;
};

class C_Semantic: public std::list<I_SemanticChunk*>
{
public:

    // Nonvirtuals
    ~C_Semantic();
    std::string expand() const;
        // Return expanded string
};

struct C_Production
{
    // Types
    typedef std::vector<I_ProductionTerm*> C_Rvalue;

    // Data
    std::string             m_Lval;           // name of nonterminal
    C_Rvalue                m_Rval;

    // Viruals
    virtual ~C_Production() =0;
        // To make the class abstract

    // Nonvirtuals
    bool operator==(const C_Production &) const;
    std::string inputName(size_t pos, const std::string &mark) const;
    std::string str() const { return inputName(0, {}); }
};
typedef C_Production::C_Rvalue C_LexSeq;

struct C_IndexedProd: C_Production
{
    // Data
    size_t                  m_Index;        // unique
};
typedef std::list<C_IndexedProd> C_Productions;

enum E_Associativity
{
    ASSOC_NONE,
    LEFT2RIGHT,
    RIGHT2LEFT
};

struct C_Priority
{
    size_t                  m_Weight;
    E_Associativity         m_Assoc;
};
typedef std::map<T_LexID,C_Priority> C_PriorityMap;

#ifdef __TEMPLATE_OUTPUT
struct C_TemplateArgDecl
{
    std::string             m_TypeName;
    std::string             m_ArgDummy;
};
typedef std::list<C_TemplateArgDecl> C_TemplateArgDecls;
#endif // __TEMPLATE_OUTPUT

typedef std::map<std::string,std::string> C_StringMap;
typedef std::list<std::string> C_StringList;
typedef std::map<std::string,T_LexID> C_Str2Id;
typedef std::vector<std::string> C_Strings;

class C_ParserInfo
{
public:

    // Nonvirtuals
    C_ParserInfo(): m_IdSet(false)
        {}
    const std::string &className() const
        { return m_ClassName; } // FC_Output
    void enterNamespaces(std::ostream &out) const;
        //
    std::string fullNamespace() const;
        //
    const C_Str2Id &generatedLexes() const
        { return m_GenLex2Id; }
    const C_Str2Id &nonterminals() const
        { return m_Nonterm2Id; }
    const C_Semantic *getOption(const std::string &name) const;
        // Return 0 on failure
    const std::string *getReduction(size_t index) const;
        // Return 0 on failure. The index is 0-based.
    std::string getReduction(const C_IndexedProd &prod) const;
        //
    static size_t getReductionIndex(const C_IndexedProd &prod)
        { return prod.m_Index; }
    bool hasLexSymbol(const std::string &id) const;
        // Return true if id is already defined
    bool idSet() const
        { return m_IdSet; }
    bool ignoreKeywordCase() const
        { return getOption("IGNORE_KEYWORD_CASE") != nullptr; }
    void leaveNamespaces(std::ostream &out) const;
        //
    const C_Str2Id &lex2ID() const
        { return m_Lex2ID; }
    const C_StringMap &literalMap() const
        { return m_Literal2IdName; }
    std::string outputId(const I_ProductionTerm *attr) const;
        //
    const C_PriorityMap &priorityMap() const
        { return m_PriorityMap; }
    bool prodTerm2id(const I_ProductionTerm *attr, T_LexID &id, std::string &errMsg) const;
        // Return true on success
    T_LexID prodTerm2id(const I_ProductionTerm *attr) const;
        // RUNTIME_ERROR() on error
    const C_Productions &productions() const
        { return m_Productions; }
    std::string contextType() const;
        //
#ifdef __TEMPLATE_OUTPUT
    const C_TemplateArgDecls &templateArgs() const
        { return m_TemplateArgs; }
#endif // __TEMPLATE_OUTPUT

protected:

    // Types
    struct C_OptionValue: C_Semantic
    {
        mutable bool            m_Used;

        C_OptionValue(): m_Used(false) {}
    };
    typedef std::map<std::string,C_OptionValue> C_OptionMap;

    // Data
    C_StringList            m_Namespace;
    std::string             m_ClassName;
#ifdef __TEMPLATE_OUTPUT
    C_TemplateArgDecls      m_TemplateArgs;
#endif // __TEMPLATE_OUTPUT
    C_Str2Id                m_Lex2ID;
    C_OptionMap             m_OptMap;
    C_PriorityMap           m_PriorityMap;
    C_Str2Id                m_GenLex2Id, m_Nonterm2Id;
    C_StringMap             m_Literal2IdName;
    C_Productions           m_Productions;
    C_Strings               m_Reductions;
    bool                    m_IdSet;    // Ids of m_Lex2ID & m_Gene2Id are generated

    // Nonvirtuals
    void wrapup();
};

class I_Terminal: public I_ProductionTerm, public I_LexAttr
{
};

class C_StrLiteral: public I_Terminal
{
public:

    // Data
    const std::string   m_Str;
    const char          m_QuoteMark;

    // Nonvirtuals
    C_StrLiteral(std::string_view s, char quoteMark =0);
    C_StrLiteral(size_t n, char c, char quoteMark =0);

    // Implement I_ProductionTerm
    bool operator==(const I_ProductionTerm&) const override;
    std::string displayStr() const override;
    bool generateId(std::string &id) const override;
};

struct C_Id: C_StrLiteral
{
    C_Id(std::string_view s): C_StrLiteral(s)
        {}
};

class C_LexSymbol: public I_Terminal, public I_SemanticChunk
{
public:

    // Data
    const std::string   m_Var;

    // Nonvirtuals
    C_LexSymbol(std::string_view var): m_Var(var)
        {}

    // Implement I_ProductionTerm
    bool operator==(const I_ProductionTerm&) const override;
    std::string displayStr() const override;
    bool generateId(std::string &id) const override;
};

class C_Nonterminal: public I_ProductionTerm, public I_LexAttr
{
public:

    // Data
    const std::string       m_id;

    // Nonvirtuals
    C_Nonterminal(std::string_view id): m_id(id)
        {}

    // Implement I_ProductionTerm
    bool operator==(const I_ProductionTerm&) const override;
    std::string displayStr() const override;
    bool generateId(std::string &id) const override;
};

class FC_LessLex
{
public:

    // Nonvirtuals
    FC_LessLex(const C_ParserInfo &parsed) noexcept: m_Parsed(parsed)
        {}
    FC_LessLex(const FC_LessLex &other) noexcept: m_Parsed(other.m_Parsed)
        {}
    bool operator()(const I_ProductionTerm *, const I_ProductionTerm *) const;

private:

    // Data
    const C_ParserInfo      &m_Parsed;

    // Nonvirtuals
    T_LexID id(const I_ProductionTerm *term) const;
};

template<class T>
struct C_LexSetT: std::set<const T*,FC_LessLex>
{
    C_LexSetT(const C_ParserInfo &parsed): std::set<const T*,FC_LessLex>(parsed) {}
};
typedef C_LexSetT<I_Terminal> C_Lookahead;

typedef std::pair<const C_IndexedProd*, size_t> C_ReductionStep;
typedef std::map<C_ReductionStep,C_Lookahead> C_StateItems;

struct C_State
{
    C_StateItems            m_rstep2la;
    size_t                  m_id;
};
typedef std::list<C_State> C_States;

class C_BracketedLex: public I_SemanticChunk, public bux::I_LexAttr
{
public:

    // Data
    std::string         m_Str;

    // Nonvirtuals
    C_BracketedLex(const std::string &s): m_Str(s)
        {}
};

//
//      Externs
//
extern std::function<bool(const std::string &str, std::string &dst)> getGeneratedID;

bool getNonterminalID(const std::string &str, std::string &dst);
bool isKeyword(const std::string &str);
std::string literalSuffix(const std::string &s);
std::string ensureNoConcat(std::string s);
std::string ensureNoConcat(const C_Semantic &src);

} // namespace ParserGen
using namespace ParserGen;

#endif // ParserGenBaseH
