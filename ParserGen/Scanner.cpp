// This file is fully generated by running scannergen v1.6.0

// Token-def %HEADERS_FOR_CPP begins
#include "ParserIdDef.h"
// Grammar %HEADERS_FOR_SCANNER_CPP begins
#include "BracketBalance.h"
// Grammar %HEADERS_FOR_SCANNER_CPP ends
using namespace Main;
// Token-def %HEADERS_FOR_CPP ends

#include "Scanner.h"

namespace {

// ASCII TABLE:
//  0: \x00\x01\x02\x03\x04\x05\x06\a\b\t\n\v\f\r\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f
// 20:  !\"#$%&\'()*+,-./0123456789:;<=>?
// 40: @ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_
// 60: `abcdefghijklmnopqrstuvwxyz{|}~\x7f
constinit const C_ParserScanner::C_GotoPair
GOTO_STATE1[3] ={
    {'\x00', 86},
    {'\n', 39},
    {'\v', 86}},
GOTO_STATE2[16] ={
    {'\x00', 86},
    {'\n', 39},
    {'\v', 86},
    {'!', 51},
    {'\"', 87},
    {'#', 51},
    {'\'', 87},
    {'(', 51},
    {'0', 86},
    {':', 51},
    {'A', 86},
    {'[', 51},
    {'_', 86},
    {'`', 51},
    {'a', 86},
    {'{', 51}},
GOTO_STATE3[17] ={
    {'\x00', 88},
    {'!', 53},
    {'\"', 89},
    {'#', 53},
    {'\'', 89},
    {'(', 53},
    {'*', 52},
    {'+', 53},
    {'/', 64},
    {'0', 88},
    {':', 53},
    {'A', 88},
    {'[', 53},
    {'_', 88},
    {'`', 53},
    {'a', 88},
    {'{', 53}},
GOTO_STATE4[16] ={
    {'\x00', 88},
    {'!', 53},
    {'\"', 89},
    {'#', 53},
    {'\'', 89},
    {'(', 53},
    {'*', 52},
    {'+', 53},
    {'0', 88},
    {':', 53},
    {'A', 88},
    {'[', 53},
    {'_', 88},
    {'`', 53},
    {'a', 88},
    {'{', 53}},
GOTO_STATE5[3] ={
    {'\x00', 88},
    {'*', 110},
    {'+', 88}},
GOTO_STATE6[5] ={
    {'\x00', 88},
    {'*', 110},
    {'+', 88},
    {'/', 68},
    {'0', 88}},
GOTO_STATE7[16] ={
    {'\x00', 90},
    {'!', 55},
    {'\"', 91},
    {'#', 55},
    {'\'', 91},
    {'(', 55},
    {'0', 90},
    {':', 55},
    {'A', 90},
    {'[', 55},
    {']', 54},
    {'^', 55},
    {'_', 90},
    {'`', 55},
    {'a', 90},
    {'{', 55}},
GOTO_STATE8[16] ={
    {'\x00', 90},
    {'!', 55},
    {'\"', 91},
    {'#', 55},
    {'\'', 91},
    {'(', 55},
    {'0', 90},
    {':', 55},
    {'A', 90},
    {'[', 55},
    {']', 65},
    {'^', 55},
    {'_', 90},
    {'`', 55},
    {'a', 90},
    {'{', 55}},
GOTO_STATE9[3] ={
    {'\x00', 90},
    {']', 69},
    {'^', 90}},
GOTO_STATE10[3] ={
    {'\x00', 90},
    {']', 111},
    {'^', 90}},
GOTO_STATE11[38] ={
    {'\t', 20},
    {'\n', std::numeric_limits<int8_t>::max()},
    {'\v', 20},
    {'\x0e', std::numeric_limits<int8_t>::max()},
    {' ', 20},
    {'!', 72},
    {'\"', 79},
    {'#', 72},
    {'$', 75},
    {'%', 72},
    {'\'', 78},
    {'(', 72},
    {'/', 73},
    {'0', 19},
    {'1', 18},
    {':', 77},
    {';', 72},
    {'<', 76},
    {'=', 72},
    {'A', 71},
    {'[', 74},
    {'\\', 72},
    {'_', 71},
    {'`', 72},
    {'a', 71},
    {'c', 12},
    {'d', 71},
    {'e', 13},
    {'f', 71},
    {'i', 14},
    {'j', 71},
    {'l', 15},
    {'m', 71},
    {'p', 16},
    {'q', 71},
    {'r', 17},
    {'s', 71},
    {'{', 72}},
GOTO_STATE12[6] ={
    {'\t', 20},
    {'\n', std::numeric_limits<int8_t>::max()},
    {'\v', 20},
    {'\x0e', std::numeric_limits<int8_t>::max()},
    {' ', 20},
    {'!', std::numeric_limits<int8_t>::max()}},
GOTO_STATE13[5] ={
    {' ', 82},
    {'\'', std::numeric_limits<int8_t>::max()},
    {'(', 82},
    {'\\', 83},
    {']', 82}},
GOTO_STATE14[5] ={
    {' ', 84},
    {'\"', 38},
    {'#', 84},
    {'\\', 85},
    {']', 84}},
GOTO_STATE15[16] ={
    {'!', 29},
    {'\"', 72},
    {'#', 29},
    {'\'', 72},
    {'(', 29},
    {'*', 33},
    {'+', 29},
    {'/', 32},
    {'0', std::numeric_limits<int8_t>::max()},
    {':', 29},
    {'A', std::numeric_limits<int8_t>::max()},
    {'[', 29},
    {'_', std::numeric_limits<int8_t>::max()},
    {'`', 29},
    {'a', std::numeric_limits<int8_t>::max()},
    {'{', 29}},
GOTO_STATE16[15] ={
    {'!', 29},
    {'\"', 72},
    {'#', 29},
    {'\'', 72},
    {'(', 29},
    {'0', std::numeric_limits<int8_t>::max()},
    {':', 29},
    {'=', 31},
    {'>', 29},
    {'A', std::numeric_limits<int8_t>::max()},
    {'[', 29},
    {'_', std::numeric_limits<int8_t>::max()},
    {'`', 29},
    {'a', std::numeric_limits<int8_t>::max()},
    {'{', 29}},
GOTO_STATE17[15] ={
    {'!', 29},
    {'\"', 72},
    {'#', 29},
    {'\'', 72},
    {'(', 29},
    {'0', std::numeric_limits<int8_t>::max()},
    {':', 29},
    {'>', 30},
    {'?', 29},
    {'A', 80},
    {'[', 29},
    {'_', 80},
    {'`', 29},
    {'a', 80},
    {'{', 29}},
GOTO_STATE18[14] ={
    {'!', 29},
    {'\"', 72},
    {'#', 29},
    {'\'', 72},
    {'(', 29},
    {'0', std::numeric_limits<int8_t>::max()},
    {':', 29},
    {'@', 35},
    {'A', 80},
    {'[', 29},
    {'_', 80},
    {'`', 29},
    {'a', 80},
    {'{', 29}},
GOTO_STATE19[13] ={
    {'!', 29},
    {'\"', 72},
    {'#', 29},
    {'\'', 72},
    {'(', 29},
    {'0', std::numeric_limits<int8_t>::max()},
    {':', 29},
    {'A', 1},
    {'[', 29},
    {'_', 1},
    {'`', 29},
    {'a', 1},
    {'{', 29}},
GOTO_STATE20[13] ={
    {'!', 29},
    {'\"', 72},
    {'#', 29},
    {'\'', 72},
    {'(', 29},
    {'0', std::numeric_limits<int8_t>::max()},
    {':', 29},
    {'A', std::numeric_limits<int8_t>::max()},
    {'[', 29},
    {'_', std::numeric_limits<int8_t>::max()},
    {'`', 29},
    {'a', std::numeric_limits<int8_t>::max()},
    {'{', 29}},
GOTO_STATE21[14] ={
    {'!', 29},
    {'\"', 72},
    {'#', 29},
    {'\'', 72},
    {'(', 29},
    {'0', std::numeric_limits<int8_t>::max()},
    {':', 29},
    {'A', std::numeric_limits<int8_t>::max()},
    {'[', 34},
    {'\\', 29},
    {'_', std::numeric_limits<int8_t>::max()},
    {'`', 29},
    {'a', std::numeric_limits<int8_t>::max()},
    {'{', 29}},
GOTO_STATE22[14] ={
    {'!', 29},
    {'\"', 72},
    {'#', 29},
    {'\'', 72},
    {'(', 29},
    {'0', std::numeric_limits<int8_t>::max()},
    {':', 36},
    {';', 29},
    {'A', std::numeric_limits<int8_t>::max()},
    {'[', 29},
    {'_', std::numeric_limits<int8_t>::max()},
    {'`', 29},
    {'a', std::numeric_limits<int8_t>::max()},
    {'{', 29}},
GOTO_STATE23[22] ={
    {'\"', 82},
    {'#', std::numeric_limits<int8_t>::max()},
    {'\'', 82},
    {'(', std::numeric_limits<int8_t>::max()},
    {'U', 94},
    {'V', std::numeric_limits<int8_t>::max()},
    {'\\', 82},
    {']', std::numeric_limits<int8_t>::max()},
    {'a', 82},
    {'c', std::numeric_limits<int8_t>::max()},
    {'f', 82},
    {'g', std::numeric_limits<int8_t>::max()},
    {'n', 82},
    {'o', std::numeric_limits<int8_t>::max()},
    {'r', 82},
    {'s', std::numeric_limits<int8_t>::max()},
    {'t', 82},
    {'u', 93},
    {'v', 82},
    {'w', std::numeric_limits<int8_t>::max()},
    {'x', 92},
    {'y', std::numeric_limits<int8_t>::max()}},
GOTO_STATE24[22] ={
    {'\"', 84},
    {'#', std::numeric_limits<int8_t>::max()},
    {'\'', 84},
    {'(', std::numeric_limits<int8_t>::max()},
    {'U', 95},
    {'V', std::numeric_limits<int8_t>::max()},
    {'\\', 84},
    {']', std::numeric_limits<int8_t>::max()},
    {'a', 84},
    {'c', std::numeric_limits<int8_t>::max()},
    {'f', 84},
    {'g', std::numeric_limits<int8_t>::max()},
    {'n', 84},
    {'o', std::numeric_limits<int8_t>::max()},
    {'r', 84},
    {'s', std::numeric_limits<int8_t>::max()},
    {'t', 84},
    {'u', 102},
    {'v', 84},
    {'w', std::numeric_limits<int8_t>::max()},
    {'x', 107},
    {'y', std::numeric_limits<int8_t>::max()}},
GOTO_STATE25[2] ={
    {'\'', 37},
    {'(', std::numeric_limits<int8_t>::max()}},
GOTO_STATE26[8] ={
    {'\'', 37},
    {'(', std::numeric_limits<int8_t>::max()},
    {'0', 82},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 82},
    {'G', std::numeric_limits<int8_t>::max()},
    {'a', 82},
    {'g', std::numeric_limits<int8_t>::max()}},
