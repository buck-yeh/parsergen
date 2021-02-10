// This file is fully generated by running parsergen v1.6.2
// with grammer definition file "grammar.txt"

#ifndef ParserH
#define ParserH

#include <bux/LR1.h>
// Grammar %HEADERS_FOR_HEADER begins
#include "BNFContext.h"     // C_BNFContext
// Grammar %HEADERS_FOR_HEADER ends

namespace Main {

class C_BNFParser: public bux::LR1::C_Parser
{
public:

    // Data
    C_BNFContext            m_context;

    // Ctor
    template<class...T_Args>
    C_BNFParser(T_Args&&...args): bux::LR1::C_Parser(policy()), m_context(std::forward<T_Args>(args)...) {}

private:

    static const bux::LR1::I_ParserPolicy &policy();
};

} // namespace Main

#endif // ParserH
