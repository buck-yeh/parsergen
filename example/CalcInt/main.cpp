#include "Parser.h"         // C_Parser
#include "ParserIdDef.h"    // TID_LEX_Spaces
#include "Scanner.h"        // C_Scanner
//-------------------------------------------------------------
#include "bux/MemIn.h"      // bux::C_IMemStream<>
#include <iostream>         // std::cin, std::cerr
#include <sstream>          // std::ostringstream

int main()
{
    std::string line;
    while (std::getline(std::cin, line))
    {
        if (line.empty())
            // Bye!
            break;

        std::ostringstream                  err_out;
        C_Parser                            parser{err_out};
        bux::C_ScreenerNo<TID_LEX_Spaces>   screener{parser};
        C_Scanner                           scanner{screener};
        bux::C_IMemStream                   in{line};
        bux::scanFile(">", in, scanner);

        // Check if parsing is ok
        if (const auto err_str = err_out.str(); !err_str.empty())
        {
            std::cerr <<err_str <<"Fail to parse!\n";
            continue;
        }

        // Acceptance
        if (!parser.accepted())
        {
            std::cerr <<"Incomplete expression!\n";
            continue;
        }

        // Output the result
        auto ans = bux::unlex<int>(parser.getFinalLex());
        std::cerr <<"= " <<ans <<" = 0x" <<std::hex <<ans <<" = 0" <<std::oct <<ans <<std::dec <<'\n';
    }

    // Ok
    std::cerr <<"Mission complete!\n";
}
