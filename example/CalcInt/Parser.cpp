// This file is fully generated by running parsergen
// with grammer definition file "grammar.txt"

// %HEADERS_FOR_CPP expanded BEGIN
#include <iostream>     // std::cerr
// %HEADERS_FOR_CPP expanded END
#include "Parser.h"
#include "ParserIdDef.h"
#include <bux/ImplLR1.h>
#include <bux/XException.h>

namespace {

using namespace bux::LR1;

//
//   Generated Constants
//
enum
{
    NID_Expr = TID_UB_+2,	// <Expr>
    TOKENGEN_UB,
    //---------------------------------------------------
    ENCODED_TOKEN_LB	 = 48
};
#define ZIP_TOKEN(x) (x-(bux::MIN_TOKEN_ID-ENCODED_TOKEN_LB))

constinit const uint8_t
alphaPrime0[6] = {
    '%',
    ')',
    '*',
    '+',
    '-',
    '/'},
alphaPrime1[6] = {
    '%',
    '*',
    '+',
    '-',
    '/',
    ZIP_TOKEN(bux::TID_EOF)},
alphaPrime2[3] = {
    '(',
    ZIP_TOKEN(TID_LEX_Num),
    ZIP_TOKEN(NID_Expr)};

int findKey(const uint8_t *klist, int n, uint8_t key)
{
    for (int i = 0; i < n; ++i)
         if (key == klist[i])
             return i;
    return -1;
}

int findPrime0_0_6	(uint8_t key)	{ return findKey(alphaPrime0,6,key); }
int findPrime1_0_5	(uint8_t key)	{ return findKey(alphaPrime1,5,key); }
int findPrime1_0_6	(uint8_t key)	{ return findKey(alphaPrime1,6,key); }
int findPrime2_0_2	(uint8_t key)	{ return findKey(alphaPrime2,2,key); }
int findPrime2_0_3	(uint8_t key)	{ return findKey(alphaPrime2,3,key); }

constinit const bux::C_KVPair<uint8_t,uint8_t>
L2A_Table0[] ={
    {'%',	ACTION_SHIFT},
    {'*',	ACTION_SHIFT},
    {'+',	ACTION_REDUCE_MIN + 1},
    {'-',	ACTION_REDUCE_MIN + 1},
    {'/',	ACTION_SHIFT},
    {ZIP_TOKEN(bux::TID_EOF),	ACTION_REDUCE_MIN + 1}},
L2A_Table1[] ={
    {'%',	ACTION_SHIFT},
    {'*',	ACTION_SHIFT},
    {'+',	ACTION_REDUCE_MIN + 2},
    {'-',	ACTION_REDUCE_MIN + 2},
    {'/',	ACTION_SHIFT},
    {ZIP_TOKEN(bux::TID_EOF),	ACTION_REDUCE_MIN + 2}},
L2A_Table2[] ={
    {'%',	ACTION_SHIFT},
    {'*',	ACTION_SHIFT},
    {'+',	ACTION_SHIFT},
    {'-',	ACTION_SHIFT},
    {'/',	ACTION_SHIFT},
    {ZIP_TOKEN(bux::TID_EOF),	ACTION_ACCEPT}},
L2A_Table3[] ={
    {'%',	ACTION_SHIFT},
    {')',	ACTION_REDUCE_MIN + 1},
    {'*',	ACTION_SHIFT},
    {'+',	ACTION_REDUCE_MIN + 1},
    {'-',	ACTION_REDUCE_MIN + 1},
    {'/',	ACTION_SHIFT}},
L2A_Table4[] ={
    {'%',	ACTION_SHIFT},
    {')',	ACTION_REDUCE_MIN + 2},
    {'*',	ACTION_SHIFT},
    {'+',	ACTION_REDUCE_MIN + 2},
    {'-',	ACTION_REDUCE_MIN + 2},
    {'/',	ACTION_SHIFT}};

constinit const bux::U_K2V<uint8_t,uint8_t> mapActionPtr[30] ={
    findPrime2_0_2,	// 0
    findPrime2_0_2,	// 1
    findPrime1_0_6,	// 2
    L2A_Table2,	// 3
    findPrime2_0_2,	// 4
    findPrime0_0_6,	// 5
    findPrime0_0_6,	// 6
    findPrime2_0_2,	// 7
    findPrime2_0_2,	// 8
    findPrime2_0_2,	// 9
    findPrime2_0_2,	// 10
    findPrime2_0_2,	// 11
    findPrime0_0_6,	// 12
    findPrime2_0_2,	// 13
    findPrime1_0_6,	// 14
    findPrime2_0_2,	// 15
    findPrime2_0_2,	// 16
    findPrime2_0_2,	// 17
    findPrime2_0_2,	// 18
    findPrime1_0_6,	// 19
    findPrime1_0_6,	// 20
    L2A_Table0,	// 21
    L2A_Table1,	// 22
    findPrime1_0_6,	// 23
    findPrime0_0_6,	// 24
    findPrime0_0_6,	// 25
    findPrime0_0_6,	// 26
    L2A_Table3,	// 27
    L2A_Table4,	// 28
    findPrime0_0_6};	// 29

constinit const int8_t mapActionSizeEx[30] ={
    ACTION_SHIFT,	// 0
    ACTION_SHIFT,	// 1
    ACTION_REDUCE_MIN + 7,	// 2
    -6,	// 3
    ACTION_SHIFT,	// 4
    ACTION_REDUCE_MIN + 7,	// 5
    ACTION_SHIFT,	// 6
    ACTION_SHIFT,	// 7
    ACTION_SHIFT,	// 8
    ACTION_SHIFT,	// 9
    ACTION_SHIFT,	// 10
    ACTION_SHIFT,	// 11
    ACTION_SHIFT,	// 12
    ACTION_SHIFT,	// 13
    ACTION_REDUCE_MIN + 6,	// 14
    ACTION_SHIFT,	// 15
    ACTION_SHIFT,	// 16
    ACTION_SHIFT,	// 17
    ACTION_SHIFT,	// 18
    ACTION_REDUCE_MIN + 5,	// 19
    ACTION_REDUCE_MIN + 3,	// 20
    -6,	// 21
    -6,	// 22
    ACTION_REDUCE_MIN + 4,	// 23
    ACTION_REDUCE_MIN + 6,	// 24
    ACTION_REDUCE_MIN + 5,	// 25
    ACTION_REDUCE_MIN + 3,	// 26
    -6,	// 27
    -6,	// 28
    ACTION_REDUCE_MIN + 4};	// 29

constinit const bux::C_KVPair<uint8_t,uint8_t>
L2S_Table0[] ={
    {'%',	7},
    {'*',	8},
    {'/',	11}},
L2S_Table1[] ={
    {'%',	13},
    {')',	24},
    {'*',	15},
    {'+',	16},
    {'-',	17},
    {'/',	18}},
L2S_Table2[] ={
    {'%',	13},
    {'*',	15},
    {'/',	18}},
L2S_Table3[] ={
    {'(',	1},
    {ZIP_TOKEN(TID_LEX_Num),	2},
    {ZIP_TOKEN(NID_Expr),	19}},
L2S_Table4[] ={
    {'(',	1},
    {ZIP_TOKEN(TID_LEX_Num),	2},
    {ZIP_TOKEN(NID_Expr),	20}},
L2S_Table5[] ={
    {'(',	1},
    {ZIP_TOKEN(TID_LEX_Num),	2},
    {ZIP_TOKEN(NID_Expr),	21}},
L2S_Table6[] ={
    {'(',	1},
    {ZIP_TOKEN(TID_LEX_Num),	2},
    {ZIP_TOKEN(NID_Expr),	22}},
L2S_Table7[] ={
    {'(',	1},
    {ZIP_TOKEN(TID_LEX_Num),	2},
    {ZIP_TOKEN(NID_Expr),	23}},
L2S_Table8[] ={
    {'(',	4},
    {ZIP_TOKEN(TID_LEX_Num),	5},
    {ZIP_TOKEN(NID_Expr),	12}},
L2S_Table9[] ={
    {'(',	4},
    {ZIP_TOKEN(TID_LEX_Num),	5},
    {ZIP_TOKEN(NID_Expr),	25}},
L2S_Table10[] ={
    {'(',	4},
    {ZIP_TOKEN(TID_LEX_Num),	5},
    {ZIP_TOKEN(NID_Expr),	26}},
L2S_Table11[] ={
    {'(',	4},
    {ZIP_TOKEN(TID_LEX_Num),	5},
    {ZIP_TOKEN(NID_Expr),	27}},
L2S_Table12[] ={
    {'(',	4},
    {ZIP_TOKEN(TID_LEX_Num),	5},
    {ZIP_TOKEN(NID_Expr),	28}},
L2S_Table13[] ={
    {'(',	4},
    {ZIP_TOKEN(TID_LEX_Num),	5},
    {ZIP_TOKEN(NID_Expr),	29}};

constinit const struct C_MapGoto {
    bux::U_K2V<uint8_t,uint8_t> m_k2v;
    uint8_t	m_curState;
    int8_t	m_nextStateEx;
}   mapGoto[20] ={
    {findPrime2_0_3,	0,	1},
    {findPrime2_0_3,	1,	4},
    {findPrime1_0_5,	3,	7},
    {L2S_Table8,	4,	-3},
    {findPrime0_0_6,	6,	13},
    {L2S_Table3,	7,	-3},
    {L2S_Table4,	8,	-3},
    {L2S_Table5,	9,	-3},
    {L2S_Table6,	10,	-3},
    {L2S_Table7,	11,	-3},
    {L2S_Table1,	12,	-6},
    {L2S_Table9,	13,	-3},
    {L2S_Table10,	15,	-3},
    {L2S_Table11,	16,	-3},
    {L2S_Table12,	17,	-3},
    {L2S_Table13,	18,	-3},
    {L2S_Table0,	21,	-3},
    {L2S_Table0,	22,	-3},
    {L2S_Table2,	27,	-3},
    {L2S_Table2,	28,	-3}};

void _reduce_0(bux::LR1::C_Parser &, const F_GetProduced &_geT_, C_RetLval _reT_)
//  <@> ::= <Expr>
{
    _reT_ = _geT_(0);
}

void _reduce_1(bux::LR1::C_Parser &, const F_GetProduced &_geT_, C_RetLval _reT_)
//  <Expr> ::= <Expr> + <Expr>
{
    bux::unlex<int>(_geT_(0)) += bux::unlex<int>(_geT_(2));
    _reT_ = _geT_(0);
}

void _reduce_2(bux::LR1::C_Parser &, const F_GetProduced &_geT_, C_RetLval _reT_)
//  <Expr> ::= <Expr> - <Expr>
{
    bux::unlex<int>(_geT_(0)) -= bux::unlex<int>(_geT_(2));
    _reT_ = _geT_(0);
}

void _reduce_3(bux::LR1::C_Parser &, const F_GetProduced &_geT_, C_RetLval _reT_)
//  <Expr> ::= <Expr> * <Expr>
{
    bux::unlex<int>(_geT_(0)) *= bux::unlex<int>(_geT_(2));
    _reT_ = _geT_(0);
}

void _reduce_4(bux::LR1::C_Parser &, const F_GetProduced &_geT_, C_RetLval _reT_)
//  <Expr> ::= <Expr> / <Expr>
{
    bux::unlex<int>(_geT_(0)) /= bux::unlex<int>(_geT_(2));
    _reT_ = _geT_(0);
}

void _reduce_5(bux::LR1::C_Parser &, const F_GetProduced &_geT_, C_RetLval _reT_)
//  <Expr> ::= <Expr> % <Expr>
{
    bux::unlex<int>(_geT_(0)) %= bux::unlex<int>(_geT_(2));
    _reT_ = _geT_(0);
}

void _reduce_6(bux::LR1::C_Parser &, const F_GetProduced &_geT_, C_RetLval _reT_)
//  <Expr> ::= ( <Expr> )
{
    _reT_ = _geT_(1);
}

void _reduce_7(bux::LR1::C_Parser &, const F_GetProduced &_geT_, C_RetLval _reT_)
//  <Expr> ::= $Num
{
    _reT_ = bux::createLex(dynamic_cast<bux::C_IntegerLex&>(*_geT_(0)).value<int>());
}

struct C_ProductionInfo
{
    void            (*m_pAction)(bux::LR1::C_Parser&,const F_GetProduced&,C_RetLval);
    uint8_t         m_ReducedID;
    uint8_t         m_PopLength;
} constinit const prodInfo[8] ={
{_reduce_0, 	ZIP_TOKEN(bux::ROOT_NID), 1},	// 0: <@> ::= <Expr>
{_reduce_1, 	ZIP_TOKEN(NID_Expr), 3},	// 1: <Expr> ::= <Expr> + <Expr>
{_reduce_2, 	ZIP_TOKEN(NID_Expr), 3},	// 2: <Expr> ::= <Expr> - <Expr>
{_reduce_3, 	ZIP_TOKEN(NID_Expr), 3},	// 3: <Expr> ::= <Expr> * <Expr>
{_reduce_4, 	ZIP_TOKEN(NID_Expr), 3},	// 4: <Expr> ::= <Expr> / <Expr>
{_reduce_5, 	ZIP_TOKEN(NID_Expr), 3},	// 5: <Expr> ::= <Expr> % <Expr>
{_reduce_6, 	ZIP_TOKEN(NID_Expr), 3},	// 6: <Expr> ::= ( <Expr> )
{_reduce_7, 	ZIP_TOKEN(NID_Expr), 1} 	// 7: <Expr> ::= $Num
};

struct C_ActionTraits
{
    template<class T1, class T2>
    static uint8_t map(T1 &&t1, T2 &&) { return static_cast<uint8_t>(t1); }
    static uint8_t valueError() { return ACTION_ERROR; }
};

struct C_ShiftTraits
{
    template<class T1, class T2>
    static uint8_t map(T1 &&t1, T2 &&t2) { return static_cast<uint8_t>(t1+t2); }
    static uint8_t valueError() { RUNTIME_ERROR("SHIFT ERROR"); }
};

//
//   Generated Parser Policy
//
class C_ParserPolicy: public I_ParserPolicy
{
public:

