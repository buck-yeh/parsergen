#pragma once

#include "bux/Intervals.h"  // bux::C_Intervals<>
#include "bux/LexBase.h"    // bux::T_LexID
#include <string_view>      // std::string_view
#include <expected>         // std::expected<>

enum E_S2C_Error
{
    SCE_INVALID_CHAR,
    SCE_MSSING_UB,
    SCE_NOT_IN_SAME_GROUP,
    SCE_LB_GREATOR_THAN_UB,
    SCE_UCODE_SYNTAX
};
typedef std::pair<size_t,E_S2C_Error> C_S2C_Error;

std::expected<bux::C_Intervals<bux::T_LexID>,C_S2C_Error> spec2charset(std::string_view spec);
