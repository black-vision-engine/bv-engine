#include "Framework/FrameworkTest.h"

#include "Engine/Editors/EditorVariables/ParametersDescriptors/EndUserParamsLogic.h"
#include "Engine/Models/SceneModel.h"

#include "Utils/TwoScenesFixture.h"


using namespace bv;


// ***********************
// Tests adding and removing parameter descriptor.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( BVProjectEditor.ParameterDescriptor, EndUserParamsLogic_AddRemoveParamDesc )
{
    auto scene = CreateSceneForParamDesc( GetProjectEditor(), "Scene" );
    EndUserParamsLogic paramsLogic( scene.get() );

    EndUserParamDescriptor desc;
    desc.SetName( "PluginDesc" );
    desc.SetDescription( "This is descriptor" );


    // ***********************
    // Add Plugin parameter descriptor.
    ParameterAddress addressPlug;
    addressPlug.SceneName = scene->GetName();
    addressPlug.NodeName = "root/ColoredRect";
    addressPlug.ParamTargetType = ParameterAddress::PluginParam;
    addressPlug.PluginName = "solid color";
    addressPlug.ParamName = "color";

    CHECK( paramsLogic.AddDescriptor( ParameterAddress( addressPlug ), EndUserParamDescriptor( desc ) ) );

    auto returnDesc = paramsLogic.GetDescriptor( addressPlug );
    REQUIRE CHECK( returnDesc != nullptr );

    CHECK( returnDesc->GetName() == "PluginDesc" );
    CHECK( returnDesc->GetDescription() == "This is descriptor" );

    // ***********************
    // Add Resource param descriptor
    ParameterAddress addressRes;
    addressRes.SceneName = scene->GetName();
    addressRes.NodeName = "root/TexturedRect";
    addressRes.ParamTargetType = ParameterAddress::ResourceParam;
    addressRes.PluginName = "texture";
    addressRes.ParamName = "wrapModeX";
    addressRes.ParamSubName = "Tex0";

    desc.SetName( "ResourceDesc" );

    // Add
    CHECK( paramsLogic.AddDescriptor( ParameterAddress( addressRes ), EndUserParamDescriptor( desc ) ) );

    returnDesc = paramsLogic.GetDescriptor( addressRes );
    REQUIRE CHECK( returnDesc != nullptr );

    CHECK( returnDesc->GetName() == "ResourceDesc" );
    CHECK( returnDesc->GetDescription() == "This is descriptor" );

    // ***********************
    // Add Light param descriptor
    ParameterAddress addressLight;
    addressLight.SceneName = scene->GetName();
    addressLight.NodeName = "";
    addressLight.ParamTargetType = ParameterAddress::LightParam;
    addressLight.PluginName = "";
    addressLight.ParamName = "color";
    addressLight.ParamSubName = "";
    addressLight.Index = 1;

    desc.SetName( "LightDesc" );

    // Add
    CHECK( paramsLogic.AddDescriptor( ParameterAddress( addressLight ), EndUserParamDescriptor( desc ) ) );

    returnDesc = paramsLogic.GetDescriptor( addressLight );
    REQUIRE CHECK( returnDesc != nullptr );

    CHECK( returnDesc->GetName() == "LightDesc" );
    CHECK( returnDesc->GetDescription() == "This is descriptor" );

    // ***********************
    // Add Camera param descriptor
    ParameterAddress addressCamera;
    addressCamera.SceneName = scene->GetName();
    addressCamera.NodeName = "";
    addressCamera.ParamTargetType = ParameterAddress::CameraParam;
    addressCamera.PluginName = "";
    addressCamera.ParamName = "IsPerspective";
    addressCamera.ParamSubName = "";
    addressCamera.Index = 1;

    desc.SetName( "CameraDesc" );

    // Add
    CHECK( paramsLogic.AddDescriptor( ParameterAddress( addressCamera ), EndUserParamDescriptor( desc ) ) );

    returnDesc = paramsLogic.GetDescriptor( addressCamera );
    REQUIRE CHECK( returnDesc != nullptr );

    CHECK( returnDesc->GetName() == "CameraDesc" );
    CHECK( returnDesc->GetDescription() == "This is descriptor" );

    // ***********************
    // Add Logic param descriptor
    ParameterAddress addressLogic;
    addressLogic.SceneName = scene->GetName();
    addressLogic.NodeName = "root/TexturedRect";
    addressLogic.ParamTargetType = ParameterAddress::NodeLogicParam;
    addressLogic.PluginName = "";
    addressLogic.ParamName = "OffsetX";
    addressLogic.ParamSubName = "";
    addressLogic.Index = 0;

    desc.SetName( "LogicDesc" );

    // Add
    CHECK( paramsLogic.AddDescriptor( ParameterAddress( addressLogic ), EndUserParamDescriptor( desc ) ) );

    returnDesc = paramsLogic.GetDescriptor( addressLogic );
    REQUIRE CHECK( returnDesc != nullptr );

    CHECK( returnDesc->GetName() == "LogicDesc" );
    CHECK( returnDesc->GetDescription() == "This is descriptor" );

    // ***********************
    // Add Effect param descriptor
    ParameterAddress addressEffect;
    addressEffect.SceneName = scene->GetName();
    addressEffect.NodeName = "root/ColoredRect";
    addressEffect.ParamTargetType = ParameterAddress::GlobalEffectParam;
    addressEffect.PluginName = "";
    addressEffect.ParamName = "alpha";
    addressEffect.ParamSubName = "";
    addressEffect.Index = 0;

    desc.SetName( "EffectDesc" );

    // Add
    CHECK( paramsLogic.AddDescriptor( ParameterAddress( addressEffect ), EndUserParamDescriptor( desc ) ) );

    returnDesc = paramsLogic.GetDescriptor( addressEffect );
    REQUIRE CHECK( returnDesc != nullptr );

    CHECK( returnDesc->GetName() == "EffectDesc" );
    CHECK( returnDesc->GetDescription() == "This is descriptor" );



    // ***********************
    // Remove phase
    CHECK( paramsLogic.RemoveDescriptor( addressLight ) );
    CHECK( paramsLogic.RemoveDescriptor( addressCamera ) );
    CHECK( paramsLogic.RemoveDescriptor( addressEffect ) );
    CHECK( paramsLogic.RemoveDescriptor( addressLogic ) );
    CHECK( paramsLogic.RemoveDescriptor( addressPlug ) );
    CHECK( paramsLogic.RemoveDescriptor( addressRes ) );

    
    CHECK( paramsLogic.GetDescriptor( addressLight ) == nullptr );
    CHECK( paramsLogic.GetDescriptor( addressCamera ) == nullptr );
    CHECK( paramsLogic.GetDescriptor( addressEffect ) == nullptr );
    CHECK( paramsLogic.GetDescriptor( addressLogic ) == nullptr );
    CHECK( paramsLogic.GetDescriptor( addressPlug ) == nullptr );
    CHECK( paramsLogic.GetDescriptor( addressRes ) == nullptr );
}

