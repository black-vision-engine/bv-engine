#include "gtest/gtest.h"
#include "Framework/FrameworkTest.h"
#include "Framework/BVTestAppLogic.h"

#include "Engine/Graphics/InputSlots/SlotsLogic/VideoInput/VideoInputSlots.h"
#include "Engine/Graphics/InputSlots/Logic/Handlers/VideoInputHandler.h"

#include "Utils/Accessors/Engine/Inputs/VideoInputHandlerAccessor.h"

#include "VideoInput/VideoInputChannelDesc.h"
#include "Helpers/AVFrameHelpers.h"

#include "Utils/Scenes/TestScenesCreator.h"
#include "Utils/Nodes/TestNodesCreator.h"

#include "Utils/Mocks/FakeVideoCard/FakeVideoCard.h"

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


// ========================================================================= //
// Disabling/Enabling video cards
// ========================================================================= //

// ***********************
// Video input should be enabled only if someone uses it. VideoInputHandler class is responsible for
// listening to events which inform about releasing and accessing input slots. If slot isn't used
// class should disable video input processing on video card for better performance.
POST_RENDER_FRAMEWORK_TEST( Engine_InputSlots, VideoInputHandler_DisableUnusedVideoInputs )
{
    // Note: ommit frame 0. Video input events come one frame later and it is posible that other test
    // used vide inputs. This could lead to failures in this test.
    if( GetFrameNumber() == 1 )
    {
        auto card1 = std::static_pointer_cast< videocards::FakeVideoCard >( GetAppLogic()->GetVideoCardManager()->GetVideoCard( 0 ) );
        auto card2 = std::static_pointer_cast< videocards::FakeVideoCard >( GetAppLogic()->GetVideoCardManager()->GetVideoCard( 1 ) );
        
        // On the beginning all inputs are disabled.
        EXPECT_FALSE( card1->IsEnabled( 0 ) );
        EXPECT_FALSE( card1->IsEnabled( 1 ) );
        EXPECT_FALSE( card2->IsEnabled( 2 ) );

        // Add VideoInputPlugin which accesses inputs.
        TestScenesCreator::VideoInputRectangle( GetProjectEditor(), "VideoInput1", 400, 400, 0 );
        TestScenesCreator::VideoInputRectangle( GetProjectEditor(), "VideoInput2", 400, 400, 1 );
        TestScenesCreator::VideoInputRectangle( GetProjectEditor(), "VideoInput3", 400, 400, 2 );
    }
    else if( GetFrameNumber() == 2 )
    {
        auto card1 = std::static_pointer_cast< videocards::FakeVideoCard >( GetAppLogic()->GetVideoCardManager()->GetVideoCard( 0 ) );
        auto card2 = std::static_pointer_cast< videocards::FakeVideoCard >( GetAppLogic()->GetVideoCardManager()->GetVideoCard( 1 ) );

        // In previous frame we accessed video inputs from plugins, so inputs shoud be enabled.
        EXPECT_TRUE( card1->IsEnabled( 0 ) );
        EXPECT_TRUE( card1->IsEnabled( 1 ) );
        EXPECT_TRUE( card2->IsEnabled( 2 ) );

        // Remove scene with on of video inputs. In next frame this input should be disabled.
        GetProjectEditor()->RemoveScene( "VideoInput2" );
    }
    else if( GetFrameNumber() == 3 )
    {
        auto card1 = std::static_pointer_cast< videocards::FakeVideoCard >( GetAppLogic()->GetVideoCardManager()->GetVideoCard( 0 ) );
        auto card2 = std::static_pointer_cast< videocards::FakeVideoCard >( GetAppLogic()->GetVideoCardManager()->GetVideoCard( 1 ) );

        EXPECT_TRUE( card1->IsEnabled( 0 ) );
        EXPECT_FALSE( card1->IsEnabled( 1 ) );
        EXPECT_TRUE( card2->IsEnabled( 2 ) );
    }

}





