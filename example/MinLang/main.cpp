#include "Parser.h"         // C_MinParser, errors
#include "Scanner.h"        // C_MinScanner
//-------------------------------------------------------------
#include "bux/LogStream.h"  // HRTN()
#include "bux/MemIn.h"      // bux::C_IMemStream<>
#include <iostream>         // std::cin, std::cerr

enum
{
    MAIN_SUCCESS        =0,
    MAIN_CAUGHT
};

int main(int argc, char* argv[])
{
    try
    {
        std::string line;
        while (std::getline(std::cin, line))
        {
            C_MinParser         parser;
            C_MinScanner        scanner(parser);
            bux::C_IMemStream   in{line.data(), line.size()};
            bux::scanFile(">", in, scanner);

            // Acceptance
            if (!parser.accepted())
            {
                std::cerr <<"Incomplete expression!\n";
                continue;
            }
        }

        // Ok
        std::cerr <<"Mission complete!\n";
        return MAIN_SUCCESS;
    }
    catch (const std::exception &e)
    {
        std::cerr <<HRTN(e) <<": " <<e.what() <<" ... \r\n";
        return MAIN_CAUGHT;
    }
    catch (...)
    {
        std::cerr <<"Unknown exception\r\n";
        return MAIN_CAUGHT;
    }
}
