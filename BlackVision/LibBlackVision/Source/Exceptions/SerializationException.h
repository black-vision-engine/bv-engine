#pragma once

#include "Exception.h"

#include <string>


namespace bv
{

// ***********************
//
class SerializationException : public Exception
{
    static ExceptionType        type;

private:

    std::string             m_message;

public:

    explicit        SerializationException   ( const std::string & message )
        : m_message( message )
    {}


    virtual std::string         GetReason   () { return m_message; }
    virtual ExceptionType       GetType     () { return Type(); }

public:

    static ExceptionType        Type() { return type; }

};


}	// bv
