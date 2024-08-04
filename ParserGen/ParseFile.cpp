#include "Parser.h"     // Main::C_BNFParser
#include "Scanner.h"    // C_ParserScanner
//--------------------------------------------------------------------
#include <fstream>      // std::ifstream
#include <print>        // std::print()
#include <set>          // std::set<>

void parseFile(const std::string &filename, Main::C_BNFParser &parser, bux::T_LexID endToken)
{
    static std::set<std::string>    g_included;
    if (g_included.contains(filename))
        return;

    g_included.emplace(filename);
    std::print("About to parse '{}' ...\n", filename);

    std::ifstream   in(filename, std::ios::binary);
    if (!in.is_open())
        return parser.m_context.log(LL_FATAL, bux::C_SourcePos(filename,0,0), "Fail to open the source file");

    bux::C_ScreenerNo<Main::TID_LEX_Spaces> preparser(parser);
    C_ParserScanner scanner(preparser);
    const auto curFile = parser.setSource(filename);
    bux::scanFile(filename, in, scanner, endToken);
    parser.setSource(curFile);
}
