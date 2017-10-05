#include "gtest/gtest.h"


#include "Helpers/RenderLogicInitializerAccessor.h"
#include "Helpers/OutputCompositeVideoAccessor.h"

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
//
TEST( Engine_RenderChannels, RenderLogicInit_OutputVideo )
{
    BVConfig config( "TestConfigs/OutputsTests/TwoRenderChannels.xml" );

    auto renderLogic = static_cast< RenderLogicImpl * >( RenderLogicInitializer::CreateInstance( config ) );
    ASSERT_NE( renderLogic, nullptr );

    auto output = static_cast< OutputCompositeVideo * >( renderLogic->GetOutputLogic()->GetOutput( CustomOutputType::COT_VIDEO ) );
    ASSERT_NE( output, nullptr );

    auto & outPreprocessor = TEST_ACCESSOR( OutputCompositeVideo )::GetVideoOutputPreprocessor( output );
    auto & inputChannels = TEST_ACCESSOR( VideoOutputsPreprocessor )::GetInputChannels( outPreprocessor );

    videocards::VideoCardManager * videoCardManager = new videocards::VideoCardManager();

    videoCardManager->RegisterDescriptors( videocards::DefaultVideoCardDescriptors() );
    videoCardManager->ReadConfig( DefaultConfig.GetNode( "config" ) );
    videoCardManager->Start();

    TEST_ACCESSOR( BVServiceProvider )::RegisterVideoCardManager( videoCardManager );

    RenderContext ctx;
    ctx.SetAudio( new audio::AudioRenderer() );

    TEST_ACCESSOR( VideoOutputsPreprocessor )::Initialize( outPreprocessor, &ctx, renderLogic->GetRenderedChannelsData() );

    delete ctx.GetAudio();


    for( unsigned int i = 0; i < inputChannels.GetNumVideoInputChannels(); ++i )
    {
        auto vic = inputChannels.GetVideoInputChannelAt( i );
        EXPECT_TRUE( vic->IsActive() );
        EXPECT_EQ( vic->GetHeight(), 1080 );
        EXPECT_EQ( vic->GetWidth(), 1920 );
    }
    
}


