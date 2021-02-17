// This file is fully generated by running parsergen v1.6.3
// with grammer definition file "grammar.txt"

#ifndef ParserH
#define ParserH

#include <bux/LR1.h>
// Grammar %HEADERS_FOR_HEADER begins
#include "Context.h"    // C_Context, C_StrList, C_WeightedStrList
// Grammar %HEADERS_FOR_HEADER ends

class C_ScannerParser: public bux::LR1::C_Parser
{
public:

    // Data
    C_Context               m_context;

    // Ctor
    template<class...T_Args>
    C_ScannerParser(T_Args&&...args): bux::LR1::C_Parser(policy()), m_context(std::forward<T_Args>(args)...) {}

private:

    static const bux::LR1::I_ParserPolicy &policy();
};

#endif // ParserH
