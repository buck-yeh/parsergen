#ifndef BNFContextH
#define BNFContextH

#include "ParserGenBase.h"  // C_ParserInfo, ...
//-------------------------------------------------------
#include "bux/Logger.h"     // bux::E_LogLevel
#include "bux/LR1.h"        // bux::LR1::C_LexPtr
#include <filesystem>       // std::filesystem::path

namespace ParserGen {

//
//      Constants
//
enum
{
    TOTAL_ERRORLEVEL = 5  // LL_FATAL, LL_ERROR, LL_WARNING, LL_INFO, LL_VERBOSE
};

//
//      Types
//
typedef std::list<C_StringList> C_TemplateArgs;
typedef std::vector<std::filesystem::path> C_Paths;

class C_BNFContext: public C_ParserInfo
{
public:

    // Data
    const C_Paths   &m_IncDirs;
    size_t          m_ErrorTotal[TOTAL_ERRORLEVEL]{};

    // Ctor/Dtor
    explicit C_BNFContext(const C_Paths &inc_dirs): m_IncDirs(inc_dirs) {}
    ~C_BNFContext();

    // Nonvirtuals
    bool activateBuiltinNonterminal(const std::string &id);
    bool addGenId(const std::string &id);
    bool addLexId(const std::string &id);
    void addIdFromToken(bux::I_LexAttr *attr);
    void addLiteral(I_ProductionTerm *term, const std::string &id);
    void addOption(const std::string &key, C_Semantic *value);
    void addPriority(E_Associativity assoc, bux::LR1::C_LexPtr &src);
    bool addProduction(C_Production &prod, C_Semantic *semantic);
    [[nodiscard]] C_LexOrError attr2id(bux::I_LexAttr *attr);
    bool checkSemanticlessProductions() const;
        // Return true if semanticless productions are found
    bool checkUnusedOptions() const;
        // Return true if unused options are found
    bool equal(const C_Semantic &a, const C_Semantic &b) const;
    std::string expand_include(const std::string &org_path) const;
    void incWeight() { ++m_PriorWeight; }
    void log(bux::E_LogLevel level, const bux::C_SourcePos &pos, std::string_view  message);
    [[nodiscard]] std::optional<std::string> setClassName(C_StringList &qualified, C_TemplateArgs &targs);
    size_t tempReductionIndex(const C_IndexedProd &prod) const;
    void wrapup(const bux::C_SourcePos &pos);

    // Nonvirtuals: #ifdef #ifndef #else #endif
    void ifCond(bool y, const C_SourcePos &pos);
    void elseCond(const C_SourcePos &pos);
    void endifCond(const C_SourcePos &pos);
    bool testCond() const;

private:

    // Types
    struct C_PriorityRaw
    {
        // Data
        I_LexAttr           *m_pAttr;   // owned
        size_t              m_Weight;
        E_Associativity     m_Assoc;
    };
    typedef std::list<C_PriorityRaw> C_PriorityRaws;

    typedef std::map<std::string,size_t> C_Str2Num;

    typedef std::vector<const C_IndexedProd*> C_ProductionPtrs;

    struct C_OntoInfo
    {
        // Data
        C_Semantic          m_Reduction;
        C_ProductionPtrs    m_Productions;
        size_t              m_Index;        // unique

        void semantic(C_Semantic &t) { m_Reduction.swap(t); }
    };
    typedef std::list<C_OntoInfo> C_OntoPool;
    typedef std::vector<const C_OntoInfo*> C_OntoPtrs;
    typedef std::set<std::string> C_StringSet;

    struct C_CondFrame
    {
        C_SourcePos         m_pos;
        bool                m_yes;

        C_CondFrame(bool yes, const C_SourcePos &pos): m_pos(pos), m_yes(yes) {}
    };
    typedef std::vector<C_CondFrame> C_CondStack;

    // Data
    C_PriorityRaws          m_PriorRaws;
    C_StringSet             m_GeneTokens;
    C_OntoPool              m_OntoPool;
    C_OntoPtrs              m_OntoJumps;
    C_ProductionPtrs        m_Semanticless;
    C_CondStack             m_CondStack;
    size_t                  m_PriorWeight{1};
    int                     m_BuilinNonterminalMask{};

    // Nonvirtuals (%%%%% superseded by C_Semantics::expand() when C_CallProductionLex obsoletes)
    void normalize(const C_Semantic &src, C_Semantic &dst) const;
    void normalize(const C_Semantic &src, C_Semantic &dst, std::string &remains) const;
};

class C_OptionLex: public I_SemanticChunk, public I_LexAttr
{
public:

    // Data
    const std::string       m_Var;

    // Nonvirtuals
    C_OptionLex(const std::string &var): m_Var(var)
        {}
};

class C_ProductionLex: public C_Production, public bux::I_LexAttr
{
};

} // namespace ParserGen

#endif // BNFContextH
