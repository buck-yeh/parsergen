// This file is fully generated by running parsergen
// with grammer definition file grammar.txt
// Read ParserGen.chm for further info.

#ifndef ParserH
#define ParserH

#include "LR1.h"

// %HEADERS_FOR_HEADER expanded BEGIN
#include "Context.h"    // C_Context, C_StrList, C_WeightedStrList
// %HEADERS_FOR_HEADER expanded END

class C_ScannerParser: public bux::LR1::C_Parser
{
public:

    // Data
    C_Context               m_context;

    // Ctor
    template<class...T_Args>
    C_ScannerParser(T_Args&&...args):
        bux::LR1::C_Parser{C_ScannerParser::policy()}, m_context{std::forward<T_Args>(args)...} {}

private:

    // Statics
    static const bux::LR1::I_ParserPolicy &policy();
};

#endif // ParserH