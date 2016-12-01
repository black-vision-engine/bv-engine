#include "stdafx.h"

#include "EventHandlerHelpers.h"
#include "Engine/Models/BVProjectEditor.h"

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
            return GetLightParameter( editor, paramAddress.SceneName, paramAddress.LightParam, paramAddress.ParamName );
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
ParameterPtr    GetPluginParameter          ( BVProjectEditor * editor, const std::string & sceneName, const std::string & nodePath, const std::string & pluginName, const std::string & paramName )
{
    auto node = editor->GetNode( sceneName, nodePath );
    if( node == nullptr )
    {
        return nullptr;
    }

    auto plugin = node->GetPlugin( pluginName );
    if( plugin == nullptr )
    {
        LOG_MESSAGE( SeverityLevel::warning ) << "Parameter event handler: scene [" + sceneName + "], node [" + nodePath + "], plugin [" + pluginName + "] not found";
        return nullptr;
    }

    auto param = plugin->GetParameter( paramName );
    if( param == nullptr )
    {
        LOG_MESSAGE( SeverityLevel::warning ) << "Parameter event handler: scene [" + sceneName + "], node [" + nodePath + "], plugin [" + pluginName + "], param [" + paramName + "] not found";
        return nullptr;
    }
    return param;
}

// ***********************
//
ParameterPtr    GetGlobalEffectParameter    ( BVProjectEditor * editor, const std::string & sceneName, const std::string & nodePath, const std::string & paramName )
{
    auto node = editor->GetNode( sceneName, nodePath );
    if( node == nullptr )
    {
        return nullptr;
    }

    auto effect = node->GetNodeEffect();
    if( effect == nullptr )
    {
        LOG_MESSAGE( SeverityLevel::warning ) << "Parameter event handler: scene [" + sceneName + "], node [" + nodePath + "], effect not found";
        return nullptr;
    }

    auto param = effect->GetParameter( paramName );
    if( param == nullptr )
    {
        LOG_MESSAGE( SeverityLevel::warning ) << "Parameter event handler: scene [" + sceneName + "], node [" + nodePath + "], param [" + paramName + "] not found";
        return nullptr;
    }
    return param;
}

// ***********************
//
ParameterPtr    GetResourceParameter        ( BVProjectEditor * editor, const std::string & sceneName, const std::string & nodePath, const std::string & pluginName, const std::string & textureName, const std::string & paramName )
{
    auto node = editor->GetNode( sceneName, nodePath );
    if( node == nullptr )
    {
        return nullptr;
    }

    auto plugin = node->GetPlugin( pluginName );
    if( plugin == nullptr )
    {
        LOG_MESSAGE( SeverityLevel::warning ) << "Parameter event handler: scene [" + sceneName + "], node [" + nodePath + "], plugin [" + pluginName + "] not found";
        return nullptr;
    }

    auto resourceModel = plugin->GetResourceStateModel( textureName );
    if( resourceModel == nullptr )
    {
        LOG_MESSAGE( SeverityLevel::warning ) << "Parameter event handler: scene [" + sceneName + "], node [" + nodePath + "], plugin [" + pluginName + "], texture [" + textureName + " not found";
        return nullptr;
    }

    return resourceModel->GetParameter( paramName );
}

// ***********************
//
ParameterPtr    GetLightParameter           ( BVProjectEditor * editor, const std::string & sceneName, UInt32 lightIndex, const std::string & paramName )
{
    auto scene = editor->GetModelScene( sceneName );
    if( scene )
    {
        auto light = scene->GetLight( lightIndex );
        if( light )
        {
            return light->GetParameter( paramName );
        }
    }

    LOG_MESSAGE( SeverityLevel::warning ) << "Parameter event handler: not found";
    return nullptr;
}

// ***********************
//
ParameterPtr    GetNodeLogicParameter       ( BVProjectEditor * editor, const std::string & sceneName, const std::string & nodePath, const std::string & paramName )
{
    auto node = editor->GetNode( sceneName, nodePath );
    if( node == nullptr )
    {
        return nullptr;
    }

    auto logic = node->GetLogic();
    if( logic )
    {
        logic->GetParameter( paramName );
    }

    LOG_MESSAGE( SeverityLevel::warning ) << "Node logic parameter not found";
    return nullptr;
}

// ***********************
//
ParameterPtr    GetCameraParameter          ( BVProjectEditor * editor, const std::string & sceneName, UInt32 cameraIndex, const std::string & paramName )
{
    auto scene = editor->GetModelScene( sceneName );
    if( scene )
    {
        auto camera = scene->GetCamerasLogic().GetCamera( cameraIndex );
        if( camera )
        {
            return camera->GetParameter( paramName );
        }
    }

    LOG_MESSAGE( SeverityLevel::warning ) << "Camera parameter not found";
    return nullptr;
}


} //bv
