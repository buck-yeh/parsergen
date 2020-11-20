#include "spec2charset.h"
#include <set>              // std::set<>

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

std::optional<C_S2C_Error> spec2charset(const std::string &spec, std::string &result)
{
    std::set<char> charset;
    for (auto i = spec.begin(), end = spec.end(); i != end;)
    {
        auto start = *i;
        if (++i != end && *i == '-')
        {
            const auto t1 = rangeType(start);
            if (t1 == CR_None)
                return C_S2C_Error{i - spec.begin(), SCE_INVALID_CHAR};

            if (++i == end)
                return C_S2C_Error{spec.size(), SCE_MSSING_UB};
            if (t1 != rangeType(*i))
                return C_S2C_Error{i - spec.begin(), SCE_NOT_IN_SAME_GROUP};
            if (*i < start)
                return C_S2C_Error{i - spec.begin(), SCE_LB_GREATOR_THAN_UB};

            do  charset.insert(start);
                while (++start <= *i);

            ++i;
        }
        else
            charset.insert(start);
    }
    result.clear();
    for (auto c: charset)
        result += c;

    return {};
}
