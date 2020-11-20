#include <fstream>              // std::ifstream
#include "Parser.h"             // Main::C_BNFParser
#include "Scanner.h"            // C_ParserScanner

void parseFile(const std::string &filename, Main::C_BNFParser &parser, bux::T_LexID endToken)
{
    std::ifstream   in(filename);
    if (!in.is_open())
    {
        parser.m_context.issueError(LL_FATAL, bux::C_SourcePos(filename,0,0),
            "Fail to open the source file");
        return;
    }

    bux::C_ScreenerNo<Main::TID_LEX_Spaces> preparser(parser);
    C_ParserScanner scanner(preparser);
    const auto curFile = parser.setSource(filename);
    bux::scanFile(filename, in, scanner, endToken);
    parser.setSource(curFile);
}
