// This file is fully generated by running scannergen v1.5.0

// User-defined %HEADERS_FOR_CPP begins
#include "ParserIdDef.h"
// User-defined %HEADERS_FOR_CPP ends

#include "Scanner.h"

namespace {

// ASCII TABLE:
//  0: \x00\x01\x02\x03\x04\x05\x06\a\b\t\n\v\f\r\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f
// 20:  !\"#$%&\'()*+,-./0123456789:;<=>?
// 40: @ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_
// 60: `abcdefghijklmnopqrstuvwxyz{|}~\x7f
constinit const C_Scanner::C_GotoPair
GOTO_STATE1[6] ={
    {'\t', 2},
    {'\n', std::numeric_limits<int8_t>::max()},
    {'\v', 2},
    {'\x0e', std::numeric_limits<int8_t>::max()},
    {' ', 2},
    {'!', std::numeric_limits<int8_t>::max()}},
GOTO_STATE2[9] ={
    {'\t', 2},
    {'\n', std::numeric_limits<int8_t>::max()},
    {'\v', 2},
    {'\x0e', std::numeric_limits<int8_t>::max()},
    {' ', 2},
    {'!', std::numeric_limits<int8_t>::max()},
    {'0', 1},
    {'1', 5},
    {':', std::numeric_limits<int8_t>::max()}},
GOTO_STATE3[2] ={
    {'0', 3},
    {'8', std::numeric_limits<int8_t>::max()}},
GOTO_STATE4[6] ={
    {'0', 3},
    {'8', std::numeric_limits<int8_t>::max()},
    {'X', 6},
    {'Y', std::numeric_limits<int8_t>::max()},
    {'x', 6},
    {'y', std::numeric_limits<int8_t>::max()}},
GOTO_STATE5[6] ={
    {'0', 4},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 4},
    {'G', std::numeric_limits<int8_t>::max()},
    {'a', 4},
    {'g', std::numeric_limits<int8_t>::max()}},
GOTO_STATE6[2] ={
    {'0', 5},
    {':', std::numeric_limits<int8_t>::max()}};

constinit const C_Scanner::C_StateRec STATES[7] = {
    { GOTO_STATE2,	nullptr	},  // 0
    { GOTO_STATE4,	bux::createDecNum<TID_LEX_Num,bux::C_LexUTF32>	},  // 1
    { GOTO_STATE1,	bux::createNothing<TID_LEX_Spaces,bux::C_LexUTF32>	},  // 2
    { GOTO_STATE3,	bux::createOctNum<TID_LEX_Num,bux::C_LexUTF32>	},  // 3
    { GOTO_STATE5,	bux::createHexNum<TID_LEX_Num,bux::C_LexUTF32>	},  // 4
    { GOTO_STATE6,	bux::createDecNum<TID_LEX_Num,bux::C_LexUTF32>	},  // 5
    { GOTO_STATE5,	nullptr	}}; // 6

constinit const uint8_t GOTO_SIZE[7] = {
    9,	// 0
    6,	// 1
    6,	// 2
    2,	// 3
    6,	// 4
    2,	// 5
    6};	// 6

} // namespace

C_Scanner::C_Scanner(bux::I_Parser &parser):
    bux::C_ScannerImpl<uint8_t,int8_t,bux::C_LexUTF32>(parser)
{
    stateTables(STATES, GOTO_SIZE);
}
