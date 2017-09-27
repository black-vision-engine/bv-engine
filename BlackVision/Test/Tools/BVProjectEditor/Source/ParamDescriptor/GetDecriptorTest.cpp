#include "Framework/FrameworkTest.h"

#include "Engine/Editors/EditorVariables/ParametersDescriptors/EndUserParamsLogic.h"
#include "Engine/Models/SceneModel.h"

#include "Utils/ParamDescUtils.h"
#include "Utils/TwoScenesFixture.h"


using namespace bv;



// ***********************
// Gets descriptor for param using almost good address.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( BVProjectEditor.ParameterDescriptor, EndUserParamsLogic_GetDescBadSceneAddress )
{
    auto editor = GetProjectEditor();

    auto scene = CreateSceneForParamDesc( editor, "Scene" );
    EndUserParamsLogic paramsLogic( scene.get() );

    AddParamDescriptors( paramsLogic, scene );

    // ***********************
    //
    ParameterAddress address;
    address.SceneName = "BadName";
    address.NodeName = "root/ColoredRect";
    address.ParamTargetType = ParameterAddress::PluginParam;
    address.PluginName = "solid color";
    address.ParamName = "color";

    CHECK( paramsLogic.GetDescriptor( address ) == nullptr );

    // ***********************
    //
    address.SceneName = "BadName";
    address.NodeName = "root/TexturedRect";
    address.ParamTargetType = ParameterAddress::ResourceParam;
    address.PluginName = "texture";
    address.ParamName = "wrapModeX";
    address.ParamSubName = "Tex0";

    CHECK( paramsLogic.GetDescriptor( address ) == nullptr );


    // ***********************
    // 
    address.SceneName = "BadName";
    address.NodeName = "";
    address.ParamTargetType = ParameterAddress::LightParam;
    address.PluginName = "";
    address.ParamName = "color";
    address.ParamSubName = "";
    address.Index = 0;

    CHECK( paramsLogic.GetDescriptor( address ) == nullptr );


    // ***********************
    // 
    address.SceneName = "BadName";
    address.NodeName = "";
    address.ParamTargetType = ParameterAddress::CameraParam;
    address.PluginName = "";
    address.ParamName = "IsPerspective";
    address.ParamSubName = "";
    address.Index = 1;

    CHECK( paramsLogic.GetDescriptor( address ) == nullptr );


    // ***********************
    //
    address.SceneName = "";
    address.NodeName = "root/TexturedRect";
    address.ParamTargetType = ParameterAddress::NodeLogicParam;
    address.PluginName = "";
    address.ParamName = "OffsetX";
    address.ParamSubName = "";
    address.Index = 0;

    CHECK( paramsLogic.GetDescriptor( address ) == nullptr );


    // ***********************
    // Add Effect param descriptor
    address.SceneName = "BadName";
    address.NodeName = "root/ColoredRect";
    address.ParamTargetType = ParameterAddress::GlobalEffectParam;
    address.PluginName = "";
    address.ParamName = "alpha";
    address.ParamSubName = "";
    address.Index = 0;

    CHECK( paramsLogic.GetDescriptor( address ) == nullptr );
}

// ***********************
// Gets descriptor for param using almost good address.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( BVProjectEditor.ParameterDescriptor, EndUserParamsLogic_GetDescCameraIndexOutOfRange )
{
    auto editor = GetProjectEditor();

    auto scene = CreateSceneForParamDesc( editor, "Scene" );
    EndUserParamsLogic paramsLogic( scene.get() );

    AddParamDescriptors( paramsLogic, scene );

    // ***********************
    //
    ParameterAddress address;
    address.SceneName = scene->GetName();
    address.NodeName = "";
    address.ParamTargetType = ParameterAddress::LightParam;
    address.PluginName = "";
    address.ParamName = "color";
    address.ParamSubName = "";
    address.Index = 2;

    CHECK( paramsLogic.GetDescriptor( address ) == nullptr );

    // ***********************
    //
    address.SceneName = scene->GetName();
    address.NodeName = "";
    address.ParamTargetType = ParameterAddress::LightParam;
    address.PluginName = "";
    address.ParamName = "color";
    address.ParamSubName = "";
    address.Index = 10;

    CHECK( paramsLogic.GetDescriptor( address ) == nullptr );
}

// ***********************
// Gets descriptor for param using almost good address.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( BVProjectEditor.ParameterDescriptor, EndUserParamsLogic_GetDescLightIndexOutOfRange )
{
    auto editor = GetProjectEditor();

    auto scene = CreateSceneForParamDesc( editor, "Scene" );
    EndUserParamsLogic paramsLogic( scene.get() );

    AddParamDescriptors( paramsLogic, scene );

    // ***********************
    //
    ParameterAddress address;
    address.SceneName = scene->GetName();
    address.NodeName = "";
    address.ParamTargetType = ParameterAddress::LightParam;
    address.PluginName = "";
    address.ParamName = "color";
    address.ParamSubName = "";
    address.Index = 2;

    CHECK( paramsLogic.GetDescriptor( address ) == nullptr );

    // ***********************
    //
    address.SceneName = scene->GetName();
    address.NodeName = "";
    address.ParamTargetType = ParameterAddress::LightParam;
    address.PluginName = "";
    address.ParamName = "color";
    address.ParamSubName = "";
    address.Index = 2;

    CHECK( paramsLogic.GetDescriptor( address ) == nullptr );
}


