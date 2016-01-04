#pragma once

#include "Engine/Models/ModelNodeEditor.h"
#include "Tools/IncludeJSON.h"
#include "System/Path.h"
#include "Engine/Models/SceneModel.h"
#include "Engine/Models/Plugins/Interfaces/IParameter.h"

#include <string>

namespace bv
{
class BVAppLogic;

Json::Value             GetRequestParamValue        ( const std::string& request );
Json::Value             Str2Json                    ( const std::string& data );


} //bv

#include "EventHandlerHelpers.inl"

