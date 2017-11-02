#include "Framework/FrameworkTest.h"


#include "Engine/Models/Timeline/Dynamic/DefaultTimeline.h"
#include "Engine/Models/Timeline/Dynamic/TimelineEventStop.h"
#include "Engine/Models/Timeline/Dynamic/TimelineEventLoop.h"
#include "Engine/Models/Timeline/Dynamic/TimelineEventNull.h"
#include "Engine/Models/Timeline/Dynamic/TimelineEventTrigger.h"


using namespace bv::model;




// ***********************
//
template< typename KeyframeType >
void        TestBaseKeyframe        ( bv::TimelineEventType type, KeyframeType& keyframe, const std::string & name )
{
    ASSERT_TRUE( keyframe );

    EXPECT_EQ( keyframe->GetEventTime(), 1.0f );
    EXPECT_EQ( keyframe->GetName(), name );
    EXPECT_EQ( keyframe->GetOwnerTimeline(), nullptr );
    EXPECT_EQ( keyframe->GetType(), type );
    EXPECT_EQ( keyframe->IsActive(), true );

    keyframe->SetEventTime( 2.0f );
    EXPECT_EQ( keyframe->GetEventTime(), 2.0f );
}




// ***********************
//
SIMPLE_FRAMEWORK_TEST_IN_SUITE( Model_Timelines, AddStopKeyframe )
{
    auto keyframe = TimelineEventStop::Create( "StopKeyframe", 1.0f );
    TestBaseKeyframe( bv::TimelineEventType::TET_STOP, keyframe, "StopKeyframe" );

    if( HasFatalFailure() )
        return;
}


// ***********************
//
SIMPLE_FRAMEWORK_TEST_IN_SUITE( Model_Timelines, AddGotoKeyframe )
{
    auto keyframe = TimelineEventLoop::Create( "GotoKeyframe", 1.0f, bv::LoopEventAction::LEA_GOTO, 300, 1.2f, nullptr );
    TestBaseKeyframe( bv::TimelineEventType::TET_LOOP, keyframe, "GotoKeyframe" );

    if( HasFatalFailure() )
        return;

    EXPECT_EQ( keyframe->GetActionType(), bv::LoopEventAction::LEA_GOTO );
    EXPECT_EQ( keyframe->GetTotalLoopCount(), 300 );
    EXPECT_EQ( keyframe->GetTargetTime(), 1.2f );

    keyframe->SetTargetTime( 1.4f );
    EXPECT_EQ( keyframe->GetTargetTime(), 1.4f );
}

// ***********************
//
SIMPLE_FRAMEWORK_TEST_IN_SUITE( Model_Timelines, AddMultipleKeyframe )
{
    auto timeline = DefaultTimeline::Create( "Timeline", bv::TimeType( 100000000000.0 ), bv::TimelineWrapMethod::TWM_CLAMP, bv::TimelineWrapMethod::TWM_CLAMP );


    // Stop keyframe
    auto stopKeyframe = TimelineEventStop::Create( "StopKeyframe", 1.0f, timeline.get() );
    timeline->AddKeyFrame( stopKeyframe );

    EXPECT_EQ( stopKeyframe->GetOwnerTimeline(), timeline.get() );

    ASSERT_EQ( timeline->NumKeyFrames(), 1 );
    EXPECT_EQ( timeline->GetKeyFrameEvent( 0 ), stopKeyframe.get() );
    EXPECT_EQ( timeline->GetKeyFrameEvent( "StopKeyframe" ), stopKeyframe.get() );


    // Add goto Loop keyframe before stop keyframe
    auto gotoKeyframe = TimelineEventLoop::Create( "GotoKeyframe", 0.5f, bv::LoopEventAction::LEA_GOTO, std::numeric_limits< unsigned int >::max(), 0.65f, timeline.get() );
    ASSERT_TRUE( timeline->AddKeyFrame( gotoKeyframe ) );

    EXPECT_EQ( gotoKeyframe->GetOwnerTimeline(), timeline.get() );

    ASSERT_EQ( timeline->NumKeyFrames(), 2 );
    EXPECT_EQ( timeline->GetKeyFrameEvent( 0 ), gotoKeyframe.get() );
    EXPECT_EQ( timeline->GetKeyFrameEvent( "GotoKeyframe" ), gotoKeyframe.get() );

    // Add restart Loop keyframe goto after stop keyframe
    auto restartKeyframe = TimelineEventLoop::Create( "RestartKeyframe", 1.5f, bv::LoopEventAction::LEA_RESTART, std::numeric_limits< unsigned int >::max(), 0.65f, timeline.get() );
    timeline->AddKeyFrame( restartKeyframe );

    ASSERT_EQ( timeline->NumKeyFrames(), 3 );
    EXPECT_EQ( timeline->GetKeyFrameEvent( 2 ), restartKeyframe.get() );
    EXPECT_EQ( timeline->GetKeyFrameEvent( "RestartKeyframe" ), restartKeyframe.get() );

    // Try to get non existing keyframe
    EXPECT_EQ( timeline->GetKeyFrameEvent( 3 ), nullptr );
}


