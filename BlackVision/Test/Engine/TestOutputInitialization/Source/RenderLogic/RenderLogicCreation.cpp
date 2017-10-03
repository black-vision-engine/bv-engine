#include "gtest/gtest.h"


#include "Helpers/RenderLogicInitializerAccessor.h"

#include "Engine/Graphics/Effects/Logic/Components/Initialization/RenderedChannelsDataDesc.h"
#include "Engine/Graphics/Effects/Logic/RenderLogicImpl.h"

#include "BVConfig.h"


using namespace bv;



// ***********************
// Creating RenderLogic with proper config
TEST( Engine_RenderChannels, RenderLogicInit )
{
    BVConfig config( "TestConfigs/OutputsTests/TwoRenderChannels.xml" );

    auto renderLogic = static_cast< RenderLogicImpl * >( RenderLogicInitializer::CreateInstance( config ) );
    ASSERT_NE( renderLogic, nullptr );

    auto renderChannels = renderLogic->GetRenderedChannelsData();

    EXPECT_TRUE( renderChannels->IsActive( RenderChannelType::RCT_OUTPUT_1 ) );
    EXPECT_TRUE( renderChannels->IsActive( RenderChannelType::RCT_OUTPUT_2 ) );
    EXPECT_FALSE( renderChannels->IsActive( RenderChannelType::RCT_OUTPUT_3 ) );
    EXPECT_FALSE( renderChannels->IsActive( RenderChannelType::RCT_OUTPUT_4 ) );
}

// ***********************
// Tests initialization with missing entries in config file. Render channels are defaulted to false.
TEST( Engine_RenderChannels, RenderLogicInit_MissingEntries )
{
    BVConfig config( "TestConfigs/OutputsTests/MissingRenderChannelEntries.xml" );

    auto renderLogic = static_cast< RenderLogicImpl * >( RenderLogicInitializer::CreateInstance( config ) );
    ASSERT_NE( renderLogic, nullptr );

    auto renderChannels = renderLogic->GetRenderedChannelsData();

    EXPECT_TRUE( renderChannels->IsActive( RenderChannelType::RCT_OUTPUT_1 ) );
    EXPECT_FALSE( renderChannels->IsActive( RenderChannelType::RCT_OUTPUT_2 ) );
    EXPECT_FALSE( renderChannels->IsActive( RenderChannelType::RCT_OUTPUT_3 ) );
    EXPECT_FALSE( renderChannels->IsActive( RenderChannelType::RCT_OUTPUT_4 ) );
}

// ***********************
// Tests initialization without Description in config. First channel should be enabled by default.
TEST( Engine_RenderChannels, RenderLogicInit_WithoutConfig )
{
    BVConfig config( "TestConfigs/OutputsTests/NoRenderChannelsDesc.xml" );

    auto renderLogic = static_cast< RenderLogicImpl * >( RenderLogicInitializer::CreateInstance( config ) );
    ASSERT_NE( renderLogic, nullptr );

    auto renderChannels = renderLogic->GetRenderedChannelsData();

    EXPECT_TRUE( renderChannels->IsActive( RenderChannelType::RCT_OUTPUT_1 ) );
    EXPECT_FALSE( renderChannels->IsActive( RenderChannelType::RCT_OUTPUT_2 ) );
    EXPECT_FALSE( renderChannels->IsActive( RenderChannelType::RCT_OUTPUT_3 ) );
    EXPECT_FALSE( renderChannels->IsActive( RenderChannelType::RCT_OUTPUT_4 ) );
}