GOTO_STATE27[8] ={
    {'0', 1},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 1},
    {'[', std::numeric_limits<int8_t>::max()},
    {'_', 1},
    {'`', std::numeric_limits<int8_t>::max()},
    {'a', 1},
    {'{', std::numeric_limits<int8_t>::max()}},
GOTO_STATE28[2] ={
    {'0', 18},
    {':', std::numeric_limits<int8_t>::max()}},
GOTO_STATE29[6] ={
    {'0', 56},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 56},
    {'G', std::numeric_limits<int8_t>::max()},
    {'a', 56},
    {'g', std::numeric_limits<int8_t>::max()}},
GOTO_STATE30[9] ={
    {'0', 71},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 71},
    {'[', std::numeric_limits<int8_t>::max()},
    {'_', 71},
    {'`', std::numeric_limits<int8_t>::max()},
    {'a', 41},
    {'b', 71},
    {'{', std::numeric_limits<int8_t>::max()}},
GOTO_STATE31[10] ={
    {'0', 71},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 71},
    {'[', std::numeric_limits<int8_t>::max()},
    {'_', 71},
    {'`', std::numeric_limits<int8_t>::max()},
    {'a', 71},
    {'c', 10},
    {'d', 71},
    {'{', std::numeric_limits<int8_t>::max()}},
