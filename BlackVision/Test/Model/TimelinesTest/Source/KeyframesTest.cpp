#include "Framework/FrameworkTest.h"


#include "Engine/Models/Timeline/Dynamic/DefaultTimeline.h"
#include "Engine/Models/Timeline/Dynamic/TimelineEventStop.h"
#include "Engine/Models/Timeline/Dynamic/TimelineEventLoop.h"
#include "Engine/Models/Timeline/Dynamic/TimelineEventNull.h"
#include "Engine/Models/Timeline/Dynamic/TimelineEventTrigger.h"

#include "Utils/Serialization/Serialize.h"

using namespace bv;
using namespace bv::model;



const double epsilon = 0.00001;

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

	EXPECT_NEAR( timeline->GetLocalTime(), 1.05f, epsilon );
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
    timeline->SetGlobalTime( bv::TimeType( 1.0f ) );

    // Keyframe should jump to time 0.2f
	EXPECT_NEAR( timeline->GetLocalTime(), 0.2f, epsilon );

    // Simulate bv updates
    timeline->SetGlobalTime( bv::TimeType( 1.7f ) );
    timeline->SetGlobalTime( bv::TimeType( 2.0f ) );
    timeline->SetGlobalTime( bv::TimeType( 2.4f ) );

    // Keyframe shouldn't jump this time since loop count is set to 1.
	EXPECT_NEAR( timeline->GetLocalTime(), 1.6f, epsilon );
}


// ***********************
//
SIMPLE_FRAMEWORK_TEST_IN_SUITE( Model_Timelines, RestartKeyframe )
{
    auto timeline = DefaultTimeline::Create( "Timeline", bv::TimeType( 100000000000.0 ), bv::TimelineWrapMethod::TWM_CLAMP, bv::TimelineWrapMethod::TWM_CLAMP );
    auto restartKeyframe = TimelineEventLoop::Create( "RestartKeyframe", 1.0f, bv::LoopEventAction::LEA_RESTART, 1 );
    ASSERT_TRUE( timeline->AddKeyFrame( restartKeyframe ) );

    ASSERT_TRUE( timeline );
    ASSERT_TRUE( restartKeyframe );

    timeline->Play();

    // Simulate bv updates
    timeline->SetGlobalTime( bv::TimeType( 0.0f ) );
    timeline->SetGlobalTime( bv::TimeType( 0.5f ) );
    timeline->SetGlobalTime( bv::TimeType( 1.0f ) );

    // Keyframe should restart timeline. Local time should be equal to 0.0.
	EXPECT_NEAR( timeline->GetLocalTime(), 0.0f, epsilon );

    // Timeline is started after Restart.
    timeline->SetGlobalTime( bv::TimeType( 1.4f ) );
	EXPECT_NEAR( timeline->GetLocalTime(), 0.4f, epsilon );
}


// ***********************
//
SIMPLE_FRAMEWORK_TEST_IN_SUITE( Model_Timelines, ReverseKeyframe )
{
    auto timeline = DefaultTimeline::Create( "Timeline", bv::TimeType( 100000000000.0 ), bv::TimelineWrapMethod::TWM_CLAMP, bv::TimelineWrapMethod::TWM_CLAMP );
    auto restartKeyframe = TimelineEventLoop::Create( "ReversesKeyframe", 1.0f, bv::LoopEventAction::LEA_REVERSE, 1 );
    ASSERT_TRUE( timeline->AddKeyFrame( restartKeyframe ) );

    ASSERT_TRUE( timeline );
    ASSERT_TRUE( restartKeyframe );

    timeline->Play();

    // Simulate bv updates
    timeline->SetGlobalTime( bv::TimeType( 0.0f ) );
    timeline->SetGlobalTime( bv::TimeType( 0.5f ) );
	timeline->SetGlobalTime( bv::TimeType( 1.0f ) );
	EXPECT_NEAR( timeline->GetLocalTime(), 1.0f, epsilon );

	timeline->SetGlobalTime( bv::TimeType( 1.2f ) );
	EXPECT_NEAR( timeline->GetLocalTime(), 0.8f, epsilon );


    timeline->SetGlobalTime( bv::TimeType( 1.8f ) );
    EXPECT_NEAR( timeline->GetLocalTime(), 0.2f, epsilon );
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
    auto stopKeyframe = TimelineEventStop::Create( "StopKeyframe", 0.02f, timeline.get() );
    ASSERT_TRUE( timeline->AddKeyFrame( stopKeyframe ) );

    ASSERT_TRUE( timeline );
    ASSERT_TRUE( stopKeyframe );

    timeline->Play();

    // Simulate bv updates
    timeline->SetGlobalTime( bv::TimeType( 0.0f ) );
    timeline->SetGlobalTime( bv::TimeType( 0.02f ) );
    timeline->SetGlobalTime( bv::TimeType( 0.04f ) );        // Note: without this line bug doesn't occur. m_prevTime is 0.2f then.

    // Here StopKeyframe should stop timeline
    EXPECT_EQ( timeline->GetLocalTime(), 0.02f );

    // This offset should be greater than GEvtTimeSeparation in DefaultTimeline.cpp to see this bug.
    bv::TimeType timelineOffset = 0.041f;
    bv::TimeType stopDurationOffset = timelineOffset + 0.04f;

    timeline->Play();
    timeline->SetGlobalTime( bv::TimeType( stopDurationOffset ) );

    EXPECT_NEAR( timeline->GetLocalTime(), timelineOffset + 0.02f, epsilon );
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

    EXPECT_NEAR( timeline->GetLocalTime(), 0.4, epsilon );
}

