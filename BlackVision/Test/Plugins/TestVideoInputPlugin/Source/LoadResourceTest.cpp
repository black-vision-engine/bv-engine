#include "gtest/gtest.h"
#include "Framework/FrameworkTest.h"
#include "Framework/BVTestAppLogic.h"

#include "Engine/Editors/BVProjectEditor.h"

#include "Assets/Input/VideoInput/VideoInputAsset.h"
#include "Assets/Input/VideoInput/VideoInputAssetDesc.h"


#include "Utils/Scenes/TestScenesCreator.h"
#include "Utils/Nodes/TestNodesCreator.h"

//#include "TestHelpers/ParametersSets.h"

#include "Assets/DefaultAssets.h"


using namespace bv;




// ***********************
// Loads valid video input.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( Plugins_VideoInput, ResourceLoading_ValidInput )
{
    auto videoPlugin = TestScenesCreator::VideoInputRectangle( GetProjectEditor(), "Scene", 400, 400, 0 );
    auto desc = VideoInputAssetDesc::Create( 1, 2 );

    ASSERT_NE( videoPlugin, nullptr );
    EXPECT_TRUE( videoPlugin->LoadResource( desc ) );

    auto lassets = videoPlugin->GetLAssets();
    ASSERT_TRUE( lassets.size() == 1 );

    auto videoDesc = std::static_pointer_cast< const VideoInputAssetDesc >( lassets[ 0 ].assetDesc );

    EXPECT_EQ( desc, videoDesc );
}



