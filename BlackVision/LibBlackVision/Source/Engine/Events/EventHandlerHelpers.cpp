#include "stdafx.h"

#include "EventHandlerHelpers.h"
#include "Engine/Editors/BVProjectEditor.h"

#include "Engine/Editors/ModelSceneEditor.h"

#include "UseLoggerLibBlackVision.h"




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

// ***********************
//
ParameterPtr    GetParameter            ( BVProjectEditor * editor, const ParameterAddress & paramAddress )
{
    switch( paramAddress.ParamTargetType )
    {
        case ParameterAddress::TargetType::PluginParam:
            return GetPluginParameter( editor, paramAddress.SceneName, paramAddress.NodeName, paramAddress.PluginName, paramAddress.ParamName );
        case ParameterAddress::TargetType::GlobalEffectParam:
            return GetGlobalEffectParameter( editor, paramAddress.SceneName, paramAddress.NodeName, paramAddress.ParamName );
        case ParameterAddress::TargetType::ResourceParam:
            return GetResourceParameter( editor, paramAddress.SceneName, paramAddress.NodeName, paramAddress.PluginName, paramAddress.ParamSubName, paramAddress.ParamName );
        case ParameterAddress::TargetType::LightParam:
            return GetLightParameter( editor, paramAddress.SceneName, paramAddress.Index, paramAddress.ParamName );
        case ParameterAddress::TargetType::NodeLogicParam:
            return GetNodeLogicParameter( editor, paramAddress.SceneName, paramAddress.NodeName, paramAddress.ParamName );
        case ParameterAddress::TargetType::CameraParam:
            return GetCameraParameter( editor, paramAddress.SceneName, paramAddress.Index, paramAddress.ParamName );
        default:
            return nullptr;
    }
}

// ***********************
//
ParameterPtr    GetParameter                ( model::SceneModel * scene, const ParameterAddress & paramAddress )
{
    switch( paramAddress.ParamTargetType )
    {
        case ParameterAddress::TargetType::PluginParam:
            return GetPluginParameter( scene, paramAddress.NodeName, paramAddress.PluginName, paramAddress.ParamName );
        case ParameterAddress::TargetType::GlobalEffectParam:
            return GetGlobalEffectParameter( scene, paramAddress.NodeName, paramAddress.ParamName );
        case ParameterAddress::TargetType::ResourceParam:
            return GetResourceParameter( scene, paramAddress.NodeName, paramAddress.PluginName, paramAddress.ParamSubName, paramAddress.ParamName );
        case ParameterAddress::TargetType::LightParam:
            return GetLightParameter( scene, paramAddress.Index, paramAddress.ParamName );
        case ParameterAddress::TargetType::NodeLogicParam:
            return GetNodeLogicParameter( scene, paramAddress.NodeName, paramAddress.ParamName );
        case ParameterAddress::TargetType::CameraParam:
            return GetCameraParameter( scene, paramAddress.Index, paramAddress.ParamName );
        default:
            return nullptr;
    }
}

// ***********************
//
ParameterPtr    GetPluginParameter          ( BVProjectEditor * editor, const std::string & sceneName, const std::string & nodePath, const std::string & pluginName, const std::string & paramName )
{
    auto scene = editor->GetModelScene( sceneName );
    return GetPluginParameter( scene.get(), nodePath, pluginName, paramName );
}

// ***********************
//
ParameterPtr    GetPluginParameter          ( model::SceneModel * scene, const std::string & nodePath, const std::string & pluginName, const std::string & paramName )
{
    if( scene != nullptr )
    {
        auto node = scene->GetModelSceneEditor()->GetNode( nodePath );
        if( node != nullptr )
        {
            auto plugin = node->GetPlugin( pluginName );
            if( plugin != nullptr )
            {
                auto param = plugin->GetParameter( paramName );
                if( param != nullptr )
                {
                    return param;
                }
            }
        }

		LOG_MESSAGE( SeverityLevel::warning ) << "Parameter: scene [" + scene->GetName() + "], node [" + nodePath + "], plugin [" + pluginName + "] not found";
    }
	else
	{
		LOG_MESSAGE( SeverityLevel::warning ) << "Scene: Cannot find scene.";
	}

	return nullptr;
}

// ***********************
//
ParameterPtr    GetGlobalEffectParameter    ( BVProjectEditor * editor, const std::string & sceneName, const std::string & nodePath, const std::string & paramName )
{
    auto scene = editor->GetModelScene( sceneName );
    return GetGlobalEffectParameter( scene.get(), nodePath, paramName );
}

// ***********************
//
ParameterPtr    GetGlobalEffectParameter    ( model::SceneModel * scene, const std::string & nodePath, const std::string & paramName )
{
    if( scene != nullptr )
    {
        auto node = scene->GetModelSceneEditor()->GetNode( nodePath );
        if( node != nullptr )
        {
            auto effect = node->GetNodeEffect();
            if( effect != nullptr )
            {
                auto param = effect->GetParameter( paramName );
                if( param != nullptr )
                {
                    return param;
                }
            }
        }

		LOG_MESSAGE( SeverityLevel::warning ) << "Parameter: scene [" + scene->GetName() + "], node [" + nodePath + "], param [" + paramName + "] not found";
    }
	else
	{
		LOG_MESSAGE( SeverityLevel::warning ) << "Scene: Cannot find scene.";
	}

	return nullptr;
}

