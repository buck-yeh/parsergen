#include "AST.h"            // All lex types for this parser
#include "Parser.h"         // C_Parser
#include "ParserIdDef.h"    // TID_LEX_Spaces
#include "Scanner.h"        // C_Scanner
//-------------------------------------------------------------
#include "bux/MemIn.h"      // bux::C_IMemStream<>
#include "bux/StrUtil.h"    // HRTN()
#include <iostream>         // std::cin, std::cerr
#include <sstream>          // std::ostringstream

int main()
{
    std::string line;
    while (std::cerr <<"Q: ", std::getline(std::cin, line))
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
        bool accepted{};
        parser.eachAccepted([&](auto &p){
            if (!accepted)
            {
                accepted = true;
                std::cerr <<"A:\n";
            }
            auto &decl = dynamic_cast<const C_TypeDecl&>(*p);
            std::cerr <<decl.m_name <<" = ";
            if (auto idList = bux::tryUnlex<C_IdList>(decl.m_type))
            {
                const char *prepend = "(";
                for (auto &i: *idList)
                {
                    std::cerr <<prepend <<i;
                    prepend = ", ";
                }
                std::cerr <<")\n";
            }
            else if (auto expr = dynamic_cast<const C_BinaryOp*>(decl.m_type.get()))
                // (Nested) Expression
                std::cerr <<expr->toStr() <<'\n';
            else if (decl.m_type)
                std::cerr <<"Unknown type: " <<HRTN(*decl.m_type) <<'\n';
            else
                std::cerr <<"nullptr\n";
        });
        if (!accepted)
            std::cerr <<"Incomplete expression!\n";
   }

    // Ok
    std::cerr <<"Mission complete!\n";
}

std::string toStr(const bux::GLR::C_LexPtr &term)
{
    if (auto expr = dynamic_cast<const C_BinaryOp*>(term.get()))
        return expr->toStr();
    if (auto id = bux::tryUnlex<std::string>(term))
        return *id;
    if (auto paren = bux::tryUnlex<bux::GLR::C_LexPtr>(term))
        return '(' + toStr(*paren) + ')';

    return "Unknown " + HRTN(*term);
}

std::string C_BinaryOp::toStr() const
{
    return '{' + ::toStr(m_lexpr) + m_op + ::toStr(m_rexpr) + '}';
}
