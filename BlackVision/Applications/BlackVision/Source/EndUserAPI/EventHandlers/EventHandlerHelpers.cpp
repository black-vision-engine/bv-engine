#include "EventHandlerHelpers.h"
#include "../../BVAppLogic.h"


#include "../../UseLoggerBVAppModule.h"

namespace bv
{

// *********************************
//
Json::Value Str2Json( const std::string & data )
{
	return Json::Value( data );
}

// *********************************
//
Json::Value GetRequestParamValue( const std::string& request )
{
	return Str2Json( std::string( request.begin(), request.end() ) );
}

} //bv
