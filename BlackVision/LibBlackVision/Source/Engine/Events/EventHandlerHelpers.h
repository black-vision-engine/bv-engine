#pragma once

#include "Tools/IncludeJSON.h"


namespace bv
{

Json::Value             GetRequestParamValue        ( const std::string & request );
Json::Value             Str2Json                    ( const std::string & data );

bool                    IsPresetScene               ( const std::string & sceneName );

} //bv

#include "EventHandlerHelpers.inl"
