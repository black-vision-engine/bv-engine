#include "stdafx.h"

#include "InvalidSceneVersion.h"


namespace bv
{

ExceptionType        InvalidSceneVersion::type = Exception::RegisterType();

InvalidSceneVersion::InvalidSceneVersion( const std::string & detailedReason )
    : RuntimeException( detailedReason )
{}

}	// bv