    // Ctor
    C_ParserPolicy(): I_ParserPolicy(TOKENGEN_UB+2) {}

    // Implement I_ParserPolicy
    size_t action(bux::T_StateID state, bux::T_LexID token) const override;
    size_t getAcceptId() const override;
    bool getTokenName(bux::T_LexID token, std::string &name) const override;
    bux::T_StateID nextState(bux::T_StateID state, bux::T_LexID lex) const override;
    void getReduceInfo(size_t id, C_ReduceInfo &info) const override;
    void onError(bux::LR1::C_Parser &_paRSeR_, const bux::C_SourcePos &pos, const std::string &message) const override;
}   g_policy;

size_t C_ParserPolicy::action(bux::T_StateID state, bux::T_LexID input) const
{
    if (input >= bux::MIN_TOKEN_ID)
        input = ZIP_TOKEN(input);
    else if (input >= ENCODED_TOKEN_LB)
        return ACTION_ERROR;

    return index2value<uint8_t,uint8_t,int8_t,C_ActionTraits>(mapActionPtr[state], mapActionSizeEx[state], input);
}

size_t C_ParserPolicy::getAcceptId() const
{
    return 0;
}

bool C_ParserPolicy::getTokenName(bux::T_LexID token, std::string &name) const
{
    static constinit char const *const NAMES[3] = {
        "$Num",
        "$Spaces",
        "<Expr>"};
    if (bux::TOKENGEN_LB <= token && token < bux::TOKENGEN_LB + 3)
    {
        name = NAMES[token - bux::TOKENGEN_LB];
        return true;
    }
    return false;
}

bux::T_StateID C_ParserPolicy::nextState(bux::T_StateID state, bux::T_LexID input) const
{
    if (input >= bux::MIN_TOKEN_ID)
        input = ZIP_TOKEN(input);
    else if (input >= ENCODED_TOKEN_LB)
        LOGIC_ERROR("Invalid input: state={} input={}", (int)state, printToken(input));

    auto end = mapGoto + 20;
    auto found = std::lower_bound(mapGoto, end, state, [](const C_MapGoto &i, bux::T_StateID state_) {
        return i.m_curState < state_;
    });
    if (found != end && found->m_curState == state)
        return index2value<uint8_t,uint8_t,int8_t,C_ShiftTraits>(found->m_k2v, found->m_nextStateEx, input);

    RUNTIME_ERROR("Invalid state: state={} input={}", (int)state, printToken(input));
}

void C_ParserPolicy::getReduceInfo(size_t id, C_ReduceInfo &info) const
{
    const auto &src  = prodInfo[id];
    info.m_ResultID  = src.m_ReducedID + (bux::MIN_TOKEN_ID-ENCODED_TOKEN_LB);
    info.m_PopLength = src.m_PopLength;
    info.m_Reduce    = src.m_pAction;
}

void C_ParserPolicy::onError(bux::LR1::C_Parser &, const bux::C_SourcePos &pos, const std::string &message) const
{
    // User-defined %ON_ERROR begins
    std::cerr <<"COL#" <<pos.m_Col <<": " <<message <<'\n';
    if (++errors > 10)
        RUNTIME_ERROR("Too many errors !");
    // User-defined %ON_ERROR ends
}

} // namespace

// %SCOPED_CPP_HEAD expanded BEGIN
int errors =0;
// %SCOPED_CPP_HEAD expanded END

C_CalcParser::C_CalcParser(): bux::LR1::C_Parser(g_policy)
{
}
