#include "stdafxCore.h"

#include "Exception.h"

namespace bv
{

ExceptionType        Exception::sInvalidType = 0;
ExceptionType        Exception::type = RegisterType();

ExceptionType        registerType = 1;

// ***********************
//
Exception::Exception()
{}

// ***********************
//
ExceptionType   Exception::RegisterType()
{
    return registerType++;
}

// ***********************
//
bool            Exception::IsValidType  ( ExceptionType type )
{
    return type != sInvalidType;
}

// ***********************
//
ExceptionType   Exception::Type         ()
{
    return Exception::type;
}

// ========================================================================= //
// RuntimeException implementation
// ========================================================================= //

#include "Expected.h"

ExceptionType RuntimeException::type = Exception::RegisterType();

// ***********************
//
ExceptionType        RuntimeException::Type()
{
    return type;
}


// ***********************
//
RuntimeException::RuntimeException( const std::string & reason )
    : reason( reason )
{}

// ***********************
//
std::string                 RuntimeException::GetReason () const
{
    return reason;
}

// ***********************
//
ExceptionType               RuntimeException::GetType   () const
{
    return Type();
}


}