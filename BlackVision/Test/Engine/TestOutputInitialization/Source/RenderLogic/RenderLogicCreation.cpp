#include "gtest/gtest.h"


#include "Helpers/RenderLogicInitializerAccessor.h"
#include "Helpers/OutputCompositeVideoAccessor.h"
#include "Helpers/OutputExtractor.h"

#include "Engine/Graphics/Effects/Logic/Components/Initialization/RenderedChannelsDataDesc.h"
#include "Engine/Graphics/Effects/Logic/RenderLogicImpl.h"

#include "Engine/Graphics/Effects/Logic/OutputRendering/Impl/CompositeOutputs/Video/OutputCompositeVideo.h"
#include "Engine/Graphics/Effects/Logic/OutputRendering/Impl/FrameDataHandlers/AVFileOutput/AVFileOutput.h"

#include "VideoCardManager.h"
#include "Services/BVServiceProvider.h"

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

// ***********************
// Gets outputs and checks there type.
// TODO: Find better way to check type then dynamic_cast. In future it could be disabled.
TEST( Engine_RenderChannels, RenderLogicInit_OutputsTypesCreation )
{
    BVConfig config( "TestConfigs/OutputsTests/TwoRenderChannels.xml" );

    auto renderLogic = static_cast< RenderLogicImpl * >( RenderLogicInitializer::CreateInstance( config ) );
    ASSERT_NE( renderLogic, nullptr );

    auto outputVideo = dynamic_cast< OutputCompositeVideo * >( renderLogic->GetOutputLogic()->GetOutput( CustomOutputType::COT_VIDEO ) );
    EXPECT_NE( outputVideo, nullptr );

    auto outputFile = dynamic_cast< AVFileOutput * >( renderLogic->GetOutputLogic()->GetOutput( CustomOutputType::COT_AV_FILE ) );
    EXPECT_NE( outputFile, nullptr );

    // We can't determine this type better at this moment :(
    auto outputPreview = dynamic_cast< OutputInstance * >( renderLogic->GetOutputLogic()->GetOutput( CustomOutputType::COT_PREVIEW ) );
    EXPECT_NE( outputPreview, nullptr );

    auto outputSharedMem = dynamic_cast< OutputInstance * >( renderLogic->GetOutputLogic()->GetOutput( CustomOutputType::COT_STREAM ) );
    EXPECT_NE( outputSharedMem, nullptr );
}

// ***********************
// Video output configuration with right config.
TEST( Engine_RenderChannels, RenderLogicInit_OutputVideo )
{
    BVConfig config( "TestConfigs/OutputsTests/TwoRenderChannels.xml" );

    auto renderLogic = static_cast< RenderLogicImpl * >( RenderLogicInitializer::CreateInstance( config ) );
    ASSERT_NE( renderLogic, nullptr );

    OutputExtractor extractor( renderLogic );
    auto & inputChannels = extractor.GetInputChannels();


    for( unsigned int i = 0; i < inputChannels.GetNumVideoInputChannels(); ++i )
    {
        auto vic = inputChannels.GetVideoInputChannelAt( i );
        EXPECT_TRUE( vic->IsActive() );
        EXPECT_EQ( vic->GetHeight(), 1080 );
        EXPECT_EQ( vic->GetWidth(), 1920 );

        // Render channells numbers have the same index as video outputs in this case. But it isn't any rule.
        EXPECT_EQ( vic->GetWrappedChannel(), renderLogic->GetRenderedChannelsData()->GetRenderChannel( (RenderChannelType)i ) );
    }
}

// ***********************
// Config doesn't specifies RenderChannels. All input channels chould return active RenderChannels.
TEST( Engine_RenderChannels, RenderLogicInit_OutputVideo_NoChannels )
{
    BVConfig config( "TestConfigs/OutputsTests/NoRenderChannelsDesc.xml" );

    auto renderLogic = static_cast< RenderLogicImpl * >( RenderLogicInitializer::CreateInstance( config ) );
    ASSERT_NE( renderLogic, nullptr );

    OutputExtractor extractor( renderLogic );
    auto & inputChannels = extractor.GetInputChannels();

    for( unsigned int i = 0; i < inputChannels.GetNumVideoInputChannels(); ++i )
    {
        auto vic = inputChannels.GetVideoInputChannelAt( i );
        EXPECT_TRUE( vic->IsActive() );
    }
}

