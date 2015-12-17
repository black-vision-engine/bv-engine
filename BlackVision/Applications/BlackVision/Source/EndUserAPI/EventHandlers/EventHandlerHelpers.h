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

model::IModelNodePtr    GetNode     ( BVAppLogic* appLogic, const std::string& sceneName, const std::string& nodeName );
model::IModelNodePtr    GetRootNode ( BVAppLogic* appLogic, const std::string& sceneName );

void                    SendOnSceneStructureResponse( int socketID, const std::string & cmd, const std::string & msgKey, const Json::Value & msgVal );
std::wstring            MakeSceneStructureResponse  ( const std::string & cmd, const std::string & msgKey, const Json::Value & msgVal );

Json::Value             ToJSONArray                 ( const PathVec& v );
Json::Value             GetRequestParamValue        ( const std::string& request );
Json::Value             Str2Json                    ( const std::string& data );
Json::Value             ToJSONArray                 ( const StringVector& v );

void                    ReqPrint                    ( model::BasicNodePtr node, int level );
Json::Value             SerializeSceneModel         ( model::SceneModelPtr sceneModel );


template< typename ParamTypePtr >
Json::Value GetParamDescription( model::IParameterPtr p )
{
    string s_name = p->GetName();
    auto paramType = p->GetType();

    Json::Value entry;

    entry[ "name" ] = s_name;
    entry[ "type" ] = ParamTypeToString( paramType );

    Json::Value jsonKeys;

    auto typedParam = QueryTypedParam< ParamTypePtr >( p );
    auto accessIntepolator = typedParam->AccessInterpolator();
    auto keys = accessIntepolator.AccessKeys();
    for( auto & k : keys )
    {
        jsonKeys.append( toString( k.t ) );
        jsonKeys.append( toString( k.val ) );
    }

    entry[ "keys" ] = jsonKeys;

    return entry;
}

} //bv

