#pragma once

#include "Engine/Editors/EditorVariables/ParametersDescriptors/EndUserParamsLogic.h"
#include "Engine/Models/SceneModel.h"

using namespace bv;


// ***********************
//
inline void             AddParamDescriptors     ( EndUserParamsLogic & descLogic, model::SceneModelPtr scene )
{
    EndUserParamDescriptor desc;
    desc.SetName( "Desc" );
    desc.SetDescription( "This is descriptor" );


    // ***********************
    //
    ParameterAddress address;
    address.SceneName = scene->GetName();
    address.NodeName = "root/ColoredRect";
    address.ParamTargetType = ParameterAddress::PluginParam;
    address.PluginName = "solid color";
    address.ParamName = "color";

    CHECK( descLogic.AddDescriptor( ParameterAddress( address ), EndUserParamDescriptor( desc ) ) );
    REQUIRE CHECK( descLogic.GetDescriptor( address ) != nullptr );

    // ***********************
    //
    address.SceneName = scene->GetName();
    address.NodeName = "root/ColoredRect";
    address.ParamTargetType = ParameterAddress::PluginParam;
    address.PluginName = "rectangle";
    address.ParamName = "width";

    CHECK( descLogic.AddDescriptor( ParameterAddress( address ), EndUserParamDescriptor( desc ) ) );
    REQUIRE CHECK( descLogic.GetDescriptor( address ) != nullptr );

    // ***********************
    //
    address.SceneName = scene->GetName();
    address.NodeName = "root/TexturedRect";
    address.ParamTargetType = ParameterAddress::ResourceParam;
    address.PluginName = "texture";
    address.ParamName = "wrapModeX";
    address.ParamSubName = "Tex0";

    CHECK( descLogic.AddDescriptor( ParameterAddress( address ), EndUserParamDescriptor( desc ) ) );
    REQUIRE CHECK( descLogic.GetDescriptor( address ) != nullptr );


    // ***********************
    // Add Light param descriptor
    address.SceneName = scene->GetName();
    address.NodeName = "";
    address.ParamTargetType = ParameterAddress::LightParam;
    address.PluginName = "";
    address.ParamName = "color";
    address.ParamSubName = "";
    address.Index = 0;

    EndUserParamDescriptor lightDesc;
    lightDesc.SetName( "Light0" );
    lightDesc.SetDescription( "This is descriptor 0" );

    CHECK( descLogic.AddDescriptor( ParameterAddress( address ), EndUserParamDescriptor( lightDesc ) ) );
    REQUIRE CHECK( descLogic.GetDescriptor( address ) != nullptr );

    // ***********************
    // Add Light param descriptor
    address.SceneName = scene->GetName();
    address.NodeName = "";
    address.ParamTargetType = ParameterAddress::LightParam;
    address.PluginName = "";
    address.ParamName = "color";
    address.ParamSubName = "";
    address.Index = 1;

    lightDesc.SetName( "Light1" );
    lightDesc.SetDescription( "This is descriptor 1" );

    CHECK( descLogic.AddDescriptor( ParameterAddress( address ), EndUserParamDescriptor( lightDesc ) ) );
    REQUIRE CHECK( descLogic.GetDescriptor( address ) != nullptr );


    // ***********************
    // Add Camera param descriptor
    address.SceneName = scene->GetName();
    address.NodeName = "";
    address.ParamTargetType = ParameterAddress::CameraParam;
    address.PluginName = "";
    address.ParamName = "IsPerspective";
    address.ParamSubName = "";
    address.Index = 1;

    EndUserParamDescriptor cameraDesc;
    cameraDesc.SetName( "Camera1" );
    cameraDesc.SetDescription( "This is descriptor 1" );

    CHECK( descLogic.AddDescriptor( ParameterAddress( address ), EndUserParamDescriptor( cameraDesc ) ) );
    REQUIRE CHECK( descLogic.GetDescriptor( address ) != nullptr );

    // ***********************
    // Add Camera param descriptor
    address.SceneName = scene->GetName();
    address.NodeName = "";
    address.ParamTargetType = ParameterAddress::CameraParam;
    address.PluginName = "";
    address.ParamName = "IsPerspective";
    address.ParamSubName = "";
    address.Index = 0;

    cameraDesc.SetName( "Camera0" );
    cameraDesc.SetDescription( "This is descriptor 0" );

    CHECK( descLogic.AddDescriptor( ParameterAddress( address ), EndUserParamDescriptor( cameraDesc ) ) );
    REQUIRE CHECK( descLogic.GetDescriptor( address ) != nullptr );


    // ***********************
    // Add Logic param descriptor
    address.SceneName = scene->GetName();
    address.NodeName = "root/TexturedRect";
    address.ParamTargetType = ParameterAddress::NodeLogicParam;
    address.PluginName = "";
    address.ParamName = "OffsetX";
    address.ParamSubName = "";
    address.Index = 0;

    CHECK( descLogic.AddDescriptor( ParameterAddress( address ), EndUserParamDescriptor( desc ) ) );
    REQUIRE CHECK( descLogic.GetDescriptor( address ) != nullptr );


    // ***********************
    // Add Effect param descriptor
    address.SceneName = scene->GetName();
    address.NodeName = "root/ColoredRect";
    address.ParamTargetType = ParameterAddress::GlobalEffectParam;
    address.PluginName = "";
    address.ParamName = "alpha";
    address.ParamSubName = "";
    address.Index = 0;

    CHECK( descLogic.AddDescriptor( ParameterAddress( address ), EndUserParamDescriptor( desc ) ) );
    REQUIRE CHECK( descLogic.GetDescriptor( address ) != nullptr );
}