GOTO_STATE32[10] ={
    {'0', 71},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 71},
    {'[', std::numeric_limits<int8_t>::max()},
    {'_', 71},
    {'`', std::numeric_limits<int8_t>::max()},
    {'a', 71},
    {'c', 46},
    {'d', 71},
    {'{', std::numeric_limits<int8_t>::max()}},
GOTO_STATE33[10] ={
    {'0', 71},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 71},
    {'[', std::numeric_limits<int8_t>::max()},
    {'_', 71},
    {'`', std::numeric_limits<int8_t>::max()},
    {'a', 71},
    {'d', 9},
    {'e', 71},
    {'{', std::numeric_limits<int8_t>::max()}},
GOTO_STATE34[10] ={
    {'0', 71},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 71},
    {'[', std::numeric_limits<int8_t>::max()},
    {'_', 71},
    {'`', std::numeric_limits<int8_t>::max()},
    {'a', 71},
    {'d', 43},
    {'e', 71},
    {'{', std::numeric_limits<int8_t>::max()}},
GOTO_STATE35[12] ={
    {'0', 71},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 71},
    {'[', std::numeric_limits<int8_t>::max()},
    {'_', 71},
    {'`', std::numeric_limits<int8_t>::max()},
    {'a', 71},
    {'d', 44},
    {'e', 71},
    {'n', 45},
    {'o', 71},
    {'{', std::numeric_limits<int8_t>::max()}},
GOTO_STATE36[10] ={
    {'0', 71},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 71},
    {'[', std::numeric_limits<int8_t>::max()},
    {'_', 71},
    {'`', std::numeric_limits<int8_t>::max()},
    {'a', 71},
    {'d', 60},
    {'e', 71},
    {'{', std::numeric_limits<int8_t>::max()}},
GOTO_STATE37[10] ={
    {'0', 71},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 71},
    {'[', std::numeric_limits<int8_t>::max()},
    {'_', 71},
    {'`', std::numeric_limits<int8_t>::max()},
    {'a', 71},
    {'d', 70},
    {'e', 71},
    {'{', std::numeric_limits<int8_t>::max()}},
GOTO_STATE38[10] ={
    {'0', 71},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 71},
    {'[', std::numeric_limits<int8_t>::max()},
    {'_', 71},
    {'`', std::numeric_limits<int8_t>::max()},
    {'a', 71},
    {'e', 3},
    {'f', 71},
    {'{', std::numeric_limits<int8_t>::max()}},
GOTO_STATE39[10] ={
    {'0', 71},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 71},
    {'[', std::numeric_limits<int8_t>::max()},
    {'_', 71},
    {'`', std::numeric_limits<int8_t>::max()},
    {'a', 71},
    {'e', 7},
    {'f', 71},
    {'{', std::numeric_limits<int8_t>::max()}},
GOTO_STATE40[10] ={
    {'0', 71},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 71},
    {'[', std::numeric_limits<int8_t>::max()},
    {'_', 71},
    {'`', std::numeric_limits<int8_t>::max()},
    {'a', 71},
    {'e', 26},
    {'f', 71},
    {'{', std::numeric_limits<int8_t>::max()}},
GOTO_STATE41[10] ={
    {'0', 71},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 71},
    {'[', std::numeric_limits<int8_t>::max()},
    {'_', 71},
    {'`', std::numeric_limits<int8_t>::max()},
    {'a', 71},
    {'e', 49},
    {'f', 71},
    {'{', std::numeric_limits<int8_t>::max()}},
GOTO_STATE42[10] ={
    {'0', 71},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 71},
    {'[', std::numeric_limits<int8_t>::max()},
    {'_', 71},
    {'`', std::numeric_limits<int8_t>::max()},
    {'a', 71},
    {'e', 59},
    {'f', 71},
    {'{', std::numeric_limits<int8_t>::max()}},
