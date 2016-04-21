#include "stdafx.h"

#include "EventHandlerHelpers.h"


namespace bv
{

// *********************************
//
Json::Value         Str2Json                ( const std::string & data )
{
	Json::Value json;
    Json::Reader reader;

	bool success = reader.parse( data, json );
    if( !success )
    {
        std::string error = reader.getFormattedErrorMessages();
        assert( success );  { success; }
    }

    return json;
}

// *********************************
//
Json::Value         GetRequestParamValue    ( const std::string & request )
{
	return Str2Json( std::string( request.begin(), request.end() ) );
}

// ***********************
//
bool                IsPresetScene               ( const std::string & sceneName )
{
    auto length = sceneName.length();
    auto pos = sceneName.find_last_of( ".bvpreset" );

    if( length - pos == 1 )
        return true;
    return false;
}


} //bv