// ***********************
//
ParameterPtr    GetResourceParameter        ( BVProjectEditor * editor, const std::string & sceneName, const std::string & nodePath, const std::string & pluginName, const std::string & textureName, const std::string & paramName )
{
    auto scene = editor->GetModelScene( sceneName );
    return GetResourceParameter( scene.get(), nodePath, pluginName, textureName, paramName );
}

// ***********************
//
ParameterPtr    GetResourceParameter        ( model::SceneModel * scene, const std::string & nodePath, const std::string & pluginName, const std::string & textureName, const std::string & paramName )
{
    if( scene != nullptr )
    {
        auto node = scene->GetModelSceneEditor()->GetNode( nodePath );
        if( node != nullptr )
        {
            auto plugin = node->GetPlugin( pluginName );
            if( plugin != nullptr )
            {
                auto resourceModel = plugin->GetResourceStateModel( textureName );
                if( resourceModel != nullptr )
                {
                    auto param = resourceModel->GetParameter( paramName );
                    if( param != nullptr )
                    {
                        return param;
                    }
                }
            }
        }

		LOG_MESSAGE( SeverityLevel::warning ) << "Parameter: scene [" + scene->GetName() + "], node [" + nodePath + "], plugin [" + pluginName + "], texture [" + textureName + "], param [" + paramName + "] not found";
	}
	else
	{
		LOG_MESSAGE( SeverityLevel::warning ) << "Scene: Cannot find scene.";
	}
    
	return nullptr;
}

// ***********************
//
ParameterPtr    GetLightParameter           ( BVProjectEditor * editor, const std::string & sceneName, UInt32 lightIndex, const std::string & paramName )
{
    auto scene = editor->GetModelScene( sceneName );
    return GetLightParameter( scene.get(), lightIndex, paramName );
}

// ***********************
//
ParameterPtr    GetLightParameter           ( model::SceneModel * scene, UInt32 lightIndex, const std::string & paramName )
{
    if( scene != nullptr )
    {
        auto light = scene->GetLight( lightIndex );
        if( light )
        {
            auto param = light->GetParameter( paramName );
            if( param != nullptr )
            {
                return param;
            }
        }

		LOG_MESSAGE( SeverityLevel::warning ) << "Parameter: scene [" + scene->GetName() + "], index [" + Convert::T2String( lightIndex ) + "], param [" + paramName + "] not found";
	}
	else
	{
		LOG_MESSAGE( SeverityLevel::warning ) << "Scene: Cannot find scene.";
	}

    return nullptr;
}

// ***********************
//
ParameterPtr    GetNodeLogicParameter       ( BVProjectEditor * editor, const std::string & sceneName, const std::string & nodePath, const std::string & paramName )
{
    auto scene = editor->GetModelScene( sceneName );
    return GetNodeLogicParameter( scene.get(), nodePath, paramName );
}

// ***********************
//
ParameterPtr    GetNodeLogicParameter       ( model::SceneModel * scene, const std::string & nodePath, const std::string & paramName )
{
    if( scene != nullptr )
    {
        auto node = scene->GetModelSceneEditor()->GetNode( nodePath );
        if( node != nullptr )
        {
            auto logic = node->GetLogic();
            if( logic )
            {
                auto param = logic->GetParameter( paramName );
                if( param != nullptr )
                {
                    return param;
                }
            }
		}

		LOG_MESSAGE( SeverityLevel::warning ) << "Parameter: scene [" + scene->GetName() + "], node [" + nodePath + "],  param [" + paramName + "] not found";
    }
	else
	{
		LOG_MESSAGE( SeverityLevel::warning ) << "Scene: Cannot find scene.";
	}
    
    return nullptr;
}

// ***********************
//
ParameterPtr    GetCameraParameter          ( BVProjectEditor * editor, const std::string & sceneName, UInt32 cameraIndex, const std::string & paramName )
{
    auto scene = editor->GetModelScene( sceneName );
    return GetCameraParameter( scene.get(), cameraIndex, paramName );
}

// ***********************
//
ParameterPtr    GetCameraParameter          ( model::SceneModel * scene, UInt32 cameraIndex, const std::string & paramName )
{
    if( scene != nullptr )
    {
        auto camera = scene->GetCamerasLogic().GetCamera( cameraIndex );
        if( camera )
        {
            auto param = camera->GetParameter( paramName );
            if( param != nullptr )
            {
                return param;
            }
        }

		LOG_MESSAGE( SeverityLevel::warning ) << "Parameter: scene [" + scene->GetName() + "], index [" + Convert::T2String( cameraIndex ) + "], param [" + paramName + "] not found";
	}
	else
	{
		LOG_MESSAGE( SeverityLevel::warning ) << "Scene: Cannot find scene.";
	}

    return nullptr;
}

} //bv
