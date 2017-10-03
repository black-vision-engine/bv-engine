#include "gtest/gtest.h"

#include "Helpers/RenderLogicInitializerAccessor.h"

#include "Engine/Graphics/Effects/Logic/Components/Initialization/RenderedChannelsDataDesc.h"

#include "BVConfig.h"


using namespace bv;


// ***********************
// Tests render channels initialization from config with proper content.
TEST( Engine_RenderChannels, RenderLogicInitialization )
{
    BVConfig config( "TestConfigs/OutputsTests/TwoRenderChannels.xml" );

    RenderedChannelsDataDesc desc;

    TEST_ACCESSOR( RenderLogicInitializer )::Initialize( desc, config );

    EXPECT_TRUE( desc.IsEnabled( RenderChannelType::RCT_OUTPUT_1 ) );
    EXPECT_TRUE( desc.IsEnabled( RenderChannelType::RCT_OUTPUT_2 ) );
    EXPECT_FALSE( desc.IsEnabled( RenderChannelType::RCT_OUTPUT_3 ) );
    EXPECT_FALSE( desc.IsEnabled( RenderChannelType::RCT_OUTPUT_4 ) );
}