GOTO_STATE43[10] ={
    {'0', 71},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 71},
    {'[', std::numeric_limits<int8_t>::max()},
    {'_', 71},
    {'`', std::numeric_limits<int8_t>::max()},
    {'a', 71},
    {'e', 66},
    {'f', 71},
    {'{', std::numeric_limits<int8_t>::max()}},
GOTO_STATE44[10] ={
    {'0', 71},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 71},
    {'[', std::numeric_limits<int8_t>::max()},
    {'_', 71},
    {'`', std::numeric_limits<int8_t>::max()},
    {'a', 71},
    {'f', 4},
    {'g', 71},
    {'{', std::numeric_limits<int8_t>::max()}},
GOTO_STATE45[10] ={
    {'0', 71},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 71},
    {'[', std::numeric_limits<int8_t>::max()},
    {'_', 71},
    {'`', std::numeric_limits<int8_t>::max()},
    {'a', 71},
    {'f', 5},
    {'g', 71},
    {'{', std::numeric_limits<int8_t>::max()}},
GOTO_STATE46[10] ={
    {'0', 71},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 71},
    {'[', std::numeric_limits<int8_t>::max()},
    {'_', 71},
    {'`', std::numeric_limits<int8_t>::max()},
    {'a', 71},
    {'f', 6},
    {'g', 71},
    {'{', std::numeric_limits<int8_t>::max()}},
GOTO_STATE47[12] ={
    {'0', 71},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 71},
    {'[', std::numeric_limits<int8_t>::max()},
    {'_', 71},
    {'`', std::numeric_limits<int8_t>::max()},
    {'a', 71},
    {'f', 24},
    {'g', 71},
    {'n', 25},
    {'o', 71},
    {'{', std::numeric_limits<int8_t>::max()}},
GOTO_STATE48[12] ={
    {'0', 71},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 71},
    {'[', std::numeric_limits<int8_t>::max()},
    {'_', 71},
    {'`', std::numeric_limits<int8_t>::max()},
    {'a', 71},
    {'f', 47},
    {'g', 71},
    {'x', 48},
    {'y', 71},
    {'{', std::numeric_limits<int8_t>::max()}},
GOTO_STATE49[10] ={
    {'0', 71},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 71},
    {'[', std::numeric_limits<int8_t>::max()},
    {'_', 71},
    {'`', std::numeric_limits<int8_t>::max()},
    {'a', 71},
    {'g', 50},
    {'h', 71},
    {'{', std::numeric_limits<int8_t>::max()}},
GOTO_STATE50[10] ={
    {'0', 71},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 71},
    {'[', std::numeric_limits<int8_t>::max()},
    {'_', 71},
    {'`', std::numeric_limits<int8_t>::max()},
    {'a', 71},
    {'h', 63},
    {'i', 71},
    {'{', std::numeric_limits<int8_t>::max()}},
GOTO_STATE51[10] ={
    {'0', 71},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 71},
    {'[', std::numeric_limits<int8_t>::max()},
    {'_', 71},
    {'`', std::numeric_limits<int8_t>::max()},
    {'a', 71},
    {'i', 28},
    {'j', 71},
    {'{', std::numeric_limits<int8_t>::max()}},
GOTO_STATE52[10] ={
    {'0', 71},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 71},
    {'[', std::numeric_limits<int8_t>::max()},
    {'_', 71},
    {'`', std::numeric_limits<int8_t>::max()},
    {'a', 71},
    {'i', 58},
    {'j', 71},
    {'{', std::numeric_limits<int8_t>::max()}},
GOTO_STATE53[10] ={
    {'0', 71},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 71},
    {'[', std::numeric_limits<int8_t>::max()},
    {'_', 71},
    {'`', std::numeric_limits<int8_t>::max()},
    {'a', 71},
    {'i', 62},
    {'j', 71},
    {'{', std::numeric_limits<int8_t>::max()}},
GOTO_STATE54[10] ={
    {'0', 71},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 71},
    {'[', std::numeric_limits<int8_t>::max()},
    {'_', 71},
    {'`', std::numeric_limits<int8_t>::max()},
    {'a', 71},
    {'l', 21},
    {'m', 71},
    {'{', std::numeric_limits<int8_t>::max()}},
GOTO_STATE55[12] ={
    {'0', 71},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 71},
    {'[', std::numeric_limits<int8_t>::max()},
    {'_', 71},
    {'`', std::numeric_limits<int8_t>::max()},
    {'a', 71},
    {'l', 22},
    {'m', 71},
    {'n', 23},
    {'o', 71},
    {'{', std::numeric_limits<int8_t>::max()}},
GOTO_STATE56[10] ={
    {'0', 71},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 71},
    {'[', std::numeric_limits<int8_t>::max()},
    {'_', 71},
    {'`', std::numeric_limits<int8_t>::max()},
    {'a', 71},
    {'l', 61},
    {'m', 71},
    {'{', std::numeric_limits<int8_t>::max()}},
GOTO_STATE57[10] ={
    {'0', 71},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 71},
    {'[', std::numeric_limits<int8_t>::max()},
    {'_', 71},
    {'`', std::numeric_limits<int8_t>::max()},
    {'a', 71},
    {'r', 27},
    {'s', 71},
    {'{', std::numeric_limits<int8_t>::max()}},
