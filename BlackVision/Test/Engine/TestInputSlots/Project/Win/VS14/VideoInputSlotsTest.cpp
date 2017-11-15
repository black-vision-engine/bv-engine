#include "gtest/gtest.h"

#include "Engine/Graphics/InputSlots/SlotsLogic/VideoInput/VideoInputSlots.h"
#include "VideoInput/VideoInputChannelDesc.h"


using namespace bv;


// ========================================================================= //
// Helpers
// ========================================================================= //


AVFrameDescriptor       CreateDefaultAVFrame    ()
{
    AVFrameDescriptor desc;
    desc.width = 1000;
    desc.height = 1000;
    desc.channels = 2;
    desc.depth = 4;
    desc.sampleRate = 44000;

    return desc;
}


// ========================================================================= //
// Tests
// ========================================================================= //


// ***********************
//
TEST( Engine_InputSlots, VideoInput_RegisterSource )
{
    auto inputSlots = std::make_shared< InputSlots >();
    VideoInputSlots slots( inputSlots );

    videocards::VideoInputChannelDesc channel1( 0, "BlueFish", "A", CreateDefaultAVFrame() );

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
//
TEST( Engine_InputSlots, VideoInput_RegisterExistingSource )
{
    VideoInputSlots slots( std::make_shared< InputSlots >() );

    videocards::VideoInputChannelDesc channel1( 0, "BlueFish", "A", CreateDefaultAVFrame() );
    videocards::VideoInputChannelDesc channel2( 0, "BlueFish", "A", CreateDefaultAVFrame() );

    ASSERT_TRUE( slots.RegisterVideoInputChannel( channel1 ) );
    ASSERT_TRUE( slots.Exists( channel1 ) );

    ASSERT_FALSE( slots.RegisterVideoInputChannel( channel2 ) );
    EXPECT_TRUE( slots.Exists( 0 ) );
}
