#include "Parser.h"     // Main::C_BNFParser
#include "Scanner.h"    // C_ParserScanner
//--------------------------------------------------------------------
#include <fmt/core.h>   // fmt::print()
#include <fstream>      // std::ifstream

void parseFile(const std::string &filename, Main::C_BNFParser &parser, bux::T_LexID endToken)
{
    fmt::print("About to parse '{}' ...\n", filename);

    std::ifstream   in(filename);
    if (!in.is_open())
    {
        parser.m_context.log(LL_FATAL, bux::C_SourcePos(filename,0,0),
            "Fail to open the source file");
        return;
    }

    bux::C_ScreenerNo<Main::TID_LEX_Spaces> preparser(parser);
    C_ParserScanner scanner(preparser);
    const auto curFile = parser.setSource(filename);
    bux::scanFile(filename, in, scanner, endToken);
    parser.setSource(curFile);
}
