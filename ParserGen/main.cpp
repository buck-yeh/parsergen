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
#include "bux/MemIn.h"      // bux::C_IMemStream<>
#include "bux/StrUtil.h"    // HRTN()
#include "bux/XConsole.h"   // bux::pressAKey();
#include "bux/XException.h" // bux::catchSE()
#include <chrono>           // std::chrono::system_clock::*
#include <print>            // std::print()

using namespace bux;

enum
{
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
    bux::C_EZArgs   ezargs{std::format(
        "LR(1)/GLR-Parser Generator command line tool v{}.{}.{}\n"
        "\n"
        "  Where:\n"
        "  1. <Grammar> is a grammar definition file.\n"
        "  2. Generated C++ source files are named as:\n"
        "     <Filename>IdDef.h - Lexical token enumerations\n"
        "     <Filename>.h      - Header of parser class\n"
        "     <Filename>.cpp    - Implementation of parser class\n"
        "  3. Generated token definitions are written to <TokensOutput> to feed scannergen\n"
        , VERSION_MAJOR, VERSION_MINOR, VERSION_RELEASE)};
    C_Paths         inc_dirs;
    int             flags = 0;
    ezargs.position_args(std::array{"Grammar","Filename","TokensOutput"})
          .add_flag("include-dir", 'I', "Search path of #include directive within <Grammar>",
                    [&](auto s){
                        bux::C_IMemStream in{s};
                        for (std::string line; std::getline(in, line, ':'); inc_dirs.emplace_back(line));
                    })
          .add_flag("yes-to-all", 'a', "Quietly overwrite all existing output files",
                    [&]{
                        flags |= ALWAYS_OVERWRITE;
                    })
          .add_flag("with-bom", "Prefix BOM to all output files (utf-8 encoding already)",
                    [&]{
                        flags |= WITH_BOM;
                    });
    auto ret = ezargs.parse(argc, argv);
    if (!ret)
    {
        std::print("{}\n", ret.message());
        return MAIN_ARG_ERROR;
    }
    if (!inc_dirs.empty())
    {
        std::print("INCLUDE_PATHS: {{");
        bool first = true;
        for (auto &i: inc_dirs)
        {
            if (first)
                first = false;
            else
                std::print(", ");

            std::print("{}", i.string());
        }
        std::print("}}\n");
    }
    try
    {
        // Syntax analysis
        std::ios_base::sync_with_stdio(true);

        const auto          startTime = std::chrono::system_clock::now();
        C_BNFContext        c{inc_dirs};
        Main::C_BNFParser   parser{c};
        parseFile(argv[1], parser, TID_EOF);
        if (!parser.accepted())
        {
            std::print("Not accepted on EOF\n");
            return MAIN_NOT_ACCEPTED;
        }
        c.wrapup(C_SourcePos("[wrapup]",0,0));

        // Check if parsing is ok
        if (c.m_ErrorTotal[LL_ERROR] || c.m_ErrorTotal[LL_FATAL])
            return MAIN_PARSE_ERROR;

        // Generate states & goto table
        C_States            states;
        C_GotoMap           stateMap(c);
        if (const auto n = makeCfa(c, states, stateMap))
            // Errors happened
        {
            std::print("\n"
                       "There are {} errors in call to makeCfa()", n);
            return MAIN_MAKECFA_ERROR;
        }

        // Generate action table
        C_ActionShifts loserShits;
        auto actionMap = makeActionMap(c, states, loserShits);
        std::print("\n"
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
            std::print("{} out of {} goto keys erased for redundancy.\n", count, orgSize);
            if (count < loserShits.size())
                std::print("Fail to find {} goto keys.\n", loserShits.size() - count);
        }

        // Output result
        FC_Output output{c, stateMap, actionMap, argv[1]};
        if (!output(argv[2], argv[3], flags))
            return MAIN_OUTPUT_ERROR;

        if (c.checkUnusedOptions() ||
            c.checkSemanticlessProductions())
        {
            std::print("Press a key to continue ...");
            bux::pressAKey();
            std::print("\n");
        }

        // Ok
        std::print("Parser created\n");
        return MAIN_SUCCESS;
    }
    catch (const std::exception &t)
    {
        std::print("{}: {} ... \n", HRTN(t), t.what());
        return MAIN_CAUGHT;
    }
    catch (...)
    {
        std::print("Unknown exception\n");
        return MAIN_CAUGHT;
    }
}
