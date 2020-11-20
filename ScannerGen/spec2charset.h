#ifndef spec2charsetH
#define spec2charsetH

#include <string>   // std::string
#include <optional> // std::optional<>

enum E_S2C_Error
{
    SCE_INVALID_CHAR,
    SCE_MSSING_UB,
    SCE_NOT_IN_SAME_GROUP,
    SCE_LB_GREATOR_THAN_UB
};
typedef std::pair<size_t,E_S2C_Error> C_S2C_Error;

std::optional<C_S2C_Error> spec2charset(const std::string &spec, std::string &result);

#endif // spec2charsetH
