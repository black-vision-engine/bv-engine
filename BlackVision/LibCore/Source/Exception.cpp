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

RuntimeException::RuntimeException( const std::string & reason )
    : reason( reason )
{
}

std::string            RuntimeException::GetReason() {
    return reason;
}



}