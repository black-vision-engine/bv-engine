#pragma once

#include <string>

namespace bv
{
    struct LogMsg
    {
        std::string     message;
        std::string     module;
        std::string     severity;
    };
} //bv