// ***********************
// Keyframe stops timeline. Timeline is stoped until someone calls play.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( Model_Timelines, StopOnKeyframe )
{
    auto timeline = DefaultTimeline::Create( "Timeline", bv::TimeType( 100000000000.0 ), bv::TimelineWrapMethod::TWM_CLAMP, bv::TimelineWrapMethod::TWM_CLAMP );
    auto stopKeyframe = TimelineEventStop::Create( "StopKeyframe", 1.0f, timeline.get() );
    ASSERT_TRUE( timeline->AddKeyFrame( stopKeyframe ) );

    ASSERT_TRUE( timeline );
    ASSERT_TRUE( stopKeyframe );

    timeline->Play();

    // Simulate bv updates
    timeline->SetGlobalTime( bv::TimeType( 0.0f ) );
    timeline->SetGlobalTime( bv::TimeType( 0.5f ) );
    timeline->SetGlobalTime( bv::TimeType( 0.8f ) );
    timeline->SetGlobalTime( bv::TimeType( 1.2f ) );
    timeline->SetGlobalTime( bv::TimeType( 1.6f ) );

    // Here StopKeyframe should stop timeline
    EXPECT_EQ( timeline->GetLocalTime(), 1.0f );

    timeline->Play();
    timeline->SetGlobalTime( bv::TimeType( 1.65f ) );

    EXPECT_TRUE( abs( timeline->GetLocalTime() - 1.05f ) < 0.00001 );
}

// ***********************
// Timeline should jump one time on keframe. In next loop it should pass keyframe time value.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( Model_Timelines, GotoOnKeyframe )
{
    auto timeline = DefaultTimeline::Create( "Timeline", bv::TimeType( 100000000000.0 ), bv::TimelineWrapMethod::TWM_CLAMP, bv::TimelineWrapMethod::TWM_CLAMP );
    auto gotoKeyframe = TimelineEventLoop::Create( "GotoKeyframe", 1.0f, bv::LoopEventAction::LEA_GOTO, 1, 0.2f, timeline.get() );
    ASSERT_TRUE( timeline->AddKeyFrame( gotoKeyframe ) );

    ASSERT_TRUE( timeline );
    ASSERT_TRUE( gotoKeyframe );

    timeline->Play();

    // Simulate bv updates
    timeline->SetGlobalTime( bv::TimeType( 0.0f ) );
    timeline->SetGlobalTime( bv::TimeType( 0.5f ) );
    timeline->SetGlobalTime( bv::TimeType( 0.8f ) );
    timeline->SetGlobalTime( bv::TimeType( 1.2f ) );

    // Keyframe should jump to time 0.2f
    EXPECT_TRUE( abs( timeline->GetLocalTime() - 0.2f ) < 0.00001 );

    // Simulate bv updates
    timeline->SetGlobalTime( bv::TimeType( 1.7f ) );
    timeline->SetGlobalTime( bv::TimeType( 2.0f ) );
    timeline->SetGlobalTime( bv::TimeType( 2.4f ) );

    // Keyframe shouldn't jump this time since loop count is set to 1.
    EXPECT_TRUE( abs( timeline->GetLocalTime() - 1.4f ) < 0.00001 );
}


// ***********************
//
SIMPLE_FRAMEWORK_TEST_IN_SUITE( Model_Timelines, RestartKeyframe )
{
    auto timeline = DefaultTimeline::Create( "Timeline", bv::TimeType( 100000000000.0 ), bv::TimelineWrapMethod::TWM_CLAMP, bv::TimelineWrapMethod::TWM_CLAMP );
    auto restartKeyframe = TimelineEventLoop::Create( "RestartKeyframe", 1.0f, bv::LoopEventAction::LEA_RESTART, 1, 0.2f, timeline.get() );
    ASSERT_TRUE( timeline->AddKeyFrame( restartKeyframe ) );

    ASSERT_TRUE( timeline );
    ASSERT_TRUE( restartKeyframe );

    timeline->Play();

    // Simulate bv updates
    timeline->SetGlobalTime( bv::TimeType( 0.0f ) );
    timeline->SetGlobalTime( bv::TimeType( 0.5f ) );
    timeline->SetGlobalTime( bv::TimeType( 1.2f ) );

    // Keyframe should restart timeline. Local time should be equal to 0.0.
    EXPECT_TRUE( abs( timeline->GetLocalTime() - 0.0f ) < 0.00001 );

    // Timeline is started after Restart.
    timeline->SetGlobalTime( bv::TimeType( 1.4f ) );
    EXPECT_TRUE( abs( timeline->GetLocalTime() - 0.2f ) < 0.00001 );
}


