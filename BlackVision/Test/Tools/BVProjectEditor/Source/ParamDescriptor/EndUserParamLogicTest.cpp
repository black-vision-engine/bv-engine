#include "Framework/FrameworkTest.h"

#include "Engine/Editors/EditorVariables/ParametersDescriptors/EndUserParamsLogic.h"
#include "Engine/Models/SceneModel.h"

#include "Utils/TwoScenesFixture.h"


using namespace bv;


// ***********************
// Tests adding and removing parameter descriptor.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( BVProjectEditor.ParameterDescriptor, EndUserParamsLogic_AddRemovePluginParamDesc )
{
    auto scene = CreateOneSceneWithColoredRect( GetProjectEditor(), "Scene" );
    EndUserParamsLogic paramsLogic( scene.get() );

    EndUserParamDescriptor desc;
    desc.SetName( "Descr" );
    desc.SetDescription( "This is descriptor" );

    ParameterAddress address;
    address.SceneName = scene->GetName();
    address.NodeName = "root/ColoredRect";
    address.ParamTargetType = ParameterAddress::PluginParam;
    address.PluginName = "solid color";
    address.ParamName = "color";

    CHECK( paramsLogic.AddDescriptor( ParameterAddress( address ), std::move( desc ) ) );

    auto returnDesc = paramsLogic.GetDescriptor( address );
    REQUIRE CHECK( returnDesc != nullptr );

    CHECK( returnDesc->GetName() == "Descr" );
    CHECK( returnDesc->GetDescription() == "This is descriptor" );

    CHECK( paramsLogic.RemoveDescriptor( address ) );

    returnDesc = paramsLogic.GetDescriptor( address );
    CHECK( returnDesc == nullptr );
}







