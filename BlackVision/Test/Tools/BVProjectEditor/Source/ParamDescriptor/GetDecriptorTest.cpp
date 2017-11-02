#include "Framework/FrameworkTest.h"

#include "Engine/Editors/EditorVariables/ParametersDescriptors/EndUserParamsLogic.h"
#include "Engine/Models/SceneModel.h"

#include "Utils/ParamDescUtils.h"
#include "Utils/TwoScenesFixture.h"


using namespace bv;



// ***********************
// Gets descriptor for param using almost good address.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( BVProjectEditor_ParameterDescriptor, EndUserParamsLogic_GetDesc_BadSceneAddress )
{
    auto editor = GetProjectEditor();

    auto scene = CreateSceneForParamDesc( editor, "Scene" );
    EndUserParamsLogic paramsLogic( scene.get() );

    AddParamDescriptors( paramsLogic, scene );  if( HasFatalFailure() ) return;

    // ***********************
    //
    ParameterAddress address;
    address.SceneName = "BadName";
    address.NodeName = "root/ColoredRect";
    address.ParamTargetType = ParameterAddress::PluginParam;
    address.PluginName = "solid color";
    address.ParamName = "color";

    EXPECT_EQ( paramsLogic.GetDescriptor( address ), nullptr );

    // ***********************
    //
    address.SceneName = "BadName";
    address.NodeName = "root/TexturedRect";
    address.ParamTargetType = ParameterAddress::ResourceParam;
    address.PluginName = "texture";
    address.ParamName = "wrapModeX";
    address.ParamSubName = "Tex0";

    EXPECT_EQ( paramsLogic.GetDescriptor( address ), nullptr );


    // ***********************
    // 
    address.SceneName = "BadName";
    address.NodeName = "";
    address.ParamTargetType = ParameterAddress::LightParam;
    address.PluginName = "";
    address.ParamName = "color";
    address.ParamSubName = "";
    address.Index = 0;

    EXPECT_EQ( paramsLogic.GetDescriptor( address ), nullptr );


    // ***********************
    // 
    address.SceneName = "BadName";
    address.NodeName = "";
    address.ParamTargetType = ParameterAddress::CameraParam;
    address.PluginName = "";
    address.ParamName = "IsPerspective";
    address.ParamSubName = "";
    address.Index = 1;

    EXPECT_EQ( paramsLogic.GetDescriptor( address ), nullptr );


    // ***********************
    //
    address.SceneName = "";
    address.NodeName = "root/TexturedRect";
    address.ParamTargetType = ParameterAddress::NodeLogicParam;
    address.PluginName = "";
    address.ParamName = "OffsetX";
    address.ParamSubName = "";
    address.Index = 0;

    EXPECT_EQ( paramsLogic.GetDescriptor( address ), nullptr );


    // ***********************
    // Add Effect param descriptor
    address.SceneName = "BadName";
    address.NodeName = "root/ColoredRect";
    address.ParamTargetType = ParameterAddress::GlobalEffectParam;
    address.PluginName = "";
    address.ParamName = "alpha";
    address.ParamSubName = "";
    address.Index = 0;

    EXPECT_EQ( paramsLogic.GetDescriptor( address ), nullptr );
}

// ***********************
// Gets descriptor for param using almost good address.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( BVProjectEditor_ParameterDescriptor, EndUserParamsLogic_GetDesc_CameraIndexOutOfRange )
{
    auto editor = GetProjectEditor();

    auto scene = CreateSceneForParamDesc( editor, "Scene" );
    EndUserParamsLogic paramsLogic( scene.get() );

    AddParamDescriptors( paramsLogic, scene );  if( HasFatalFailure() ) return;

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

    EXPECT_EQ( paramsLogic.GetDescriptor( address ), nullptr );

    // ***********************
    //
    address.SceneName = scene->GetName();
    address.NodeName = "";
    address.ParamTargetType = ParameterAddress::LightParam;
    address.PluginName = "";
    address.ParamName = "color";
    address.ParamSubName = "";
    address.Index = 10;

    EXPECT_EQ( paramsLogic.GetDescriptor( address ), nullptr );
}

