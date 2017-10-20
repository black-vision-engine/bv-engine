#include "stdafxCore.h"

#include "Exception.h"

namespace bv {

Exception::Exception()
{
}

static ExceptionType registerType = 0;

ExceptionType   Exception::RegisterType()
{
    return registerType++;
}

// ========================================================================= //
// RuntimeException implementation
// ========================================================================= //

#include "Expected.h"

ExceptionType RuntimeException::type = Exception::RegisterType();

ExceptionType        RuntimeException::Type()
{
    return type;
}


RuntimeException::RuntimeException( const std::string & reason )
    : reason( reason )
{
}

std::string            RuntimeException::GetReason() {
    return reason;
}

ExceptionType               RuntimeException::GetType()
{
    return Type();
}


}