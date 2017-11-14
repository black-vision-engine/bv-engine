#include "gtest/gtest.h"

#include "Engine/Graphics/InputSlots/SlotsLogic/VideoInput/VideoInputSlots.h"
#include "VideoInput/VideoInputChannelDesc.h"


using namespace bv;




// ***********************
//
TEST( Engine_InputSlots, VideoInput_RegisterSource )
{
    VideoInputSlots slots( std::make_shared< InputSlots >() );

    videocards::VideoInputChannelDesc channel1( 0, "BlueFish", "A" );

    ASSERT_TRUE( slots.RegisterVideoInputChannel( channel1 ) );
    EXPECT_TRUE( slots.Exists( 0 ) );
}


