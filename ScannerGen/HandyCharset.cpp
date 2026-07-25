#include "spec2charset.h"   // spec2charset()
//--------------------------------------------------------------------
#include "bux/LexBase.h"    // bux::asciiLiteral()
#include <print>            // std::println()

enum
{
    MAIN_OK,
    MAIN_HELP
};

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::println("handycharset <CharsetSpec1> <CharsetSpec2> ...");
        return MAIN_HELP;
    }
    for (int i = 1; i < argc; ++i)
    {
        std::println("[{}] ->", argv[i]);
        if (auto ret = spec2charset(argv[i]))
        {
            bool first = true;
            for (auto& j: *ret)
            {
                if (first)
                    first = false;
                else
                    std::print(", ");

                std::print("[\\u{:x},\\u{:x}]", j.first, j.second);
            }
            std::println();
        }
        else
            // On error
        {
            const auto errPos = ret.error();
            std::print("#{}: ", 1+errPos.first);
            switch (errPos.second)
            {
            case SCE_INVALID_CHAR:          std::println("Invalid char");           break;
            case SCE_MSSING_UB:             std::println("Missing range UB");       break;
            case SCE_NOT_IN_SAME_GROUP:     std::println("Not in same group");      break;
            case SCE_LB_GREATOR_THAN_UB:    std::println("Range LB < UB");          break;
            case SCE_UCODE_SYNTAX:          std::println("Unicode escape syntax");  break;
            default: std::println("Unknown error code {}", int(errPos.second));
            }
        }
    }
    return MAIN_OK;
}
