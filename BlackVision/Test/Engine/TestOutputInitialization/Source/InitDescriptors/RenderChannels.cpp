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

// ***********************
// Duplicate RenderChannels entries in config file. Second entry is always ignored.
TEST( Engine_RenderChannels, DescriptorInit_DuplicateRenderChannels )
{
    BVConfig config( "TestConfigs/OutputsTests/DuplicateRenderChannels.xml" );

    RenderedChannelsDataDesc desc;

    TEST_ACCESSOR( RenderLogicInitializer )::Initialize( desc, config );

    EXPECT_TRUE( desc.IsEnabled( RenderChannelType::RCT_OUTPUT_1 ) );
    EXPECT_TRUE( desc.IsEnabled( RenderChannelType::RCT_OUTPUT_2 ) );
    EXPECT_TRUE( desc.IsEnabled( RenderChannelType::RCT_OUTPUT_3 ) );
    EXPECT_TRUE( desc.IsEnabled( RenderChannelType::RCT_OUTPUT_4 ) );
}

// ***********************
// RenderChannel's id not specified in config. Note: this test checks if this lack
// of id is handled properly. In first implementation id was defaulted to 0, what can cause problems.
// Second entry in config has no id and enable value set to false.
TEST( Engine_RenderChannels, RenderLogicInit_ChannelWithoutID )
{
    BVConfig config( "TestConfigs/OutputsTests/ChannelWithoutID.xml" );

    RenderedChannelsDataDesc desc;

    TEST_ACCESSOR( RenderLogicInitializer )::Initialize( desc, config );

    EXPECT_TRUE( desc.IsEnabled( RenderChannelType::RCT_OUTPUT_1 ) );
    EXPECT_FALSE( desc.IsEnabled( RenderChannelType::RCT_OUTPUT_2 ) );
    EXPECT_TRUE( desc.IsEnabled( RenderChannelType::RCT_OUTPUT_3 ) );
    EXPECT_TRUE( desc.IsEnabled( RenderChannelType::RCT_OUTPUT_4 ) );
}





