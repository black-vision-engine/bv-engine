#include "gtest/gtest.h"

#include "Helpers/RenderLogicInitializerAccessor.h"

#include "Engine/Graphics/Effects/Logic/Components/Initialization/RenderedChannelsDataDesc.h"

#include "BVConfig.h"


using namespace bv;


// ***********************
// Tests render channels initialization from config with proper content.
TEST( Engine_RenderChannels, DescriptorInit )
{
    BVConfig config( "TestConfigs/OutputsTests/TwoRenderChannels.xml" );

    RenderedChannelsDataDesc desc;

    TEST_ACCESSOR( RenderLogicInitializer )::Initialize( desc, config );

    EXPECT_TRUE( desc.IsEnabled( RenderChannelType::RCT_OUTPUT_1 ) );
    EXPECT_TRUE( desc.IsEnabled( RenderChannelType::RCT_OUTPUT_2 ) );
    EXPECT_FALSE( desc.IsEnabled( RenderChannelType::RCT_OUTPUT_3 ) );
    EXPECT_FALSE( desc.IsEnabled( RenderChannelType::RCT_OUTPUT_4 ) );
}

// ***********************
// Tests initialization with missing entries in config file. Render channels are defaulted to false.
TEST( Engine_RenderChannels, DescriptorInit_MissingEntries )
{
    BVConfig config( "TestConfigs/OutputsTests/MissingRenderChannelEntries.xml" );

    RenderedChannelsDataDesc desc;

    TEST_ACCESSOR( RenderLogicInitializer )::Initialize( desc, config );

    EXPECT_TRUE( desc.IsEnabled( RenderChannelType::RCT_OUTPUT_1 ) );
    EXPECT_FALSE( desc.IsEnabled( RenderChannelType::RCT_OUTPUT_2 ) );
    EXPECT_FALSE( desc.IsEnabled( RenderChannelType::RCT_OUTPUT_3 ) );
    EXPECT_FALSE( desc.IsEnabled( RenderChannelType::RCT_OUTPUT_4 ) );
}

// ***********************
// Tests initialization without Description in config. First channel should be enabled by default.
TEST( Engine_RenderChannels, DescriptorInit_WithoutConfig )
{
    BVConfig config( "TestConfigs/OutputsTests/NoRenderChannelsDesc.xml" );

    RenderedChannelsDataDesc desc;

    TEST_ACCESSOR( RenderLogicInitializer )::Initialize( desc, config );

    EXPECT_TRUE( desc.IsEnabled( RenderChannelType::RCT_OUTPUT_1 ) );
    EXPECT_FALSE( desc.IsEnabled( RenderChannelType::RCT_OUTPUT_2 ) );
    EXPECT_FALSE( desc.IsEnabled( RenderChannelType::RCT_OUTPUT_3 ) );
    EXPECT_FALSE( desc.IsEnabled( RenderChannelType::RCT_OUTPUT_4 ) );
}


