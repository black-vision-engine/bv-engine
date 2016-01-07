#include "EventHandlerHelpers.h"
#include "../../BVAppLogic.h"


#include "../../UseLoggerBVAppModule.h"

namespace bv
{

// *********************************
//
Json::Value Str2Json( const std::string & data )
{
	Json::Value json;
    Json::Reader reader;
	reader.parse( data, json );
    return json;
}

// *********************************
//
Json::Value GetRequestParamValue( const std::string& request )
{
	return Str2Json( std::string( request.begin(), request.end() ) );
}

} //bv