GOTO_STATE58[10] ={
    {'0', 71},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 71},
    {'[', std::numeric_limits<int8_t>::max()},
    {'_', 71},
    {'`', std::numeric_limits<int8_t>::max()},
    {'a', 71},
    {'s', 2},
    {'t', 71},
    {'{', std::numeric_limits<int8_t>::max()}},
GOTO_STATE59[10] ={
    {'0', 71},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 71},
    {'[', std::numeric_limits<int8_t>::max()},
    {'_', 71},
    {'`', std::numeric_limits<int8_t>::max()},
    {'a', 71},
    {'s', 42},
    {'t', 71},
    {'{', std::numeric_limits<int8_t>::max()}},
GOTO_STATE60[10] ={
    {'0', 71},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 71},
    {'[', std::numeric_limits<int8_t>::max()},
    {'_', 71},
    {'`', std::numeric_limits<int8_t>::max()},
    {'a', 71},
    {'s', 57},
    {'t', 71},
    {'{', std::numeric_limits<int8_t>::max()}},
GOTO_STATE61[10] ={
    {'0', 71},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 71},
    {'[', std::numeric_limits<int8_t>::max()},
    {'_', 71},
    {'`', std::numeric_limits<int8_t>::max()},
    {'a', 71},
    {'t', 8},
    {'u', 71},
    {'{', std::numeric_limits<int8_t>::max()}},
GOTO_STATE62[10] ={
    {'0', 71},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 71},
    {'[', std::numeric_limits<int8_t>::max()},
    {'_', 71},
    {'`', std::numeric_limits<int8_t>::max()},
    {'a', 71},
    {'t', 11},
    {'u', 71},
    {'{', std::numeric_limits<int8_t>::max()}},
GOTO_STATE63[10] ={
    {'0', 71},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 71},
    {'[', std::numeric_limits<int8_t>::max()},
    {'_', 71},
    {'`', std::numeric_limits<int8_t>::max()},
    {'a', 71},
    {'u', 67},
    {'v', 71},
    {'{', std::numeric_limits<int8_t>::max()}},
GOTO_STATE64[8] ={
    {'0', 71},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 71},
    {'[', std::numeric_limits<int8_t>::max()},
    {'_', 71},
    {'`', std::numeric_limits<int8_t>::max()},
    {'a', 71},
    {'{', std::numeric_limits<int8_t>::max()}},
GOTO_STATE65[10] ={
    {'0', 80},
    {':', std::numeric_limits<int8_t>::max()},
    {'>', 40},
    {'?', std::numeric_limits<int8_t>::max()},
    {'A', 80},
    {'[', std::numeric_limits<int8_t>::max()},
    {'_', 80},
    {'`', std::numeric_limits<int8_t>::max()},
    {'a', 80},
    {'{', std::numeric_limits<int8_t>::max()}},
GOTO_STATE66[6] ={
    {'0', 82},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 82},
    {'G', std::numeric_limits<int8_t>::max()},
    {'a', 82},
    {'g', std::numeric_limits<int8_t>::max()}},
GOTO_STATE67[6] ={
    {'0', 84},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 84},
    {'G', std::numeric_limits<int8_t>::max()},
    {'a', 84},
    {'g', std::numeric_limits<int8_t>::max()}},
GOTO_STATE68[6] ={
    {'0', 93},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 93},
    {'G', std::numeric_limits<int8_t>::max()},
    {'a', 93},
    {'g', std::numeric_limits<int8_t>::max()}},
GOTO_STATE69[6] ={
    {'0', 96},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 96},
    {'G', std::numeric_limits<int8_t>::max()},
    {'a', 96},
    {'g', std::numeric_limits<int8_t>::max()}},
GOTO_STATE70[6] ={
    {'0', 97},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 97},
    {'G', std::numeric_limits<int8_t>::max()},
    {'a', 97},
    {'g', std::numeric_limits<int8_t>::max()}},
GOTO_STATE71[6] ={
    {'0', 98},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 98},
    {'G', std::numeric_limits<int8_t>::max()},
    {'a', 98},
    {'g', std::numeric_limits<int8_t>::max()}},
GOTO_STATE72[6] ={
    {'0', 99},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 99},
    {'G', std::numeric_limits<int8_t>::max()},
    {'a', 99},
    {'g', std::numeric_limits<int8_t>::max()}},
GOTO_STATE73[6] ={
    {'0', 100},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 100},
    {'G', std::numeric_limits<int8_t>::max()},
    {'a', 100},
    {'g', std::numeric_limits<int8_t>::max()}},
GOTO_STATE74[6] ={
    {'0', 101},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 101},
    {'G', std::numeric_limits<int8_t>::max()},
    {'a', 101},
    {'g', std::numeric_limits<int8_t>::max()}},
GOTO_STATE75[6] ={
    {'0', 102},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 102},
    {'G', std::numeric_limits<int8_t>::max()},
    {'a', 102},
    {'g', std::numeric_limits<int8_t>::max()}},
GOTO_STATE76[6] ={
    {'0', 103},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 103},
    {'G', std::numeric_limits<int8_t>::max()},
    {'a', 103},
    {'g', std::numeric_limits<int8_t>::max()}},
GOTO_STATE77[6] ={
    {'0', 104},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 104},
    {'G', std::numeric_limits<int8_t>::max()},
    {'a', 104},
    {'g', std::numeric_limits<int8_t>::max()}},
GOTO_STATE78[6] ={
    {'0', 105},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 105},
    {'G', std::numeric_limits<int8_t>::max()},
    {'a', 105},
    {'g', std::numeric_limits<int8_t>::max()}},
