#pragma once

#include <exception>
#include "CoreDEF.h"

namespace bv {

typedef     Int32       ExceptionType;

class Exception : public std::exception
{
    ExceptionType               type;
public:
                                Exception();

    virtual std::string         GetReason() = 0;

    static ExceptionType        RegisterType();
};

DEFINE_PTR_TYPE( Exception );

class RuntimeException : public Exception
{
    std::string                 reason;

public:
                                RuntimeException    ( const std::string& reason );

    std::string                 GetReason() override;
};

}
