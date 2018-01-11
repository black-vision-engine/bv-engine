#include "gtest/gtest.h"
#include "Framework/FrameworkTest.h"
#include "Framework/BVTestAppLogic.h"

#include "Engine/Graphics/InputSlots/SlotsLogic/VideoInput/VideoInputSlots.h"
#include "Engine/Graphics/InputSlots/Logic/Handlers/VideoInputHandler.h"

#include "Utils/Accessors/Engine/Inputs/VideoInputHandlerAccessor.h"

#include "VideoInput/VideoInputChannelDesc.h"
#include "Helpers/AVFrameHelpers.h"

#include "Engine/Graphics/Effects/Logic/Components/RenderContext.h"


using namespace bv;



// ***********************
// Tests registration of video inputs. Note that we test internal registration function
// to avoid using VideoCardManager to query video card descriptors. This way we don't have to
// initialize VideoCardManager with real video cards.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( Engine_InputSlots, VideoInputHandler_Register )
{
    auto inputSlots = std::make_shared< InputSlots >();
    VideoInputHandlerPtr handler = std::make_shared< VideoInputHandler >( GetAppLogic()->GetVideoCardManager(), inputSlots );

    videocards::InputChannelsDescsVec channels;

    videocards::VideoInputChannelDesc channel1( 1, 0, "BlueFish", "A", CreateDefaultAVFrame() );
    videocards::VideoInputChannelDesc channel2( 2, 1, "BlueFish", "C", CreateDefaultAVFrame() );

    channels.push_back( channel1 );
    channels.push_back( channel2 );

    TEST_ACCESSOR( VideoInputHandler )::RegisterInputs( handler, channels );

    // We expect that slots will reside under first 2 indicies.
    // That isn't necessary condition but it's the best way to check if slots were registered.
    EXPECT_TRUE( inputSlots->AccessSource( 0 ).IsValid() );
    EXPECT_TRUE( inputSlots->AccessSource( 1 ).IsValid() );
}

// ***********************
//
SIMPLE_FRAMEWORK_TEST_IN_SUITE( Engine_InputSlots, VideoInputHandler_Unregister )
{
    auto inputSlots = std::make_shared< InputSlots >();
    VideoInputHandlerPtr handler = std::make_shared< VideoInputHandler >( GetAppLogic()->GetVideoCardManager(), inputSlots );

    videocards::InputChannelsDescsVec channels;

    videocards::VideoInputChannelDesc channel1( 1, 0, "BlueFish", "A", CreateDefaultAVFrame() );
    videocards::VideoInputChannelDesc channel2( 2, 1, "BlueFish", "C", CreateDefaultAVFrame() );

    channels.push_back( channel1 );
    channels.push_back( channel2 );

    TEST_ACCESSOR( VideoInputHandler )::RegisterInputs( handler, channels );
    TEST_ACCESSOR( VideoInputHandler )::SetRenderContext( handler, GetAppLogic()->GetRenderer(), GetAppLogic()->GetAudioRenderer() );
    TEST_ACCESSOR( VideoInputHandler )::UnregisterInputs( handler );

    EXPECT_FALSE( inputSlots->AccessSource( 0 ).IsValid() );
    EXPECT_FALSE( inputSlots->AccessSource( 1 ).IsValid() );
}


