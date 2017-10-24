#pragma once

#include <exception>
#include "CoreDEF.h"



namespace bv
{

typedef     Int32       ExceptionType;


// ***********************
//
class Exception : public std::exception
{
public:
                                Exception();

    virtual std::string         GetReason() = 0;
    virtual ExceptionType       GetType() = 0;

    static ExceptionType        RegisterType();
};

DEFINE_PTR_TYPE( Exception );



typedef std::vector< ExceptionPtr > Exceptions;

// ***********************
//
class RuntimeException : public Exception
{
    static ExceptionType        type;
public:
    static ExceptionType        Type();

protected:

    std::string                 reason;

public:
                                RuntimeException    ( const std::string& reason );

    std::string                 GetReason() override;
    ExceptionType               GetType() override;
};

}
