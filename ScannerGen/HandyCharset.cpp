#include "spec2charset.h"   // spec2charset()
#include "LexBase.h"        // bux::asciiLiteral()
#include <iostream>         // std::cout

enum
{
    MAIN_OK,
    MAIN_HELP
};

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout <<"handycharset <CharsetSpec1> <CharsetSpec2> ...\n";
        return MAIN_HELP;
    }
    for (int i = 1; i < argc; ++i)
    {
        std::cout <<'[' <<argv[i] <<"] ->\n";
        std::string s;
        if (auto errPos = spec2charset(argv[i], s))
            // On error
        {
            std::cout <<'#' <<(1+errPos->first) <<": ";
            switch (errPos->second)
            {
            case SCE_INVALID_CHAR:          std::cout <<"Invalid char\n";       break;
            case SCE_MSSING_UB:             std::cout <<"Missing range UB\n";   break;
            case SCE_NOT_IN_SAME_GROUP:     std::cout <<"Not in same group\n";  break;
            case SCE_LB_GREATOR_THAN_UB:    std::cout <<"Range LB < UB\n";      break;
            }
        }
        else
            // On success
            std::cout <<bux::asciiLiteral(s) <<'\n';
    }
    return MAIN_OK;
}
