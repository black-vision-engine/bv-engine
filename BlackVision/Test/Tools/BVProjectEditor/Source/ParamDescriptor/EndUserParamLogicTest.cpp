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






