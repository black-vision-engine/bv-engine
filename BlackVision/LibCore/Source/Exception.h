#pragma once

#include <exception>
#include "CoreDEF.h"

namespace bv {
    typedef     Int32       ExceptionType;

    class Exception : public std::exception
    {
        ExceptionType       type;
    };

    DEFINE_PTR_TYPE( Exception );
}
