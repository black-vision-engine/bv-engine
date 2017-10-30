#pragma once

#include "Exception.h"

namespace bv
{

class InvalidSceneVersion : public RuntimeException
{
    static ExceptionType        type;

public:
    static ExceptionType        Type() { return type; }

public:
                                InvalidSceneVersion( const std::string & detailedReason );
    virtual ExceptionType       GetType() { return Type(); }
};

}	// bv