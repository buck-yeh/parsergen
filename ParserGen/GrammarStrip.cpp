#include "ParseFile.h"      // parseFile()
#include "Parser.h"         // Main::C_BNFParserPolicy, Main::C_BNFParser
//------------------------------------------------------------------------------
#include "bux/EZArgs.h"     // bux::C_EZArgs
#include "bux/LogStream.h"  // HRTN()
#include "bux/MemIn.h"      // bux::C_IMemStream<>
#include "bux/XException.h" // bux::catchSE()
#include <fmt/core.h>       // fmt::print()

#undef PROGNAME
#ifdef _WIN32
#define PROGNAME "grammarstrip.exe"
#else
#define PROGNAME "grammarstrip"
#endif

using namespace bux;

enum
{
    MAIN_SUCCESS        =0,
    MAIN_ARG_ERROR,
    MAIN_NOT_ACCEPTED,
    MAIN_PARSE_ERROR,
    MAIN_CAUGHT
};

struct C_LexAssoc
{
    std::string     m_Lex;
    std::string     m_Assoc;
};
typedef std::multimap<size_t,C_LexAssoc> C_Weight2Lex;

int main(int argc, const char* argv[])
{
    bux::C_EZArgs   ezargs{
        "\t1. <Grammar> is a grammar definition file.\n"
        "\t2. The stripped grammar items will be printed to standard output.\n"};
    C_Paths         inc_dirs;
    ezargs.position_args({"Grammar"})
          .add_flag("include_dir", 'I', "Search path of #include derivatives within tokens.txt",
                    [&](auto s){
                        bux::C_IMemStream in{s};
                        for (std::string line; std::getline(in, line, ':'); inc_dirs.emplace_back(line));
                    });
    auto ret = ezargs.parse(argc, argv);
    if (!ret)
    {
        fmt::print("{}", ret.m_error);
        return MAIN_ARG_ERROR;
    }

    try
    {
        // Syntax analysis
        std::ios_base::sync_with_stdio(true);

        Main::C_BNFParser   parser{inc_dirs};
        parseFile(argv[1], parser, TID_EOF);
        if (!parser.accepted())
        {
            fmt::print("Not accepted on EOF\n");
            return MAIN_NOT_ACCEPTED;
        }
        parser.m_context.wrapup(C_SourcePos("[wrapup]",0,0));

        // Check if parsing is ok
        const auto &c =parser.m_context;
        if (c.m_ErrorTotal[LL_ERROR] || c.m_ErrorTotal[LL_FATAL])
            return MAIN_PARSE_ERROR;

        // Outputs
        if (!c.priorityMap().empty())
        {
            C_Weight2Lex w2l;
            for (auto& i: c.priorityMap())
            {
                C_LexAssoc t;
                for (auto j: c.lex2ID())
                    if (j.second == i.first)
                    {
                        t.m_Lex = '$' + j.first;
                        goto SetAssoc;
                    }
                for (auto &j: c.generatedLexes())
                    if (j.second == i.first)
                    {
                        t.m_Lex = '\"' + bux::asciiLiteral(j.first) + '\"';
                        goto SetAssoc;
                    }
                t.m_Lex = {'\"', char(i.first), '\"'};
            SetAssoc:
                switch (i.second.m_Assoc)
                {
                case ASSOC_NONE:
                    t.m_Assoc ="assoc";
                    break;
                case LEFT2RIGHT:
                    t.m_Assoc ="left";
                    break;
                case RIGHT2LEFT:
                    t.m_Assoc ="right";
                    break;
                }
                w2l.emplace(i.second.m_Weight, t);
            }

            for (auto &i: w2l)
                fmt::print("{}\t{}\t{}\n", i.second.m_Lex, i.second.m_Assoc, i.first);

            fmt::print("\n");
        }
        for (auto &i: c.productions())
            fmt::print("{}\n", i.str());

        return MAIN_SUCCESS;
    }
    catch (const std::exception &t)
    {
        fmt::print("{}: {} ... \n", HRTN(t), t.what());
        return MAIN_CAUGHT;
    }
    catch (...)
    {
        fmt::print("Unknown exception\r\n");
        return MAIN_CAUGHT;
    }
}
