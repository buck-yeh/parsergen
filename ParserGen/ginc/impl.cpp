#include "impl.h"

%ON_ERROR [[
    $c.log(LL_ERROR, $pos, $message);
]]

%UPCAST_TOKEN [[
    if (const auto it = int($token); isascii(it) && !iscntrl(it) && !isalnum(it) && !isspace(it))
    {
        $attr.assign(bux::createLex<std::string>(1u,char($token)), true);
        $token = TID_LEX_Operator;
        return true;
    }
    return false;
]]
