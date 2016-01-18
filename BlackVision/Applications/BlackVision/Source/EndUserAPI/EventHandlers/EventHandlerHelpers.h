#pragma once

#include "Tools/IncludeJSON.h"


namespace bv
{

class BVAppLogic;


Json::Value             GetRequestParamValue        ( const std::string & request );
Json::Value             Str2Json                    ( const std::string & data );

} //bv

#include "EventHandlerHelpers.inl"
