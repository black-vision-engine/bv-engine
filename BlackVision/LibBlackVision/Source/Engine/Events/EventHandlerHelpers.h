#pragma once

#include "Tools/IncludeJSON.h"
#include "Engine/Models/Plugins/Interfaces/IParameter.h"


namespace bv
{

class BVProjectEditor;
typedef std::shared_ptr< model::IParameter > ParameterPtr;

namespace model
{
class SceneModel;
DEFINE_PTR_TYPE( SceneModel );
}


Json::Value             GetRequestParamValue        ( const std::string & request );
Json::Value             Str2Json                    ( const std::string & data );

bool                    IsPresetScene               ( const std::string & sceneName );

ParameterPtr            GetParameter            ( BVProjectEditor * editor, const ParameterAddress & paramAddress );
ParameterPtr            GetParameter            ( model::SceneModel * scene, const ParameterAddress & paramAddress );

ParameterPtr            GetPluginParameter      ( BVProjectEditor * editor,
                                                  const std::string & sceneName,
                                                  const std::string & nodePath,
                                                  const std::string & pluginName,
                                                  const std::string & paramName );

ParameterPtr            GetPluginParameter      ( model::SceneModel * scene,
                                                  const std::string & nodePath,
                                                  const std::string & pluginName,
                                                  const std::string & paramName );

ParameterPtr            GetGlobalEffectParameter( BVProjectEditor * editor,
                                                  const std::string & sceneName,
                                                  const std::string & nodePath,
                                                  const std::string & paramName );

ParameterPtr            GetGlobalEffectParameter( model::SceneModel * scene,
                                                  const std::string & nodePath,
                                                  const std::string & paramName );

ParameterPtr            GetResourceParameter    ( BVProjectEditor * editor,
                                                  const std::string & sceneName,
                                                  const std::string & nodePath,
                                                  const std::string & pluginName,
                                                  const std::string & textureName,
                                                  const std::string & paramName );

ParameterPtr            GetResourceParameter    ( model::SceneModel * scene,
                                                  const std::string & nodePath,
                                                  const std::string & pluginName,
                                                  const std::string & textureName,
                                                  const std::string & paramName );

ParameterPtr            GetLightParameter       ( BVProjectEditor * editor,
                                                  const std::string & sceneName,
                                                  UInt32 lightIndex,
                                                  const std::string & paramName );

ParameterPtr            GetLightParameter       ( model::SceneModel * scene,
                                                  UInt32 lightIndex,
                                                  const std::string & paramName );

ParameterPtr            GetNodeLogicParameter   ( BVProjectEditor * editor,
                                                  const std::string & sceneName,
                                                  const std::string & nodePath,
                                                  const std::string & paramName );

ParameterPtr            GetNodeLogicParameter   ( model::SceneModel * scene,
                                                  const std::string & nodePath,
                                                  const std::string & paramName );

ParameterPtr            GetCameraParameter      ( BVProjectEditor * editor,
                                                  const std::string & sceneName,
                                                  UInt32 cameraIndex,
                                                  const std::string & paramName );

ParameterPtr            GetCameraParameter      ( model::SceneModel * scene,
                                                  UInt32 cameraIndex,
                                                  const std::string & paramName );

ParameterPtr            GetNodeParameter        ( BVProjectEditor * editor,
                                                  const std::string & sceneName,
                                                  const std::string & nodePath,
                                                  const std::string & paramName );

ParameterPtr            GetNodeParameter        ( model::SceneModel * scene,
                                                  const std::string & nodePath,
                                                  const std::string & paramName );




} //bv

#include "EventHandlerHelpers.inl"
