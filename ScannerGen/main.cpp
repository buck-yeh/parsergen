//#define __DEBUG_MINDFA

//
//  ALGORITHM SOURCES:
//  1. Rienhard Wilhelm and Dieter Maurer(1995). Compiler Design. Addison-Wesley.
//      * Figure 7.3 (p243): Rules for the algorithm RE->NFA
//      * Figure 7.5 (p245): Algorithm NFA->DFA
//      * Figure 7.7 (p247): Algorithm MinDFA
//
#include "Parser.h"         // C_ScannerParser
#include "ParserIdDef.h"    // TID_LEX_Spaces
#include "Scanner.h"        // C_ScannerScanner
//------------------------------------------------------------------------------
#include "bux/EZArgs.h"     // bux::C_EZArgs
#include "bux/FA.h"         // bux::C_DFA
#include "bux/LogStream.h"  // HRTN()
#include "bux/MemIn.h"      // bux::C_IMemStream<>
#include "bux/Range2Type.h" // bux::fittestType()
#include "bux/XConsole.h"   // bux::testWritability()
#include <filesystem>       // std::filesystem::path
#include <fmt/ostream.h>    // fmt::print() for std::ostream
#include <fstream>          // std::ifstream
#include <iostream>         // std::cin, std::cerr
#include <limits>           // std::numeric_limits<>
#include <cstring>          // strlen()

namespace fs = std::filesystem;

void parseFile(const std::string &filename, C_ScannerParser &parser, bux::T_LexID endToken);

//
//      Constants
//
#define PROGNAME "scannergen"
const char USERPREFIX[] ="create_";

enum
{
    //
    //      Version Macros
    //
    VERSION_MAJOR           = 1,
    VERSION_MINOR           = 5,
    VERSION_RELEASE         = 1,
    //
    //      Error Codes
    //
    LEVEL_OK                = 0,
    LEVEL_HELP,
    LEVEL_NOT_ACCEPTED,
    LEVEL_PARSE_ERROR,
    LEVEL_TEST_H,
    LEVEL_OPEN_H,
    LEVEL_TEST_CPP,
    LEVEL_OPEN_CPP,
    LEVEL_STD_EXCEPTION,
    LEVEL_UNKNOWN_EXCEPTION
};

namespace {
//
//      In-Module Types
//
typedef bux::C_DFA<C_LexSet,C_WeightedStrList> C_LexDfa;

class FC_GetLimits
{
public:

    // Nonvirtuals
    FC_GetLimits();
    void operator()(int state, const C_LexSet &input, int nextState);
    std::string inputType() const;
    std::string stateType() const;

private:

    // Data
    bux::T_LexID            m_InputMin;
    bux::T_LexID            m_InputMax;
    int                     m_StateMin;
    int                     m_StateMax;
};

class C_Output
{
public:

    // Nonvirtuials
    C_Output(const C_LexDfa &dfa, C_Context &context);
    void writeCpp(std::ostream &out, const std::string &base) const;
    void writeHeader(std::ostream &out, const std::string &base) const;

private:

    // Data
    const C_LexDfa          &m_DFA;
    C_Context               &m_context;
    FC_GetLimits            m_Limits;
    C_StrList               m_NS;
    std::string             m_baseClass, m_buxNS;
    std::string             m_Prefix;

    // Nonvirtuals
    void enterNamespaces(std::ostream &out) const;
    void leaveNamespaces(std::ostream &out) const;
    void writeUserSection(std::ostream &out, const char *optionKey) const;
};

struct FC_SolveConflict
{
    // Types
    typedef std::set<C_WeightedStrList> C_Actions;

    // Data
    const C_Context         &m_context;

    // Ctor
    FC_SolveConflict(const C_Context &context): m_context(context) {}
    const C_WeightedStrList &operator()(int state, const C_Actions &actions);
};

class FC_CollectNS
{
public:

    // Nonvirtuals
    FC_CollectNS(C_StrList &ns): m_NS(ns), m_ShouldBeId(true) {}
    void operator()(const std::string &term);

private:

