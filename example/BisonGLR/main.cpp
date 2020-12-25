#include "AST.h"            // All lex types for this parser
#include "Parser.h"         // C_GLRPOC
#include "ParserIdDef.h"    // TID_LEX_Spaces
#include "Scanner.h"        // C_GLRScanner
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
        while (std::cerr <<"Q: ", std::getline(std::cin, line))
        {
            if (line.empty())
                // Bye!
                break;

            C_GLRPOC                            parser;
            bux::C_ScreenerNo<TID_LEX_Spaces>   preparser(parser);
            C_GLRScanner                        scanner(preparser);
            bux::C_IMemStream                   in{line.data(), line.size()};
            bux::scanFile(">", in, scanner);

            // Check if parsing is ok
            if (errors)
            {
                errors =0;
                std::cerr <<"Fail to parse!\n";
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
