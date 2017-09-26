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
    address.Index = 1;

    CHECK( descLogic.AddDescriptor( ParameterAddress( address ), EndUserParamDescriptor( desc ) ) );
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

    CHECK( descLogic.AddDescriptor( ParameterAddress( address ), EndUserParamDescriptor( desc ) ) );
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

}