// ***********************
//
SIMPLE_FRAMEWORK_TEST_IN_SUITE( Model_Timelines, StopAtZero )
{
	auto timeline = DefaultTimeline::Create( "Timeline", bv::TimeType( 100000000000.0 ), bv::TimelineWrapMethod::TWM_CLAMP, bv::TimelineWrapMethod::TWM_CLAMP );
	ASSERT_NE( timeline, nullptr );

	timeline->SetGlobalTime( 1 ); // update is needed

	EXPECT_NEAR( timeline->GetLocalTime(), 0, epsilon );

	auto stopKeyframe = TimelineEventStop::Create( "StopKeyframe", 0.0f, timeline.get() );
	ASSERT_TRUE( timeline->AddKeyFrame( stopKeyframe ) );

	timeline->SetGlobalTime( 3 );

	EXPECT_NEAR( timeline->GetLocalTime(), 0, epsilon );

	//timeline->Play();
	//
	//timeline->SetGlobalTime( 4 );

	timeline->Play();

	timeline->SetGlobalTime( 5 );

	ASSERT_NEAR( timeline->GetLocalTime(), 2, epsilon );
}

// ***********************
//
SIMPLE_FRAMEWORK_TEST_IN_SUITE( Model_Timelines, StopAtZeroViaSerPlayOn )
{
	auto timeline = Deserialize< DefaultTimeline >( "TestAssets/Keyframes/StopAtZeroPlayOn.xml", "timeline" );

	EXPECT_NEAR( timeline->GetLocalTime(), 1.5, epsilon ); // this is serialized local time

	timeline->SetGlobalTime( 1.f ); // this is the time that Play() in deserialization will start counting from

	EXPECT_NEAR( timeline->GetLocalTime(), 1.5, epsilon ); // so we still haven't moved on

	timeline->SetGlobalTime( 3.f ); // now we really moved on

	EXPECT_NEAR( timeline->GetLocalTime(), 3.5, epsilon );
}

// ***********************
//
SIMPLE_FRAMEWORK_TEST_IN_SUITE( Model_Timelines, StopAtZeroViaSerPlayOff )
{
	auto timeline = Deserialize< DefaultTimeline >( "TestAssets/Keyframes/StopAtZeroPlayOff.xml", "timeline" );
	EXPECT_NEAR( timeline->GetLocalTime(), 1.5, epsilon );

	timeline->SetGlobalTime( 1.f );
	EXPECT_NEAR( timeline->GetLocalTime(), 1.5, epsilon );

	timeline->SetGlobalTime( 2.f );
	EXPECT_NEAR( timeline->GetLocalTime(), 1.5, epsilon );

	timeline->Play();
	timeline->SetGlobalTime( 4.f ); // timeline starts in global time = 4

    timeline->SetGlobalTime( 6.f ); // so we need to move on

	EXPECT_NEAR( timeline->GetLocalTime(), 3.5, epsilon );
}

// ***********************
//
SIMPLE_FRAMEWORK_TEST_IN_SUITE( Model_Timelines, StopAtZeroViaSer )
{
	auto timeline = Deserialize< DefaultTimeline >( "TestAssets/Keyframes/StopAtZero.xml", "timeline" );
	EXPECT_NEAR( timeline->GetLocalTime(), 0, epsilon );

	timeline->SetGlobalTime( 1.f );
	EXPECT_NEAR( timeline->GetLocalTime(), 0, epsilon );

	timeline->SetGlobalTime( 2.f );
	EXPECT_NEAR( timeline->GetLocalTime(), 0, epsilon );

	timeline->Play();
	timeline->SetGlobalTime( 4.f );
	EXPECT_NEAR( timeline->GetLocalTime(), 2, epsilon );
}


// ***********************
//
SIMPLE_FRAMEWORK_TEST_IN_SUITE( Model_Timelines, GotoAtLocalTime )
{
	auto timeline = DefaultTimeline::Create( "Timeline", bv::TimeType( 100000000000.0 ), bv::TimelineWrapMethod::TWM_CLAMP, bv::TimelineWrapMethod::TWM_CLAMP );
	ASSERT_TRUE( timeline );
	timeline->SetGlobalTime( 1 ); // update is needed

	timeline->Play();
	timeline->SetGlobalTime( 3 );
	EXPECT_NEAR( timeline->GetLocalTime(), 2, epsilon );

	auto gotoKeyframe = TimelineEventLoop::Create( "GotoTest", 2, LoopEventAction::LEA_GOTO, 1, 1, timeline.get() );
	ASSERT_TRUE( timeline->AddKeyFrame( gotoKeyframe ) );

	EXPECT_NEAR( timeline->GetLocalTime(), 1, epsilon );

	timeline->SetGlobalTime( 4 );

	EXPECT_NEAR( timeline->GetLocalTime(), 2, epsilon ); // goto keyframe should not be fired this time

	timeline->SetGlobalTime( 6 );

	EXPECT_NEAR( timeline->GetLocalTime(), 4, epsilon );
}

// ***********************
//
SIMPLE_FRAMEWORK_TEST_IN_SUITE( Model_Timelines, ReverseKeyframeWithLag )
{
	FAIL(); // write when the behavious is specified
}