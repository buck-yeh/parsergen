// This file is fully generated by running parsergen
// with grammer definition file "grammar.txt"

#ifndef ParserIdDef_H
#define ParserIdDef_H

#include <bux/LexBase.h>    // bux::TOKENGEN_LB

namespace Main {

// Token Id Constants
enum
{
    TID_LEX_Bracketed   = bux::TOKENGEN_LB+0,	// $Bracketed
    TID_LEX_Id          = bux::TOKENGEN_LB+1,	// $Id
    TID_LEX_Key         = bux::TOKENGEN_LB+2,	// $Key
    TID_LEX_LexSymbol   = bux::TOKENGEN_LB+3,	// $LexSymbol
    TID_LEX_Nonterminal = bux::TOKENGEN_LB+4,	// $Nonterminal
    TID_LEX_Num         = bux::TOKENGEN_LB+5,	// $Num
    TID_LEX_Operator    = bux::TOKENGEN_LB+6,	// $Operator
    TID_LEX_Spaces      = bux::TOKENGEN_LB+7,	// $Spaces
    TID_LEX_String      = bux::TOKENGEN_LB+8,	// $String
    TID_KEYWORD_class   = bux::TOKENGEN_LB+9,	// "class"
    TID_KEYWORD_else    = bux::TOKENGEN_LB+10,	// "else"
    TID_KEYWORD_endif   = bux::TOKENGEN_LB+11,	// "endif"
    TID_KEYWORD_ifdef   = bux::TOKENGEN_LB+12,	// "ifdef"
    TID_KEYWORD_ifndef  = bux::TOKENGEN_LB+13,	// "ifndef"
    TID_KEYWORD_include = bux::TOKENGEN_LB+14,	// "include"
    TID_KEYWORD_left    = bux::TOKENGEN_LB+15,	// "left"
    TID_KEYWORD_lexid   = bux::TOKENGEN_LB+16,	// "lexid"
    TID_KEYWORD_prec    = bux::TOKENGEN_LB+17,	// "prec"
    TID_KEYWORD_right   = bux::TOKENGEN_LB+18,	// "right"
    TID_LITERAL_3a3a    = bux::TOKENGEN_LB+19,	// "::"
    TID_LITERAL_3a3a3d  = bux::TOKENGEN_LB+20,	// "::="
    TID_UB_
};

} // namespace Main

#endif // ParserIdDef_H