GOTO_STATE79[6] ={
    {'0', 106},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 106},
    {'G', std::numeric_limits<int8_t>::max()},
    {'a', 106},
    {'g', std::numeric_limits<int8_t>::max()}},
GOTO_STATE80[6] ={
    {'0', 108},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 108},
    {'G', std::numeric_limits<int8_t>::max()},
    {'a', 108},
    {'g', std::numeric_limits<int8_t>::max()}},
GOTO_STATE81[6] ={
    {'0', 109},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 109},
    {'G', std::numeric_limits<int8_t>::max()},
    {'a', 109},
    {'g', std::numeric_limits<int8_t>::max()}},
GOTO_STATE82[6] ={
    {'0', 112},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 112},
    {'G', std::numeric_limits<int8_t>::max()},
    {'a', 112},
    {'g', std::numeric_limits<int8_t>::max()}},
GOTO_STATE83[6] ={
    {'0', 113},
    {':', std::numeric_limits<int8_t>::max()},
    {'A', 113},
    {'G', std::numeric_limits<int8_t>::max()},
    {'a', 113},
    {'g', std::numeric_limits<int8_t>::max()}},
GOTO_STATE84[4] ={
    {'X', 81},
    {'Y', std::numeric_limits<int8_t>::max()},
    {'x', 81},
    {'y', std::numeric_limits<int8_t>::max()}};

