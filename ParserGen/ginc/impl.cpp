#include "ginc/impl.h"

%ON_ERROR [[
    $c.issueError(LL_ERROR, $pos, $message);
]]

%UPCAST_TOKEN [[
    if (isascii($token) && !iscntrl($token) && !isalnum($token) && !isspace($token))
    {
        $attr.assign(bux::createLex<std::string>(1,char($token)), true);
        $token = TID_LEX_Operator;
        return true;
    }
    return false;
]]
