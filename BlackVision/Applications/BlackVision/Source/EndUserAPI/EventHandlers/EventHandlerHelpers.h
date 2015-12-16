#pragma once

#include "Engine/Models/ModelNodeEditor.h"
#include "Tools/IncludeJSON.h"
#include "System/Path.h"

#include <string>

namespace bv
{
class BVAppLogic;

model::IModelNodePtr    GetNode     ( BVAppLogic* appLogic, const std::string& sceneName, const std::string& nodeName );
model::IModelNodePtr    GetRootNode ( BVAppLogic* appLogic, const std::string& sceneName );

void                    SendOnSceneStructureResponse( int socketID, const std::string & cmd, const std::string & msgKey, const Json::Value & msgVal );

Json::Value             ToJSONArray                 ( const PathVec& v );
Json::Value             GetRequestParamValue        ( std::string& request );
Json::Value             Str2Json                    ( const std::string& data );
Json::Value             ToJSONArray                 ( const StringVector& v );

} //bv