constinit const C_ParserScanner::C_StateRec STATES[114] = {
    { GOTO_STATE11,	nullptr	},  // 0
    { GOTO_STATE27,	bux::createPlainString<TID_LEX_LexSymbol,bux::C_LexUTF32, 1, 0>	},  // 1
    { GOTO_STATE64,	bux::createNothing<TID_KEYWORD_class,bux::C_LexUTF32>	},  // 2
    { GOTO_STATE64,	bux::createNothing<TID_KEYWORD_else,bux::C_LexUTF32>	},  // 3
    { GOTO_STATE64,	bux::createNothing<TID_KEYWORD_endif,bux::C_LexUTF32>	},  // 4
    { GOTO_STATE64,	bux::createNothing<TID_KEYWORD_ifdef,bux::C_LexUTF32>	},  // 5
    { GOTO_STATE64,	bux::createNothing<TID_KEYWORD_ifndef,bux::C_LexUTF32>	},  // 6
    { GOTO_STATE64,	bux::createNothing<TID_KEYWORD_include,bux::C_LexUTF32>	},  // 7
    { GOTO_STATE64,	bux::createNothing<TID_KEYWORD_left,bux::C_LexUTF32>	},  // 8
    { GOTO_STATE64,	bux::createNothing<TID_KEYWORD_lexid,bux::C_LexUTF32>	},  // 9
    { GOTO_STATE64,	bux::createNothing<TID_KEYWORD_prec,bux::C_LexUTF32>	},  // 10
    { GOTO_STATE64,	bux::createNothing<TID_KEYWORD_right,bux::C_LexUTF32>	},  // 11
    { GOTO_STATE54,	bux::createPlainString<TID_LEX_Id,bux::C_LexUTF32>	},  // 12
    { GOTO_STATE55,	bux::createPlainString<TID_LEX_Id,bux::C_LexUTF32>	},  // 13
    { GOTO_STATE47,	bux::createPlainString<TID_LEX_Id,bux::C_LexUTF32>	},  // 14
    { GOTO_STATE40,	bux::createPlainString<TID_LEX_Id,bux::C_LexUTF32>	},  // 15
    { GOTO_STATE57,	bux::createPlainString<TID_LEX_Id,bux::C_LexUTF32>	},  // 16
    { GOTO_STATE51,	bux::createPlainString<TID_LEX_Id,bux::C_LexUTF32>	},  // 17
    { GOTO_STATE28,	bux::createDecNum<TID_LEX_Num,bux::C_LexUTF32>	},  // 18
    { GOTO_STATE84,	bux::createDecNum<TID_LEX_Num,bux::C_LexUTF32>	},  // 19
    { GOTO_STATE12,	bux::createNothing<TID_LEX_Spaces,bux::C_LexUTF32>	},  // 20
    { GOTO_STATE30,	bux::createPlainString<TID_LEX_Id,bux::C_LexUTF32>	},  // 21
    { GOTO_STATE59,	bux::createPlainString<TID_LEX_Id,bux::C_LexUTF32>	},  // 22
    { GOTO_STATE34,	bux::createPlainString<TID_LEX_Id,bux::C_LexUTF32>	},  // 23
    { GOTO_STATE35,	bux::createPlainString<TID_LEX_Id,bux::C_LexUTF32>	},  // 24
    { GOTO_STATE32,	bux::createPlainString<TID_LEX_Id,bux::C_LexUTF32>	},  // 25
    { GOTO_STATE48,	bux::createPlainString<TID_LEX_Id,bux::C_LexUTF32>	},  // 26
    { GOTO_STATE41,	bux::createPlainString<TID_LEX_Id,bux::C_LexUTF32>	},  // 27
    { GOTO_STATE49,	bux::createPlainString<TID_LEX_Id,bux::C_LexUTF32>	},  // 28
    { GOTO_STATE20,	bux::createPlainString<TID_LEX_Operator,bux::C_LexUTF32>	},  // 29
    { GOTO_STATE20,	bux::createPlainString<TID_LEX_Nonterminal,bux::C_LexUTF32, 1, 1>	},  // 30
    { GOTO_STATE20,	bux::createNothing<TID_LITERAL_3a3a3d,bux::C_LexUTF32>	},  // 31
    { GOTO_STATE2,	bux::createPlainString<TID_LEX_Operator,bux::C_LexUTF32>	},  // 32
    { GOTO_STATE4,	bux::createPlainString<TID_LEX_Operator,bux::C_LexUTF32>	},  // 33
    { GOTO_STATE7,	bux::createPlainString<TID_LEX_Operator,bux::C_LexUTF32>	},  // 34
    { GOTO_STATE17,	bux::createPlainString<TID_LEX_Operator,bux::C_LexUTF32>	},  // 35
    { GOTO_STATE16,	bux::createNothing<TID_LITERAL_3a3a,bux::C_LexUTF32>	},  // 36
    { nullptr,	bux::createCharLiteral<TID_LEX_Key,bux::C_LexUTF32>	},  // 37
    { nullptr,	bux::createEscapeString<TID_LEX_String,bux::C_LexUTF32, 1, 1>	},  // 38
    { nullptr,	bux::createNothing<'\n',bux::C_LexUTF32>	},  // 39
    { nullptr,	bux::createPlainString<TID_LEX_Nonterminal,bux::C_LexUTF32, 1, 1>	},  // 40
    { GOTO_STATE60,	bux::createPlainString<TID_LEX_Id,bux::C_LexUTF32>	},  // 41
    { GOTO_STATE38,	bux::createPlainString<TID_LEX_Id,bux::C_LexUTF32>	},  // 42
    { GOTO_STATE52,	bux::createPlainString<TID_LEX_Id,bux::C_LexUTF32>	},  // 43
    { GOTO_STATE42,	bux::createPlainString<TID_LEX_Id,bux::C_LexUTF32>	},  // 44
    { GOTO_STATE36,	bux::createPlainString<TID_LEX_Id,bux::C_LexUTF32>	},  // 45
    { GOTO_STATE56,	bux::createPlainString<TID_LEX_Id,bux::C_LexUTF32>	},  // 46
    { GOTO_STATE61,	bux::createPlainString<TID_LEX_Id,bux::C_LexUTF32>	},  // 47
    { GOTO_STATE53,	bux::createPlainString<TID_LEX_Id,bux::C_LexUTF32>	},  // 48
    { GOTO_STATE31,	bux::createPlainString<TID_LEX_Id,bux::C_LexUTF32>	},  // 49
    { GOTO_STATE50,	bux::createPlainString<TID_LEX_Id,bux::C_LexUTF32>	},  // 50
    { GOTO_STATE2,	bux::createPlainString<TID_LEX_Operator,bux::C_LexUTF32>	},  // 51
    { GOTO_STATE3,	bux::createPlainString<TID_LEX_Operator,bux::C_LexUTF32>	},  // 52
    { GOTO_STATE4,	bux::createPlainString<TID_LEX_Operator,bux::C_LexUTF32>	},  // 53
    { GOTO_STATE8,	bux::createPlainString<TID_LEX_Operator,bux::C_LexUTF32>	},  // 54
    { GOTO_STATE7,	bux::createPlainString<TID_LEX_Operator,bux::C_LexUTF32>	},  // 55
    { GOTO_STATE29,	bux::createHexNum<TID_LEX_Num,bux::C_LexUTF32>	},  // 56
    { GOTO_STATE58,	bux::createPlainString<TID_LEX_Id,bux::C_LexUTF32>	},  // 57
    { GOTO_STATE44,	bux::createPlainString<TID_LEX_Id,bux::C_LexUTF32>	},  // 58
    { GOTO_STATE45,	bux::createPlainString<TID_LEX_Id,bux::C_LexUTF32>	},  // 59
    { GOTO_STATE43,	bux::createPlainString<TID_LEX_Id,bux::C_LexUTF32>	},  // 60
    { GOTO_STATE63,	bux::createPlainString<TID_LEX_Id,bux::C_LexUTF32>	},  // 61
    { GOTO_STATE33,	bux::createPlainString<TID_LEX_Id,bux::C_LexUTF32>	},  // 62
    { GOTO_STATE62,	bux::createPlainString<TID_LEX_Id,bux::C_LexUTF32>	},  // 63
    { GOTO_STATE4,	bux::createNothing<TID_LEX_Spaces,bux::C_LexUTF32>	},  // 64
    { GOTO_STATE8,	bux::createPlainString<TID_LEX_Bracketed,bux::C_LexUTF32, 2, 2>	},  // 65
    { GOTO_STATE46,	bux::createPlainString<TID_LEX_Id,bux::C_LexUTF32>	},  // 66
    { GOTO_STATE37,	bux::createPlainString<TID_LEX_Id,bux::C_LexUTF32>	},  // 67
    { GOTO_STATE5,	bux::createNothing<TID_LEX_Spaces,bux::C_LexUTF32>	},  // 68
    { GOTO_STATE9,	bux::createPlainString<TID_LEX_Bracketed,bux::C_LexUTF32, 2, 2>	},  // 69
    { GOTO_STATE39,	bux::createPlainString<TID_LEX_Id,bux::C_LexUTF32>	},  // 70
    { GOTO_STATE64,	bux::createPlainString<TID_LEX_Id,bux::C_LexUTF32>	},  // 71
    { GOTO_STATE20,	nullptr	},  // 72
    { GOTO_STATE15,	nullptr	},  // 73
    { GOTO_STATE21,	nullptr	},  // 74
    { GOTO_STATE19,	nullptr	},  // 75
    { GOTO_STATE18,	nullptr	},  // 76
    { GOTO_STATE22,	nullptr	},  // 77
    { GOTO_STATE13,	nullptr	},  // 78
    { GOTO_STATE14,	nullptr	},  // 79
    { GOTO_STATE65,	nullptr	},  // 80
    { GOTO_STATE29,	nullptr	},  // 81
    { GOTO_STATE25,	nullptr	},  // 82
    { GOTO_STATE23,	nullptr	},  // 83
    { GOTO_STATE14,	nullptr	},  // 84
    { GOTO_STATE24,	nullptr	},  // 85
    { GOTO_STATE1,	nullptr	},  // 86
    { GOTO_STATE2,	nullptr	},  // 87
    { GOTO_STATE5,	nullptr	},  // 88
    { GOTO_STATE4,	nullptr	},  // 89
    { GOTO_STATE10,	nullptr	},  // 90
    { GOTO_STATE7,	nullptr	},  // 91
    { GOTO_STATE82,	nullptr	},  // 92
    { GOTO_STATE76,	nullptr	},  // 93
    { GOTO_STATE69,	nullptr	},  // 94
    { GOTO_STATE70,	nullptr	},  // 95
    { GOTO_STATE71,	nullptr	},  // 96
    { GOTO_STATE72,	nullptr	},  // 97
    { GOTO_STATE73,	nullptr	},  // 98
    { GOTO_STATE74,	nullptr	},  // 99
    { GOTO_STATE68,	nullptr	},  // 100
    { GOTO_STATE75,	nullptr	},  // 101
    { GOTO_STATE77,	nullptr	},  // 102
    { GOTO_STATE78,	nullptr	},  // 103
    { GOTO_STATE79,	nullptr	},  // 104
    { GOTO_STATE80,	nullptr	},  // 105
    { GOTO_STATE81,	nullptr	},  // 106
    { GOTO_STATE83,	nullptr	},  // 107
    { GOTO_STATE66,	nullptr	},  // 108
    { GOTO_STATE67,	nullptr	},  // 109
    { GOTO_STATE6,	nullptr	},  // 110
    { GOTO_STATE9,	nullptr	},  // 111
    { GOTO_STATE26,	nullptr	},  // 112
    { GOTO_STATE14,	nullptr	}}; // 113

