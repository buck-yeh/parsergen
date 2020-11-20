#ifndef ParseFileH
#define ParseFileH

#include "Parser.h"  // Main::C_BNFParser

void parseFile(const std::string &filename, Main::C_BNFParser &parser, bux::T_LexID endToken);

#endif // ParseFileH