// ***********************
//
void                AddParamDescriptors     ( EndUserParamsLogic & descLogic, model::SceneModelPtr scene )
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


// ***********************
// Tests adding and removing parameter descriptor.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( BVProjectEditor.ParameterDescriptor, EndUserParamsLogic_RemovingNodeDeletesDescriptor )
{
    auto editor = GetProjectEditor();

    auto scene = CreateSceneForParamDesc( editor, "Scene" );
    EndUserParamsLogic paramsLogic( scene.get() );

    AddParamDescriptors( paramsLogic, scene );

    CHECK( editor->DeleteChildNode( "Scene", "root/ColoredRect", false ) );

    // ***********************
    // Check if descriptors were deleted.
    ParameterAddress address;
    address.SceneName = scene->GetName();
    address.NodeName = "root/ColoredRect";
    address.ParamTargetType = ParameterAddress::PluginParam;
    address.PluginName = "solid color";
    address.ParamName = "color";

    CHECK( paramsLogic.GetDescriptor( address ) == nullptr );

    address.ParamTargetType = ParameterAddress::GlobalEffectParam;
    address.PluginName = "";
    address.ParamName = "alpha";
    address.ParamSubName = "";
    address.Index = 0;

    CHECK( paramsLogic.GetDescriptor( address ) == nullptr );

    address.ParamTargetType = ParameterAddress::PluginParam;
    address.PluginName = "rectangle";
    address.ParamName = "width";

    CHECK( paramsLogic.GetDescriptor( address ) == nullptr );

    // ***********************
    // Remove second node
    CHECK( editor->DeleteChildNode( "Scene", "root/TexturedRect", false ) );

    // ***********************
    // Check if descriptors were deleted.
    address.NodeName = "root/TexturedRect";
    address.ParamTargetType = ParameterAddress::NodeLogicParam;
    address.PluginName = "";
    address.ParamName = "OffsetX";
    address.ParamSubName = "";
    address.Index = 0;

    CHECK( paramsLogic.GetDescriptor( address ) == nullptr );

    address.SceneName = scene->GetName();
    address.NodeName = "root/TexturedRect";
    address.ParamTargetType = ParameterAddress::ResourceParam;
    address.PluginName = "texture";
    address.ParamName = "wrapModeX";
    address.ParamSubName = "Tex0";

    CHECK( paramsLogic.GetDescriptor( address ) == nullptr );
}

