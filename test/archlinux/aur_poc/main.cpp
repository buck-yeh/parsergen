#include "Parser.h"         // C_CalcParser, errors
#include "ParserIdDef.h"    // TID_LEX_Spaces
#include "Scanner.h"        // C_CalcScanner
//-------------------------------------------------------------
#include <bux/LogStream.h>  // HRTN()
#include <bux/MemIn.h>      // bux::C_IMemStream<>
#include <iostream>         // std::cin, std::cerr

enum
{
    MAIN_SUCCESS        =0,
    MAIN_CAUGHT
};

int main()
{
    try
    {
        std::string line;
        while (std::getline(std::cin, line))
        {
            if (line.empty())
                // Bye!
                break;

            C_CalcParser                        parser;
            bux::C_ScreenerNo<TID_LEX_Spaces>   preparser(parser);
            C_CalcScanner                       scanner(preparser);
            bux::C_IMemStream<char>             in{line};
            bux::scanFile(">", in, scanner);

            // Check if parsing is ok
            if (errors)
            {
                errors =0;
                std::cerr <<"Fail to parse!\n";
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
        return MAIN_SUCCESS;
    }
    catch (const std::exception &t)
    {
        std::cerr <<HRTN(t) <<": " <<t.what() <<" ... \r\n";
        return MAIN_CAUGHT;
    }
    catch (...)
    {
        std::cerr <<"Unknown exception\r\n";
        return MAIN_CAUGHT;
    }
}