    // Data
    C_StrList       &m_NS;
    bool            m_ShouldBeId;
};

//
//      In-Module Functions
//
std::string banner()
{
    return fmt::format("// This file is fully generated by running " PROGNAME " v{}.{}.{}\n",
        VERSION_MAJOR,
        VERSION_MINOR,
        VERSION_RELEASE);
}

void parseFile(const std::string &filename, std::istream &in, C_ScannerParser &parser, bux::T_LexID endToken)
{
    fmt::print("About to parse '{}' ...\n", filename);

    bux::C_ScreenerNo<TID_LEX_Spaces>   preparser(parser);
    C_ScannerScanner                    scanner(preparser);
    const auto                          curFile = parser.setSource(filename);

    bux::scanFile(filename, in, scanner, endToken);
    parser.setSource(curFile);
}

//
//      In-Mudole Class Implementations
//
void FC_CollectNS::operator()(const std::string &term)
{
    if (m_ShouldBeId)
    {
        if (!bux::isIdentifier(term))
            RUNTIME_ERROR("Namespace contains non-identifier term \"{}\"", term);

        m_NS.emplace_back(term);
        m_ShouldBeId =false;
    }
    else
    {
        if (term != "::")
            RUNTIME_ERROR("Namespace should separate identifiers only by \"::\"");

        m_ShouldBeId =true;
    }
}

C_Output::C_Output(const C_LexDfa &dfa, C_Context &context):
    m_DFA(dfa),
    m_context(context)
{
    context.forEachOptionTerm("NAMESPACE", FC_CollectNS(m_NS));

    if (context.getOptionString("NAMEPREFIX",m_Prefix))
    {
        if (!bux::isIdentifier(m_Prefix))
            RUNTIME_ERROR("Illegal prefix contents: {}", m_Prefix);
    }

    m_buxNS = "bux::";
    if (!m_NS.empty() && m_NS.front() == "bux")
        m_buxNS.clear();

    m_DFA.eachTransition(std::ref(m_Limits));
    m_baseClass.assign(m_buxNS+"C_ScannerImpl<")
               .append(m_Limits.inputType())
               .append(1, ',')
               .append(m_Limits.stateType())
               .append(1, ',');
    std::string chType;
    if (!m_context.getOptionString("CHAR_TYPE", chType))
    {
        chType = m_buxNS + "C_LexUTF32";
        C_StrList value;
        value.emplace_back(chType);
        m_context.addOption("CHAR_TYPE", value);
    }
    m_baseClass += m_buxNS.empty() && !chType.compare(0, 5, "bux::")? chType.substr(5): chType;

    std::string traitsType;
    if (m_context.getOptionString("CHAR_TRAITS_TYPE", traitsType))
        m_baseClass.append(1,',').append(traitsType);
    m_baseClass += '>';
}

void C_Output::enterNamespaces(std::ostream &out) const
{
    if (!m_NS.empty())
    {
        for (const auto &i: m_NS)
            out <<"namespace " <<i <<" {\n";

        out <<'\n';
    }
}

void C_Output::leaveNamespaces(std::ostream &out) const
{
    if (!m_NS.empty())
    {
        out <<'\n';
        for (auto i =m_NS.rbegin(); i != m_NS.rend(); ++i)
            out <<"} // namespace " <<*i <<"\n";
    }
}

void C_Output::writeCpp(std::ostream &out, const std::string &base) const
{
    out <<banner()
        <<"\n";
    writeUserSection(out, "HEADERS_FOR_CPP");
    out <<"#include \"" <<base <<".h\"\n"
          "\n"
          "namespace {\n"
          "\n";
    bool first;
    if (!m_NS.empty())
    {
        out <<"using namespace ";
        first =true;
        for (const auto &i: m_NS)
        {
            if (first)
                first =false;
            else
                out <<"::";

            out <<i;
        }
        out <<";\n"
              "\n";
    }

    typedef std::map<C_Interval,int> C_Interval2State;
    typedef std::map<int,C_Interval2State> C_RawTransitMap;

    C_RawTransitMap rawTrans;
    m_DFA.eachTransition([&rawTrans](int state, const C_LexSet &inputs, int nextState) {
        auto &dst = rawTrans[state];
        for (auto i: inputs)
            dst[i] = nextState;
    });

    typedef std::pair<bux::T_LexID,int> C_LexStatePair;
    typedef std::vector<C_LexStatePair> C_Lex2State;
    typedef std::map<C_Lex2State,int> C_StateMap2Index;
    typedef std::map<int,C_StateMap2Index::iterator> C_CookedTransitMap;

    C_StateMap2Index    cookedMaps;
    C_CookedTransitMap  cookedTrans;
    for (const auto &i: rawTrans)
    {
        C_Lex2State cookedMap;
        first =true;
        bux::T_LexID prevUB;
        for (const auto &j: i.second)
        {
            if (first)
                first =false;
            else if (++prevUB < j.first.first)
                cookedMap.emplace_back(prevUB, std::numeric_limits<int>::max());

            cookedMap.emplace_back(j.first.first, j.second);
            prevUB = j.first.second;
        }
        if (prevUB != std::numeric_limits<bux::T_LexID>::max())
            cookedMap.emplace_back(prevUB+1, std::numeric_limits<int>::max());

        cookedTrans[i.first] = cookedMaps.insert(C_StateMap2Index::value_type(cookedMap,0)).first;
    }

    if (!cookedMaps.empty())
    {
        out <<"// ASCII TABLE:";
        for (int i =0; i < 128; ++i)
        {
            if (i %0x20 == 0)
                out <<"\n// " <<std::hex <<std::setw(2) <<i <<std::dec <<": ";

            out <<bux::asciiLiteral(char(i));
        }

        int tag =0;
        out <<"\n"
              "constinit const C_" <<m_Prefix <<"Scanner::C_GotoPair\n";
        for (auto &i: cookedMaps)
        {
            i.second = ++tag;
            out <<"GOTO_STATE" <<tag <<'[' <<i.first.size() <<"] =";
            first = true;
            for (const auto &j: i.first)
            {
                out <<(first?'{':',') <<"\n"
                      "    {";
                if (isascii(j.first))
                    out <<'\'' <<bux::asciiLiteral(char(j.first)) <<"\', ";
                else
                    out <<"0x" <<std::hex <<j.first <<std::dec <<", ";

                if (j.second != std::numeric_limits<int>::max())
                    out <<j.second <<'}';
                else
                    out <<"std::numeric_limits<" <<m_Limits.stateType() <<">::max()}";
                first = false;
            }
            out <<(tag < decltype(tag)(cookedMaps.size())? "},\n": "};\n");
        }
    }

    std::map<int, std::tuple<std::string,size_t,std::string>> stateMap;
    std::map<int, std::string> firstFits;
    for (const auto &i: cookedTrans)
    {
        auto &t = stateMap[i.first];
        std::get<0>(t) = "GOTO_STATE" + std::to_string(i.second->second);
        std::get<1>(t) = i.second->first.size();
    }
    m_DFA.eachFinalState([&](int state, const C_WeightedStrList &action) {
        std::get<2>(stateMap[state]) = expand(action.m_Expr,m_context);
        auto isFinal = expand(action.m_isFinal,m_context);
        if (action.m_1stFit)
            firstFits[state] = isFinal;
        else if (!isFinal.empty())
            fmt::print("Warning: Non-first-fit action has isFinal: {}\n", isFinal);
    });
    writeUserSection(out, "LOCAL_ACTION_DEFS");
    if (!stateMap.empty())
    {
        const size_t ARR_SIZE = stateMap.crbegin()->first + 1;
        out <<"\n"
              "constinit const C_" <<m_Prefix <<"Scanner::C_StateRec STATES[" <<ARR_SIZE <<"] = {\n";
        int stopState = 0;
        for (const auto &i: stateMap)
        {
            while (stopState < i.first)
                out <<"    { nullptr, nullptr },  // " <<(stopState++) <<'\n';

            const auto val0 = std::get<0>(i.second);
            const auto val2 = std::get<2>(i.second);
            out <<"    { " <<(val0.empty()?"nullptr":val0) <<",\t" <<(val2.empty()?"nullptr":val2)
                <<(i.first + 1U < ARR_SIZE? "\t},  ": "\t}}; ") <<"// " <<(stopState++) <<'\n';
        }
        out <<"\n"
              "constinit const " <<m_Limits.inputType() <<" GOTO_SIZE[" <<ARR_SIZE <<"] = {\n";
        stopState = 0;
        for (const auto &i: stateMap)
        {
            while (stopState < i.first)
                out <<"    0,\t// " <<(stopState++) <<'\n';

            out <<"    " <<std::get<1>(i.second)
                <<(i.first + 1U < ARR_SIZE? ",": "};") <<"\t// " <<(stopState++) <<'\n';
        }
    }
    if (!firstFits.empty())
    {
        out <<"\n"
              "constinit const " <<m_Limits.stateType() <<" FIRST_FITS[" <<firstFits.size() <<"] = {\n";
        first = true;
        for (auto &i: firstFits)
        {
            if (first)
                first = false;
            else
                out <<",\n";

            out <<"    " <<i.first;
        }
        out <<"};\n"
              "\n"
              "constinit C_" <<m_Prefix <<"Scanner::F_IsFinal *const IS_FINALS[" <<firstFits.size() <<"] = {\n";
        first = true;
        for (auto &i: firstFits)
        {
            if (first)
                first = false;
            else
                out <<",\n";

            out <<"    " <<(i.second.empty()? "nullptr": i.second);
        }
        out <<"};\n";
    }
    out <<"\n"
          "} // namespace\n"
          "\n";
    enterNamespaces(out);
    out <<"C_" <<m_Prefix <<"Scanner::C_" <<m_Prefix <<"Scanner(" <<m_buxNS <<"I_Parser &parser):\n"
          "    " <<m_baseClass <<"(parser)\n"
          "{\n";
    if (!stateMap.empty())
        out <<"    stateTables(STATES, GOTO_SIZE);\n";
    if (!firstFits.empty())
        out <<"    firstFits(FIRST_FITS, IS_FINALS, " <<firstFits.size() <<");\n";
    out <<"}\n";
    leaveNamespaces(out);
}

void C_Output::writeHeader(std::ostream &out, const std::string &base) const
{
    out <<banner()
        <<"\n"
          "#ifndef " <<base <<"H\n"
          "#define " <<base <<"H\n"
          "\n";

    out <<"#include <bux/ImplScanner.h>\n";
    writeUserSection(out, "HEADERS_FOR_HEADER");
    out <<'\n';
    enterNamespaces(out);
    out <<"class C_" <<m_Prefix <<"Scanner: public " <<m_baseClass <<"\n"
          "{\n"
          "public:\n"
          "    C_" <<m_Prefix <<"Scanner(" <<m_buxNS <<"I_Parser &parser);\n"
          "};\n";
    leaveNamespaces(out);
    out <<"\n"
          "#endif // " <<base <<"H\n";
}

void C_Output::writeUserSection(std::ostream &out, const char *optionKey) const
{
    std::string value;
    if (m_context.getOptionString(optionKey, value))
    {
        out <<"// User-defined %" <<optionKey <<" begins\n";
        bool first =true;
        for (const auto &i: value)
            if (i != '\r')
            {
                if (first)
                {
                    first =false;
                    if (i == '\n')
                        continue;
                }
                out <<i;
            }
        out <<"// User-defined %" <<optionKey <<" ends\n"
              "\n";
    }
}

const C_WeightedStrList &FC_SolveConflict::operator()(int state, const C_Actions &actions)
{
    int count = 0, maxWeight;
    auto ret = actions.end();
    for (auto i = actions.begin(); i != actions.end(); ++i)
        if (!expand(i->m_Expr,m_context).compare(0, strlen(USERPREFIX), USERPREFIX))
        {
            if (!count++)
                // First of all create_*
                ret = i;
        }
    switch (count)
    {
    case 1:
        return *ret;
    case 0:
        count = 0;
        ret = actions.end();
        maxWeight = std::numeric_limits<int>::min();
        for (auto i =actions.begin(); i != actions.end(); ++i)
        {
            if (i->m_Weight > maxWeight)
            {
                maxWeight =i->m_Weight;
                ret =i;
                count =1;
            }
            else if (i->m_Weight == maxWeight)
                ++count;
        }
        if (count == 1)
            return *ret;
        break;
    }
    fmt::print("Action conflicts at state {}:\n", state);
    for (const auto &i: actions)
        fmt::print("'{}' ", expand(i.m_Expr,m_context));
    fmt::print("\n");
    if (ret == actions.end())
        ret =actions.begin();
    return *ret;
}

FC_GetLimits::FC_GetLimits():
    m_InputMin(std::numeric_limits<bux::T_LexID>::max()),
    m_InputMax(std::numeric_limits<bux::T_LexID>::min()),
    m_StateMin(std::numeric_limits<int>::max()),
    m_StateMax(std::numeric_limits<int>::min())
{
}

void FC_GetLimits::operator()(int state, const C_LexSet &inputs, int)
{
    if (state < m_StateMin)
        m_StateMin =state;
    if (state > m_StateMax)
        m_StateMax =state;

    for (auto i: inputs)
    {
        if (i.first < m_InputMin)
            m_InputMin =i.first;
        if (i.second > m_InputMax && i.second != std::numeric_limits<bux::T_LexID>::max())
            m_InputMax =i.second;
    }
}

std::string FC_GetLimits::inputType() const
{
    return bux::fittestType<bux::T_LexID>(m_InputMin,m_InputMax);
}

std::string FC_GetLimits::stateType() const
{
    return bux::fittestType<int>(m_StateMin,m_StateMax+1);
        // Use (m_StateMax+1) rather m_StateMax to secure the case
        // m_StateMax == std::numeric_limits<ResultingStateType>::max()
}

} // namespace

