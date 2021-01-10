#include "Parser.h"         // C_Parser, errors
#include "Scanner.h"        // C_Scanner
//-------------------------------------------------------------
#include "bux/LogStream.h"  // HRTN()
#include "bux/MemIn.h"      // bux::C_IMemStream<>
#include <iostream>         // std::cin, std::cerr

int main()
{
    std::string line;
    while (std::getline(std::cin, line))
    {
        C_Parser            parser;
        C_Scanner           scanner{parser};
        bux::C_IMemStream   in{line};
        try
        {
            bux::scanFile(">", in, scanner);
        }
        catch (const std::exception &e)
        {
            std::cerr <<HRTN(e) <<": " <<e.what() <<"\n"
                        "Fail to parse!\n";
            continue;
        }

        // Acceptance
        if (parser.accepted())
            break;

        std::cerr <<"Incomplete expression!\n"; // Never reach here!
    }

    // Ok
    std::cerr <<"Mission complete!\n";
}
