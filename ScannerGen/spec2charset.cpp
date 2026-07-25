#include "spec2charset.h"
#include <cstring>          // strstr()

namespace {

enum E_CharRange
{
    CR_None,
    CR_0_to_9,
    CR_A_to_Z,
    CR_a_to_z
};

E_CharRange rangeType(char c)
{
    return '0' <= c && c <= '9'? CR_0_to_9:
           'A' <= c && c <= 'Z'? CR_A_to_Z:
           'a' <= c && c <= 'z'? CR_a_to_z:
                                 CR_None;
}

} // namespace

std::expected<bux::C_Intervals<bux::T_LexID>,C_S2C_Error> spec2charset(std::string_view spec)
{
    bux::C_Intervals<bux::T_LexID> charset;
    for (auto i = spec.begin(), end = spec.end(); i != end;)
    {
        const auto start = *i;
        if (++i != end)
        {
            if (*i == '-')
            {
                const auto t1 = rangeType(start);
                if (t1 == CR_None)
                    return std::unexpected(C_S2C_Error{i - spec.begin(), SCE_INVALID_CHAR});
                if (++i == end)
                    return std::unexpected(C_S2C_Error{spec.size(), SCE_MSSING_UB});
                if (t1 != rangeType(*i))
                    return std::unexpected(C_S2C_Error{i - spec.begin(), SCE_NOT_IN_SAME_GROUP});
                if (*i < start)
                    return std::unexpected(C_S2C_Error{i - spec.begin(), SCE_LB_GREATOR_THAN_UB});

                if (start < *i)
                    charset |= {bux::T_LexID(start), bux::T_LexID(*i)};
                else if (start == *i)
                    charset |= bux::T_LexID(start);
                else
                    {} // do nothing

                ++i;
                continue;
            }
            if (start == '\\' && *i == 'u')
            {
                if (auto next_ucode_esc = strstr(++i, "\\u"))
                {
                    if (next_ucode_esc[-1] == '-')
                    {
                        char* from_end;
                        const auto from = strtoul(i, &from_end, 16);
                        char* to_end;
                        const auto to = strtoul(next_ucode_esc+2, &to_end, 16);
                        if (bux::T_LexID(from) == from &&
                            bux::T_LexID(to) == to &&
                            from_end == next_ucode_esc -1 &&
                            (!*to_end || *to_end != '-'))
                        {
                            if (from < to)
                                charset |= {bux::T_LexID(from), bux::T_LexID(to)};
                            else if (from == to)
                                charset |= bux::T_LexID(from);
                            else
                                {} // do nothing

                            i = to_end;
                            continue;
                        }
                    }
                }
                return std::unexpected(C_S2C_Error{i - spec.begin(), SCE_UCODE_SYNTAX});
            }
        }
        charset |= bux::T_LexID(start);
    }
    return charset;
}