// ***********************
// Gets descriptor for param using almost good address.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( BVProjectEditor_ParameterDescriptor, EndUserParamsLogic_GetDesc_LightIndexOutOfRange )
{
    auto editor = GetProjectEditor();

    auto scene = CreateSceneForParamDesc( editor, "Scene" );
    EndUserParamsLogic paramsLogic( scene.get() );

    AddParamDescriptors( paramsLogic, scene );  if( HasFatalFailure() ) return;

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

    EXPECT_EQ( paramsLogic.GetDescriptor( address ), nullptr );

    // ***********************
    //
    address.SceneName = scene->GetName();
    address.NodeName = "";
    address.ParamTargetType = ParameterAddress::LightParam;
    address.PluginName = "";
    address.ParamName = "color";
    address.ParamSubName = "";
    address.Index = 2;

    EXPECT_EQ( paramsLogic.GetDescriptor( address ), nullptr );
}

// ***********************
// Gets descriptor for param using almost good address.
// Wrong node path shouldn't crash engine (this happend in some old versions of BV).
SIMPLE_FRAMEWORK_TEST_IN_SUITE( BVProjectEditor_ParameterDescriptor, EndUserParamsLogic_GetDesc_InvalidNodeName )
{
    auto editor = GetProjectEditor();

    auto scene = CreateSceneForParamDesc( editor, "Scene" );
    EndUserParamsLogic paramsLogic( scene.get() );

    AddParamDescriptors( paramsLogic, scene );  if( HasFatalFailure() ) return;

    // ***********************
    //
    ParameterAddress address;
    address.SceneName = scene->GetName();
    address.NodeName = "root/Colo";
    address.ParamTargetType = ParameterAddress::PluginParam;
    address.PluginName = "solid color";
    address.ParamName = "color";

    EXPECT_EQ( paramsLogic.GetDescriptor( address ), nullptr );

    // ***********************
    //
    address.SceneName = scene->GetName();
    address.NodeName = "root/Textur";
    address.ParamTargetType = ParameterAddress::ResourceParam;
    address.PluginName = "texture";
    address.ParamName = "wrapModeX";
    address.ParamSubName = "Tex0";

    EXPECT_EQ( paramsLogic.GetDescriptor( address ), nullptr );

    // ***********************
    //
    address.SceneName = scene->GetName();
    address.NodeName = "";
    address.ParamTargetType = ParameterAddress::PluginParam;
    address.PluginName = "solid color";
    address.ParamName = "color";

    EXPECT_EQ( paramsLogic.GetDescriptor( address ), nullptr );
}

// ***********************
// Gets descriptor for param using almost good address.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( BVProjectEditor_ParameterDescriptor, EndUserParamsLogic_GetDesc_InvalidPluginName )
{
    auto editor = GetProjectEditor();

    auto scene = CreateSceneForParamDesc( editor, "Scene" );
    EndUserParamsLogic paramsLogic( scene.get() );

    AddParamDescriptors( paramsLogic, scene );  if( HasFatalFailure() ) return;

    // ***********************
    //
    ParameterAddress address;
    address.SceneName = scene->GetName();
    address.NodeName = "root/ColoredRect";
    address.ParamTargetType = ParameterAddress::PluginParam;
    address.PluginName = "solid ";
    address.ParamName = "color";

    EXPECT_EQ( paramsLogic.GetDescriptor( address ), nullptr );

    // ***********************
    //
    address.SceneName = scene->GetName();
    address.NodeName = "root/TexturedRect";
    address.ParamTargetType = ParameterAddress::ResourceParam;
    address.PluginName = "te";
    address.ParamName = "wrapModeX";
    address.ParamSubName = "Tex0";

    EXPECT_EQ( paramsLogic.GetDescriptor( address ), nullptr );
}


// ***********************
// Gets descriptor for param using almost good address.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( BVProjectEditor_ParameterDescriptor, EndUserParamsLogic_GetDesc_InvalidParamName )
{
    auto editor = GetProjectEditor();

    auto scene = CreateSceneForParamDesc( editor, "Scene" );
    EndUserParamsLogic paramsLogic( scene.get() );

    AddParamDescriptors( paramsLogic, scene );  if( HasFatalFailure() ) return;

    // ***********************
    //
    ParameterAddress address;
    address.SceneName = scene->GetName();
    address.NodeName = "root/ColoredRect";
    address.ParamTargetType = ParameterAddress::PluginParam;
    address.PluginName = "solid color";
    address.ParamName = "col";

    EXPECT_EQ( paramsLogic.GetDescriptor( address ), nullptr );

    // ***********************
    //
    address.SceneName = scene->GetName();
    address.NodeName = "root/TexturedRect";
    address.ParamTargetType = ParameterAddress::ResourceParam;
    address.PluginName = "texture";
    address.ParamName = "wra";
    address.ParamSubName = "Tex0";

    EXPECT_EQ( paramsLogic.GetDescriptor( address ), nullptr );
}
