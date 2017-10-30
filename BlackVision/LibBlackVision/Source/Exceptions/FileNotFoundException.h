#pragma once

#include "Exception.h"


namespace bv
{

class FileNotFoundException : public Exception
{
    static ExceptionType        type;

public:
    static ExceptionType        Type() { return type; }

public:
    virtual std::string         GetReason() { return "File not found."; }
    virtual ExceptionType       GetType() { return Type(); }
};


}	// bv
