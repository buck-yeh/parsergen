/*! \file
    ALGORITHM SOURCES:
     -# Rienhard Wilhelm and Dieter Maurer(1995). Compiler Design. Addison-Wesley.
        - Definitions for eps() and e-ffi() (p319), Sec 8.3
            FC_CreateClosure::FC_CreateClosure(const C_ParserInfo &parsed, std::ostream &log);
        - Figure 8.22: Algorithm LR(k)-PARSER,  Sec 8.4
        - Figure 8.23: Algorithm LR(1)-Table,   Sec 8.4
        - Figure 8.24: Algorithm LR(1)-GEN,     Sec 8.4
            void FC_CreateClosure::operator()(C_StateItems &dst) const;
        - 8.5 Bison, an LALR(1)-parser generator

     -# Alfred V. Aho, Ravi Sethi, and Jeffrey D. Ullman(1985). Compilers -
        Priciples, Techniques, and Tools. Addison-Wesley.
        - Conflicts During Shift-Reduce Parsing(p201), Sec 4.5
        - 4.6 OPERATOR-RECEDENCE PARSING (p203)
        - Compaction of LR Parsing Tables(p244), Sec 4.7
        - Ambiguities from Special-Case Productions(p251), Sec 4.8
        - Using Yacc with Ambiguous Grammars(p261), 4.9
        - Error Recovery in Yacc(p264), 4.9
*/

#include "ActionMap.h"      // makeActionMap()
#include "Cfa.h"            // makeCfa()
#include "Output.h"         // FC_Output
#include "ParseFile.h"      // parseFile()
#include "Parser.h"         // Main::C_BNFParserPolicy, Main::C_BNFParser
//--------------------------------------------------------------------------------------------------
#include "bux/EZArgs.h"     // bux::C_EZArgs
#include "bux/LogStream.h"  // HRTN()
#include "bux/MemIn.h"      // bux::C_IMemStream<>
#include "bux/XConsole.h"   // bux::pressAKey();
#include "bux/XException.h" // bux::catchSE()
#include <fmt/core.h>       // fmt::print()
#include <chrono>           // std::chrono::system_clock::*

using namespace bux;

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
    MAIN_SUCCESS            = 0,
    MAIN_ARG_ERROR,
    MAIN_NOT_ACCEPTED,
    MAIN_PARSE_ERROR,
    MAIN_MAKECFA_ERROR,
    MAIN_OUTPUT_ERROR,
    MAIN_CAUGHT
};

int main(int argc, const char* argv[])
{
    bux::C_EZArgs   ezargs{fmt::format(
        "LR(1)/GLR-Parser Generator command line tool v{}.{}.{}\n"
        "\n"
        "  Where:\n"
        "  1. <Grammar> refers to a grammar definition file.\n"
        "  2. The resulting C++ source files are named as <Filename>IdDef.h <Filename>.h\n"
        "     and <Filename>.cpp\n"
        "  3. Token definitions for compound literals are written to <TokensOutput>.\n"
        , VERSION_MAJOR, VERSION_MINOR, VERSION_RELEASE)};
    C_Paths         inc_dirs;
    ezargs.position_args({"Grammar","Filename","TokensOutput"})
          .add_flag("include_dir", 'I', "Search path of #include derivatives within tokens.txt",
                    [&](auto s){
                        bux::C_IMemStream in{s};
                        for (std::string line; std::getline(in, line, ':'); inc_dirs.emplace_back(line));
                    });
    auto ret = ezargs.parse(argc, argv);
    if (!ret)
    {
        fmt::print("{}\n", ret.m_error);
        return MAIN_ARG_ERROR;
    }
    if (!inc_dirs.empty())
    {
        fmt::print("INCLUDE_PATHS: {{");
        bool first = true;
        for (auto &i: inc_dirs)
        {
            if (first)
                first = false;
            else
                fmt::print(", ");

            fmt::print("{}", i.string());
        }
        fmt::print("}}\n");
    }
    try
    {
        // Syntax analysis
        std::ios_base::sync_with_stdio(true);

        const auto          startTime = std::chrono::system_clock::now();
        Main::C_BNFParser   parser{inc_dirs};
        parseFile(argv[1], parser, TID_EOF);
        if (!parser.accepted())
        {
            fmt::print("Not accepted on EOF\n");
            return MAIN_NOT_ACCEPTED;
        }
        parser.m_context.wrapup(C_SourcePos("[wrapup]",0,0));

        // Check if parsing is ok
        const auto &c = parser.m_context;
        if (c.m_ErrorTotal[LL_ERROR] || c.m_ErrorTotal[LL_FATAL])
            return MAIN_PARSE_ERROR;

        // Generate states & goto table
        C_States            states;
        C_GotoMap           stateMap(c);
        if (const auto n = makeCfa(c, states, stateMap))
            // Errors happened
        {
            fmt::print("\n"
                       "There are {} errors in call to makeCfa()", n);
            return MAIN_MAKECFA_ERROR;
        }

        // Generate action table
        C_ActionShifts loserShits;
        auto actionMap = makeActionMap(c, states, loserShits);
        fmt::print("\n"
                   "Spent {}\"\n", std::chrono::duration<double>(std::chrono::system_clock::now() - startTime).count());

        // Strip redundant goto keys
        if (!loserShits.empty())
        {
            const size_t orgSize = stateMap.size();
            size_t count{};
            for (auto i: loserShits)
            {
                C_State fake;
                fake.m_id = i.first;
                auto found = stateMap.find({&fake, i.second});
                if (found != stateMap.end())
                {
                    ++count;
                    stateMap.erase(found);
                }
            }
            fmt::print("{} out of {} goto keys erased for redundancy.\n", count, orgSize);
            if (count < loserShits.size())
                fmt::print("Fail to find {} goto keys.\n", loserShits.size() - count);
        }

        // Output result
        FC_Output output{c, stateMap, actionMap, argv[1]};
        if (!output(argv[2], argv[3]))
            return MAIN_OUTPUT_ERROR;

        if (c.checkUnusedOptions() ||
            c.checkSemanticlessProductions())
        {
            fmt::print("Press a key to continue ...");
            bux::pressAKey();
            fmt::print("\n");
        }

        // Ok
        fmt::print("Parser created\n");
        return MAIN_SUCCESS;
    }
    catch (const std::exception &t)
    {
        fmt::print("{}: {} ... \n", HRTN(t), t.what());
        return MAIN_CAUGHT;
    }
    catch (...)
    {
        fmt::print("Unknown exception\n");
        return MAIN_CAUGHT;
    }
}