constinit const uint8_t GOTO_SIZE[114] = {
    38,	// 0
    8,	// 1
    8,	// 2
    8,	// 3
    8,	// 4
    8,	// 5
    8,	// 6
    8,	// 7
    8,	// 8
    8,	// 9
    8,	// 10
    8,	// 11
    10,	// 12
    12,	// 13
    12,	// 14
    10,	// 15
    10,	// 16
    10,	// 17
    2,	// 18
    4,	// 19
    6,	// 20
    9,	// 21
    10,	// 22
    10,	// 23
    12,	// 24
    10,	// 25
    12,	// 26
    10,	// 27
    10,	// 28
    13,	// 29
    13,	// 30
    13,	// 31
    16,	// 32
    16,	// 33
    16,	// 34
    15,	// 35
    15,	// 36
    0,	// 37
    0,	// 38
    0,	// 39
    0,	// 40
    10,	// 41
    10,	// 42
    10,	// 43
    10,	// 44
    10,	// 45
    10,	// 46
    10,	// 47
    10,	// 48
    10,	// 49
    10,	// 50
    16,	// 51
    17,	// 52
    16,	// 53
    16,	// 54
    16,	// 55
    6,	// 56
    10,	// 57
    10,	// 58
    10,	// 59
    10,	// 60
    10,	// 61
    10,	// 62
    10,	// 63
    16,	// 64
    16,	// 65
    10,	// 66
    10,	// 67
    3,	// 68
    3,	// 69
    10,	// 70
    8,	// 71
    13,	// 72
    16,	// 73
    14,	// 74
    13,	// 75
    14,	// 76
    14,	// 77
    5,	// 78
    5,	// 79
    10,	// 80
    6,	// 81
    2,	// 82
    22,	// 83
    5,	// 84
    22,	// 85
    3,	// 86
    16,	// 87
    3,	// 88
    16,	// 89
    3,	// 90
    16,	// 91
    6,	// 92
    6,	// 93
    6,	// 94
    6,	// 95
    6,	// 96
    6,	// 97
    6,	// 98
    6,	// 99
    6,	// 100
    6,	// 101
    6,	// 102
    6,	// 103
    6,	// 104
    6,	// 105
    6,	// 106
    6,	// 107
    6,	// 108
    6,	// 109
    5,	// 110
    3,	// 111
    8,	// 112
    5};	// 113

constinit const int8_t FIRST_FITS[4] = {
    64,
    65,
    68,
    69};

constinit C_ParserScanner::F_IsFinal *const IS_FINALS[4] = {
    nullptr,
    bux::bracketsMatched,
    nullptr,
    bux::bracketsMatched};

} // namespace

C_ParserScanner::C_ParserScanner(bux::I_Parser &parser):
    bux::C_ScannerImpl<uint8_t,int8_t,bux::C_LexUTF32>(parser)
{
    stateTables(STATES, GOTO_SIZE);
    firstFits(FIRST_FITS, IS_FINALS, 4);
}