// ***********************
// Deleting camera should delete camera param descriptor. If we delete camera under index 0, camera from index 1 gets index 0.
// In this example we have to cameras, each have the same parameter pinned. After removing first camera, we access
// parameter using the same address and we should get descriptors of the second camera parameter.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( BVProjectEditor.ParameterDescriptor, EndUserParamsLogic_RemovingCameraDeletesDescriptor )
{
    auto editor = GetProjectEditor();

    auto scene = CreateSceneForParamDesc( editor, "Scene" );
    EndUserParamsLogic paramsLogic( scene.get() );

    AddParamDescriptors( paramsLogic, scene );

    // ***********************
    // Remove camera
    editor->RemoveCamera( scene, 0 );

    // ***********************
    // Check descirptores
    ParameterAddress address;
    address.SceneName = scene->GetName();
    address.NodeName = "";
    address.ParamTargetType = ParameterAddress::CameraParam;
    address.PluginName = "";
    address.ParamName = "IsPerspective";
    address.ParamSubName = "";
    address.Index = 0;

    auto desc = paramsLogic.GetDescriptor( address );
    REQUIRE CHECK( desc != nullptr );                   // Camera from index 1 replaces camera under index 0.
    
    CHECK( desc->GetName() == "Camera1" );

    // ***********************
    // There's no camera under index 1 now.
    address.SceneName = scene->GetName();
    address.NodeName = "";
    address.ParamTargetType = ParameterAddress::CameraParam;
    address.PluginName = "";
    address.ParamName = "IsPerspective";
    address.ParamSubName = "";
    address.Index = 1;

    desc = paramsLogic.GetDescriptor( address );
    REQUIRE CHECK( desc == nullptr );
}

// ***********************
// Deleting light should delete camera param descriptor. If we delete camera under index 0, camera from index 1 gets index 0.
// In this example we have to lights, each have the same parameter pinned. After removing first light, we access
// parameter using the same address and we should get descriptors of the second light parameter.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( BVProjectEditor.ParameterDescriptor, EndUserParamsLogic_RemovingLightDeletesDescriptor )
{
    auto editor = GetProjectEditor();

    auto scene = CreateSceneForParamDesc( editor, "Scene" );
    EndUserParamsLogic paramsLogic( scene.get() );

    AddParamDescriptors( paramsLogic, scene );

    // ***********************
    // Remove camera
    editor->RemoveLight( scene, 0 );

    // ***********************
    // Check descirptores
    ParameterAddress address;
    address.SceneName = scene->GetName();
    address.NodeName = "";
    address.ParamTargetType = ParameterAddress::LightParam;
    address.PluginName = "";
    address.ParamName = "color";
    address.ParamSubName = "";
    address.Index = 0;

    auto desc = paramsLogic.GetDescriptor( address );
    REQUIRE CHECK( desc != nullptr );                   // Camera from index 1 replaces camera under index 0.

    CHECK( desc->GetName() == "Light1" );

    // ***********************
    // There's no camera under index 1 now.
    address.SceneName = scene->GetName();
    address.NodeName = "";
    address.ParamTargetType = ParameterAddress::LightParam;
    address.PluginName = "";
    address.ParamName = "color";
    address.ParamSubName = "";
    address.Index = 1;

    desc = paramsLogic.GetDescriptor( address );
    REQUIRE CHECK( desc == nullptr );
}