// ***********************
//
SIMPLE_FRAMEWORK_TEST_IN_SUITE( Model_Timelines, ReverseKeyframe )
{
    auto timeline = DefaultTimeline::Create( "Timeline", bv::TimeType( 100000000000.0 ), bv::TimelineWrapMethod::TWM_CLAMP, bv::TimelineWrapMethod::TWM_CLAMP );
    auto restartKeyframe = TimelineEventLoop::Create( "ReversesKeyframe", 1.0f, bv::LoopEventAction::LEA_REVERSE, 1, 0.2f, timeline.get() );
    ASSERT_TRUE( timeline->AddKeyFrame( restartKeyframe ) );

    ASSERT_TRUE( timeline );
    ASSERT_TRUE( restartKeyframe );

    timeline->Play();

    // Simulate bv updates
    timeline->SetGlobalTime( bv::TimeType( 0.0f ) );
    timeline->SetGlobalTime( bv::TimeType( 0.5f ) );
    timeline->SetGlobalTime( bv::TimeType( 1.2f ) );

    // Keyframe should reverse timeline direction.
    // FIXME: This test checks currently implemented behavior. Maybe timeline shouldn't reach 1.2
    // and time should be equal to 0.8 here.
    EXPECT_TRUE( abs( timeline->GetLocalTime() - 1.2f ) < 0.00001 );

    timeline->SetGlobalTime( bv::TimeType( 1.4f ) );
    EXPECT_TRUE( abs( timeline->GetLocalTime() - 1.0f ) < 0.00001 );


    timeline->SetGlobalTime( bv::TimeType( 1.8f ) );
    EXPECT_TRUE( abs( timeline->GetLocalTime() - 0.6f ) < 0.00001 );
}


// ***********************
// Bug https://www.pivotaltracker.com/story/show/148410315
// Sending events sequence Goto -> Play doesn't work in debug mode. The reason is too low FPS
// which causes that time offset between first and second frame after clicking play is greater then GEvtTimeSeparation
// from DefaultTimeline.cpp file. SetGlobalTime function which is called in second update, first sets field
// m_triggeredEvent to nullptr, and then tries to find next keyframe event. Since m_prevTime is still 0, function
// CurrentEvent chooses the same stop event which stops timeline.
// Note that if time offset between frames is less then GEvtTimeSeparation, stop event will be deactivated in third frame
// and bug wouldn't occur.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( Model_Timelines, StopOnKeyframeAndPlay )
{
    auto timeline = DefaultTimeline::Create( "Timeline", bv::TimeType( 100000000000.0 ), bv::TimelineWrapMethod::TWM_CLAMP, bv::TimelineWrapMethod::TWM_CLAMP );
    auto stopKeyframe = TimelineEventStop::Create( "StopKeyframe", 0.2f, timeline.get() );
    ASSERT_TRUE( timeline->AddKeyFrame( stopKeyframe ) );

    ASSERT_TRUE( timeline );
    ASSERT_TRUE( stopKeyframe );

    timeline->Play();

    // Simulate bv updates
    timeline->SetGlobalTime( bv::TimeType( 0.0f ) );
    timeline->SetGlobalTime( bv::TimeType( 0.2f ) );
    timeline->SetGlobalTime( bv::TimeType( 0.4f ) );        // Note: without this line bug doesn't occur. m_prevTime is 0.2f then.

    // Here StopKeyframe should stop timeline
    EXPECT_EQ( timeline->GetLocalTime(), 0.2f );

    // This offset should be greater than GEvtTimeSeparation in DefaultTimeline.cpp to see this bug.
    bv::TimeType timelineOffset = 0.41f;
    bv::TimeType stopDurationOffset = timelineOffset + 0.4f;

    timeline->Play();
    timeline->SetGlobalTime( bv::TimeType( stopDurationOffset ) );

    EXPECT_TRUE( abs( timeline->GetLocalTime() - ( timelineOffset + 0.2f ) ) < 0.00001 );
}


// ***********************
// If there are two keyframes to near from each other, the second can be lost.
// Note: this is example problem. This bug has never occured in real situation but we need to think what
// to do in this case.
SIMPLE_FRAMEWORK_TEST_IN_SUITE( Model_Timelines, TwoNearKeyframes )
{
    auto timeline = DefaultTimeline::Create( "Timeline", bv::TimeType( 100000000000.0 ), bv::TimelineWrapMethod::TWM_CLAMP, bv::TimelineWrapMethod::TWM_CLAMP );
    ASSERT_NE( timeline, nullptr );

    // Add fake keyframe
    auto nullKeyframe = TimelineEventNull::Create( "NullKeyframe", 0.2f, timeline.get() );
    ASSERT_TRUE( timeline->AddKeyFrame( nullKeyframe ) );

    // Add stop keyframe which could be lost.
    auto stopKeyframe = TimelineEventStop::Create( "StopKeyframe", 0.4f, timeline.get() );
    ASSERT_TRUE( timeline->AddKeyFrame( stopKeyframe ) );

    timeline->Play();

    // Simulate bv updates
    timeline->SetGlobalTime( bv::TimeType( 0.0f ) );
    timeline->SetGlobalTime( bv::TimeType( 0.6f ) );

    EXPECT_TRUE( abs( timeline->GetLocalTime() - 0.4f ) < 0.00001 );
}