// ***********************
// One RenderChannel is mapped to multiple VideoChannels
TEST( Engine_RenderChannels, RenderLogicInit_MultiVideoOutputsFromChannel )
{
    BVConfig config( "TestConfigs/OutputsTests/MultiOutputsFromChannel.xml" );

    auto renderLogic = static_cast< RenderLogicImpl * >( RenderLogicInitializer::CreateInstance( config ) );
    ASSERT_NE( renderLogic, nullptr );

    OutputExtractor extractor( renderLogic );
    auto & inputChannels = extractor.GetInputChannels();

    ASSERT_EQ( inputChannels.GetNumVideoInputChannels(), 1 );

    auto renderChannel = renderLogic->GetRenderedChannelsData()->GetRenderChannel( ( RenderChannelType )0 );

    EXPECT_EQ( inputChannels.GetInputChannel( 0 )->GetWrappedChannel(), renderChannel );
    EXPECT_EQ( inputChannels.GetInputChannel( 1 )->GetWrappedChannel(), renderChannel );
}

// ***********************
// RenderChannel number 1 is disabled but it has VideoOutput defined in itself.
// This VideoOutput should be ignored.
TEST( Engine_RenderChannels, RenderLogicInit_OutputInDisabledChannel )
{
    BVConfig config( "TestConfigs/OutputsTests/OutputInDisabledChannel.xml" );

    auto renderLogic = static_cast< RenderLogicImpl * >( RenderLogicInitializer::CreateInstance( config ) );
    ASSERT_NE( renderLogic, nullptr );

    OutputExtractor extractor( renderLogic );
    auto & inputChannels = extractor.GetInputChannels();

    ASSERT_EQ( inputChannels.GetNumVideoInputChannels(), 1 );
}

// ***********************
// At least one channel must be enabled. Engine Enables first channel by default.
TEST( Engine_RenderChannels, RenderLogicInit_AllChannelsDisabled )
{
    BVConfig config( "TestConfigs/OutputsTests/AllChannelsDisabled.xml" );

    auto renderLogic = static_cast< RenderLogicImpl * >( RenderLogicInitializer::CreateInstance( config ) );
    ASSERT_NE( renderLogic, nullptr );

    auto renderChannels = renderLogic->GetRenderedChannelsData();

    EXPECT_TRUE( renderChannels->IsActive( RenderChannelType::RCT_OUTPUT_1 ) );
    EXPECT_FALSE( renderChannels->IsActive( RenderChannelType::RCT_OUTPUT_2 ) );
    EXPECT_FALSE( renderChannels->IsActive( RenderChannelType::RCT_OUTPUT_3 ) );
    EXPECT_FALSE( renderChannels->IsActive( RenderChannelType::RCT_OUTPUT_4 ) );
}

// ***********************
// This test checks proper handling of bad width height VideoOutput parameter
TEST( Engine_RenderChannels, RenderLogicInit_VideoOutput_BadWidthHeight )
{
    BVConfig config( "TestConfigs/OutputsTests/BadOutputsWidthHeight.xml" );

    auto renderLogic = static_cast< RenderLogicImpl * >( RenderLogicInitializer::CreateInstance( config ) );
    ASSERT_NE( renderLogic, nullptr );

    OutputExtractor extractor( renderLogic );
    auto & inputChannels = extractor.GetInputChannels();

    ASSERT_EQ( inputChannels.GetNumVideoInputChannels(), 2 );

    auto vic1 = inputChannels.GetInputChannel( 0 );
    auto vic2 = inputChannels.GetInputChannel( 1 );

    EXPECT_EQ( vic1->GetHeight(), 200 );
    EXPECT_EQ( vic1->GetWidth(), 1000 );

    EXPECT_EQ( vic2->GetHeight(), 0 );
    EXPECT_EQ( vic2->GetWidth(), 0 );
}


