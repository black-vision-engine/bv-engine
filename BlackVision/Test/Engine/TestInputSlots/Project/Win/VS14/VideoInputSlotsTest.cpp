#include "gtest/gtest.h"
#include "Framework/FrameworkTest.h"
#include "Framework/BVTestAppLogic.h"

#include "Engine/Graphics/InputSlots/SlotsLogic/VideoInput/VideoInputSlots.h"
#include "VideoInput/VideoInputChannelDesc.h"

#include "Engine/Graphics/Effects/Logic/Components/RenderContext.h"

#include "Helpers/AVFrameHelpers.h"

using namespace bv;


// ========================================================================= //
// Tests
// ========================================================================= //


// ***********************
//
TEST( Engine_InputSlots, VideoInput_RegisterSource )
{
    auto inputSlots = std::make_shared< InputSlots >();
    VideoInputSlots slots( inputSlots );

    videocards::VideoInputChannelDesc channel1( 1, 0, "BlueFish", "A", CreateDefaultAVFrame() );

    ASSERT_TRUE( slots.RegisterVideoInputChannel( channel1 ) );
    EXPECT_TRUE( slots.Exists( channel1 ) );
    EXPECT_TRUE( slots.Exists( 0 ) );

    auto slotIdx = slots.GetSlotIndex( 0 );
    Expected< InputSlot > slot = inputSlots->AccessSource( slotIdx );

    ASSERT_TRUE( slot.IsValid() );
    EXPECT_NE( slot.GetVal().Texture, nullptr );
    EXPECT_NE( slot.GetVal().Audio, nullptr );
}

// ***********************
// VideoInputSlots should fail to register channel for the second time.
TEST( Engine_InputSlots, VideoInput_RegisterExistingSource )
{
    VideoInputSlots slots( std::make_shared< InputSlots >() );

    videocards::VideoInputChannelDesc channel1( 1, 0, "BlueFish", "A", CreateDefaultAVFrame() );
    videocards::VideoInputChannelDesc channel2( 1, 0, "BlueFish", "A", CreateDefaultAVFrame() );

    ASSERT_TRUE( slots.RegisterVideoInputChannel( channel1 ) );
    ASSERT_TRUE( slots.Exists( channel1 ) );

    ASSERT_FALSE( slots.RegisterVideoInputChannel( channel2 ) );
    EXPECT_TRUE( slots.Exists( 0 ) );
}

// ***********************
// VideoInputSlots can register and hold data for multiple channels.
TEST( Engine_InputSlots, VideoInput_Register2DifferentChannels )
{
    VideoInputSlots slots( std::make_shared< InputSlots >() );

    videocards::VideoInputChannelDesc channel1( 1, 0, "BlueFish", "A", CreateDefaultAVFrame() );
    videocards::VideoInputChannelDesc channel2( 1, 1, "BlueFish", "C", CreateDefaultAVFrame() );

    ASSERT_TRUE( slots.RegisterVideoInputChannel( channel1 ) );
    ASSERT_TRUE( slots.Exists( channel1 ) );

    ASSERT_TRUE( slots.RegisterVideoInputChannel( channel2 ) );
    ASSERT_TRUE( slots.Exists( channel2 ) );

    EXPECT_TRUE( slots.Exists( 0 ) );
    EXPECT_TRUE( slots.Exists( 1 ) );
}


// ***********************
//
SIMPLE_FRAMEWORK_TEST_IN_SUITE( Engine_InputSlots, VideoInput_UnregisterSource )
{
    auto inputSlots = std::make_shared< InputSlots >();
    VideoInputSlots slots( inputSlots );

    videocards::VideoInputChannelDesc channel1( 1, 0, "BlueFish", "A", CreateDefaultAVFrame() );

    RenderContext renderCtx;
    renderCtx.SetRenderer( GetAppLogic()->GetRenderer() );
    renderCtx.SetAudio( GetAppLogic()->GetAudioRenderer() );

    ASSERT_TRUE( slots.RegisterVideoInputChannel( channel1 ) );
    ASSERT_TRUE( slots.UnregisterVideoInputChannel( &renderCtx, 0 ) );

    EXPECT_FALSE( slots.Exists( 0 ) );
    EXPECT_FALSE( slots.Exists( channel1 ) );
}

// ***********************
//
SIMPLE_FRAMEWORK_TEST_IN_SUITE( Engine_InputSlots, VideoInput_UnregisterAllSources )
{
    auto inputSlots = std::make_shared< InputSlots >();
    VideoInputSlots slots( inputSlots );

    videocards::VideoInputChannelDesc channel1( 1, 0, "BlueFish", "A", CreateDefaultAVFrame() );
    videocards::VideoInputChannelDesc channel2( 1, 1, "BlueFish", "C", CreateDefaultAVFrame() );

    RenderContext renderCtx;
    renderCtx.SetRenderer( GetAppLogic()->GetRenderer() );
    renderCtx.SetAudio( GetAppLogic()->GetAudioRenderer() );

    ASSERT_TRUE( slots.RegisterVideoInputChannel( channel1 ) );
    ASSERT_TRUE( slots.RegisterVideoInputChannel( channel2 ) );

    ASSERT_TRUE( slots.UnregisterAllChannels( &renderCtx ) );

    EXPECT_FALSE( slots.Exists( 0 ) );
    EXPECT_FALSE( slots.Exists( channel1 ) );

    EXPECT_FALSE( slots.Exists( 1 ) );
    EXPECT_FALSE( slots.Exists( channel2 ) );
}

// ***********************
//
SIMPLE_FRAMEWORK_TEST_IN_SUITE( Engine_InputSlots, VideoInput_UpdateSlot )
{
    auto inputSlots = std::make_shared< InputSlots >();
    VideoInputSlots slots( inputSlots );

    auto avFrameDesc = CreateDefaultAVFrame();
    videocards::VideoInputChannelDesc channel1( 1, 0, "BlueFish", "A", avFrameDesc );

    RenderContext renderCtx;
    renderCtx.SetRenderer( GetAppLogic()->GetRenderer() );
    renderCtx.SetAudio( GetAppLogic()->GetAudioRenderer() );

    ASSERT_TRUE( slots.RegisterVideoInputChannel( channel1 ) );

    auto videoSize = avFrameDesc.depth * avFrameDesc.height * avFrameDesc.width;
    auto audioSize = avFrameDesc.channelDepth * avFrameDesc.channels * avFrameDesc.numSamples;

    auto videoChunk = MemoryChunk::Create( videoSize );
    auto audioChunk = MemoryChunk::Create( audioSize );

    slots.UpdateVideoInput( 0, AVFrame::Create( videoChunk, audioChunk, avFrameDesc ) );

    auto slot = slots.GetInputSlots()->AccessSource( slots.GetSlotIndex( 0 ) );
    ASSERT_TRUE( slot.IsValid() );

    EXPECT_EQ( slot.GetVal().Texture->GetData(), videoChunk );
    EXPECT_EQ( slot.GetVal().Audio->GetData(), audioChunk );
}