// ***********************
// Move node whose parameters are registered and check if we can access descriptor with new address.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( BVProjectEditor.ParameterDescriptor, EndUserParamsLogic_MovingNodeChangesDescsAddress )
{
    auto editor = GetProjectEditor();

    auto scene = CreateSceneForParamDesc( editor, "Scene" );
    EndUserParamsLogic paramsLogic( scene.get() );

    AddParamDescriptors( paramsLogic, scene );

    // ***********************
    // Move node
    REQUIRE CHECK( editor->MoveNode( scene->GetName(), "root/TexturedRect", 0, scene->GetName(), "root/ColoredRect" ) );

    // ***********************
    // Check old addresses. Descriptor shouldn't be accessed.
    ParameterAddress address;
    address.SceneName = scene->GetName();
    address.NodeName = "root/ColoredRect";
    address.ParamTargetType = ParameterAddress::PluginParam;
    address.PluginName = "solid color";
    address.ParamName = "color";

    CHECK( paramsLogic.GetDescriptor( address ) == nullptr );

    address.ParamTargetType = ParameterAddress::GlobalEffectParam;
    address.NodeName = "root/ColoredRect";
    address.PluginName = "";
    address.ParamName = "alpha";
    address.ParamSubName = "";
    address.Index = 0;

    CHECK( paramsLogic.GetDescriptor( address ) == nullptr );

    address.NodeName = "root/ColoredRect";
    address.ParamTargetType = ParameterAddress::PluginParam;
    address.PluginName = "rectangle";
    address.ParamName = "width";

    CHECK( paramsLogic.GetDescriptor( address ) == nullptr );

    // ***********************
    // Check new adresses for existance of descriptos.
    address.SceneName = scene->GetName();
    address.NodeName = "root/TexturedRect/ColoredRect";
    address.ParamTargetType = ParameterAddress::PluginParam;
    address.PluginName = "solid color";
    address.ParamName = "color";

    CHECK( paramsLogic.GetDescriptor( address ) != nullptr );

    address.ParamTargetType = ParameterAddress::GlobalEffectParam;
    address.NodeName = "root/TexturedRect/ColoredRect";
    address.PluginName = "";
    address.ParamName = "alpha";
    address.ParamSubName = "";

    CHECK( paramsLogic.GetDescriptor( address ) != nullptr );

    address.NodeName = "root/TexturedRect/ColoredRect";
    address.ParamTargetType = ParameterAddress::PluginParam;
    address.PluginName = "rectangle";
    address.ParamName = "width";

    CHECK( paramsLogic.GetDescriptor( address ) != nullptr );

    // ***********************
    // Move TexturedNode (it holds logic and resource param descriptors)
    REQUIRE CHECK( editor->MoveNode( scene->GetName(), "root/Group1", 0, scene->GetName(), "root/TexturedRect" ) );

    // ***********************
    // Check old addresses. Descriptor shouldn't be accessed.
    address.NodeName = "root/TexturedRect";
    address.ParamTargetType = ParameterAddress::NodeLogicParam;
    address.PluginName = "";
    address.ParamName = "OffsetX";
    address.ParamSubName = "";
    address.Index = 0;

    CHECK( paramsLogic.GetDescriptor( address ) == nullptr );

    address.SceneName = scene->GetName();
    address.NodeName = "root/TexturedRect";
    address.ParamTargetType = ParameterAddress::ResourceParam;
    address.PluginName = "texture";
    address.ParamName = "wrapModeX";
    address.ParamSubName = "Tex0";

    CHECK( paramsLogic.GetDescriptor( address ) == nullptr );

    // ***********************
    // Check new adresses for existance of descriptos.
    address.NodeName = "root/Group1/TexturedRect";
    address.ParamTargetType = ParameterAddress::NodeLogicParam;
    address.PluginName = "";
    address.ParamName = "OffsetX";
    address.ParamSubName = "";
    address.Index = 0;

    CHECK( paramsLogic.GetDescriptor( address ) != nullptr );

    address.SceneName = scene->GetName();
    address.NodeName = "root/Group1/TexturedRect";
    address.ParamTargetType = ParameterAddress::ResourceParam;
    address.PluginName = "texture";
    address.ParamName = "wrapModeX";
    address.ParamSubName = "Tex0";

    CHECK( paramsLogic.GetDescriptor( address ) != nullptr );
}

// ***********************
// Move plugin whose parameters are registered and check if we can access descriptor with new address.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( BVProjectEditor.ParameterDescriptor, EndUserParamsLogic_MovingPluginChangesDescsAddress )
{
    auto editor = GetProjectEditor();

    auto scene = CreateSceneForParamDesc( editor, "Scene" );
    EndUserParamsLogic paramsLogic( scene.get() );

    AddParamDescriptors( paramsLogic, scene );

    // ***********************
    // Move plugin
    REQUIRE CHECK( editor->MovePlugin( scene->GetName(), "root/Group1", 1, scene->GetName(), "root/ColoredRect", "solid color" ) );

    // ***********************
    // Check old addresses. Descriptor shouldn't be accessed.
    ParameterAddress address;
    address.SceneName = scene->GetName();
    address.NodeName = "root/ColoredRect";
    address.ParamTargetType = ParameterAddress::PluginParam;
    address.PluginName = "solid color";
    address.ParamName = "color";

    CHECK( paramsLogic.GetDescriptor( address ) == nullptr );

    address.NodeName = "root/ColoredRect";
    address.ParamTargetType = ParameterAddress::PluginParam;
    address.PluginName = "rectangle";
    address.ParamName = "width";

    CHECK( paramsLogic.GetDescriptor( address ) == nullptr );

}