int main(int argc, const char *argv[])
{
    std::ios_base::sync_with_stdio(true);
    bux::C_EZArgs   ezargs{fmt::format(
        "Scanner Generator command line tool v{}.{}.{}\n"
        "\n"
        "  Where:\n"
        "  1. Generated C++ source files are named as:\n"
        "     <ScannerBase>.h   - Header of scanner class\n"
        "     <ScannerBase>.cpp - Implementation of scanner class\n"
        "  2. <RE1> <RE2> ... are lexical token definition files.\n"
        "  3. If no <REn> file is given, standard input is assumed.\n",
        VERSION_MAJOR, VERSION_MINOR, VERSION_RELEASE)};
    C_Paths         inc_dirs;
    ezargs.position_args({"ScannerBase", "RE1", "RE2"}, {1,2}, true)
          .add_flag("include_dir", 'I', "Search path of #include directive within <REn>",
                    [&](auto s){
                        bux::C_IMemStream in{s};
                        for (std::string line; std::getline(in, line, ':'); inc_dirs.emplace_back(line));
                    });
    auto ret = ezargs.parse(argc, argv);
    if (!ret)
    {
        fmt::print("{}\n", ret.m_error);
        return LEVEL_PARSE_ERROR;
    }

    int state = 0;
    try
    {
        // Parse inputs
        C_ScannerParser     parser{inc_dirs};
        const auto          n_pos_args = int(ezargs.parsed_position_argc());
        if (n_pos_args > 1)
        {
            fmt::print("#pos_args = {}\n", n_pos_args);
            state = 1;
            for (int i = 2; i < n_pos_args; ++i)
            {
                state = 2;
                ::parseFile(argv[i], parser, i+1 == n_pos_args? bux::T_LexID(bux::TID_EOF): '\n');
            }
        }
        else
        {
            state = 3;
            parseFile("<CON", std::cin, parser, bux::TID_EOF);
        }

        if (!parser.accepted())
        {
            fmt::print("Not accepted on EOF\n");
            return LEVEL_NOT_ACCEPTED;
        }

        // Check if parsing is ok
        auto &c = parser.m_context;
        if (c.m_ErrorTotal[LL_ERROR] || c.m_ErrorTotal[LL_FATAL])
            return LEVEL_PARSE_ERROR;

        // Render the final DFA & output
        state = 4;
        const auto pnfa = c.finalExpr();
        state = 5;
        FC_SolveConflict    solve(c);
        C_LexDfa            dfa(pnfa? *pnfa: C_LexNfa(), solve); // Algorithm MinDFA appllied
        const fs::path      path(argv[1]);
        const std::string   base =path.stem().generic_string();
        const std::string   path_h =path.string()+".h";
        if (!bux::testWritability(path_h.c_str()))
            return LEVEL_TEST_H;
        std::ofstream out(path_h.c_str());
        if (!out.is_open())
        {
            std::cerr <<"Write error on " <<path_h <<'\n';
            return LEVEL_OPEN_H;
        }
        state = 6;
        C_Output   output(dfa, c);
        state = 7;
        output.writeHeader(out, base);
        out.close();

        state = 8;
        const auto  path_cpp = path.string()+".cpp";
        if (!bux::testWritability(path_cpp.c_str()))
            return LEVEL_TEST_CPP;

        out.open(path_cpp.c_str());
        if (!out.is_open())
        {
            std::cerr <<"Write error on " <<path_cpp <<'\n';
            return LEVEL_OPEN_CPP;
        }
        state = 9;
        output.writeCpp(out, base);
    }
    catch (const std::exception &e)
    {
        std::cerr <<'[' <<state <<"] " <<HRTN(e) <<": " <<e.what() <<'\n';
        return LEVEL_STD_EXCEPTION;
    }
    catch (...)
    {
        std::cerr <<'[' <<state <<"] Caught unknown exception\n";
        return LEVEL_UNKNOWN_EXCEPTION;
    }
    return LEVEL_OK;
}

void parseFile(const std::string &filename, C_ScannerParser &parser, bux::T_LexID endToken)
{
    std::ifstream   in(filename);
    if (!in.is_open())
    {
        parser.m_context.issueError(LL_FATAL, bux::C_SourcePos(filename,0,0),
            fmt::format("Fail to open the source file '{}'", filename));
        return;
    }
    parseFile(filename, in, parser, endToken);
}

namespace bux {
template<>
struct C_FA_Traits<C_LexSet>
{
    // Types
    typedef C_LexSet T_Inputs;

    // Class Methods
    static bool isEmptyInput(const T_Inputs &src)                       { return src.empty(); }
    static bool equalInput(const T_Inputs &a, const T_Inputs &b)        { return a == b; }
    static void inputDifference(T_Inputs &dst, const T_Inputs &src)     { dst -= src; }
    static void inputIntersection(T_Inputs &dst, const T_Inputs &src)   { dst &= src; }
    static void inputUnion(T_Inputs &dst, const T_Inputs &src)          { dst |= src; }
};
} // namespace bux
