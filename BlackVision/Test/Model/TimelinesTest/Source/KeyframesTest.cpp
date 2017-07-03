#include "UnitTest++.h"

#include "Engine/Models/Timeline/Dynamic/DefaultTimeline.h"
#include "Engine/Models/Timeline/Dynamic/TimelineEventStop.h"
#include "Engine/Models/Timeline/Dynamic/TimelineEventLoop.h"


using namespace bv::model;


// ***********************
//
template< typename KeyframeType >
void        TestBaseKeyframe        ( bv::TimelineEventType type, KeyframeType& keyframe, const std::string & name )
{
    REQUIRE( keyframe );

    CHECK( keyframe->GetEventTime() == 1.0f );
    CHECK( keyframe->GetName() == name );
    CHECK( keyframe->GetOwnerTimeline() == nullptr );
    CHECK( keyframe->GetType() == type );
    CHECK( keyframe->IsActive() == true );

    keyframe->SetEventTime( 2.0f );
    CHECK( keyframe->GetEventTime() == 2.0f );
}


SUITE( DefaultTimelineTest )
{


TEST( StopKeyframe )
{
    auto keyframe = TimelineEventStop::Create( "StopKeyframe", 1.0f );
    TestBaseKeyframe( bv::TimelineEventType::TET_STOP, keyframe, "StopKeyframe" );
}

TEST( GotoKeyframe )
{
    auto keyframe = TimelineEventLoop::Create( "GotoKeyframe", 1.0f, bv::LoopEventAction::LEA_GOTO, 300, 1.2f, nullptr );
    TestBaseKeyframe( bv::TimelineEventType::TET_LOOP, keyframe, "GotoKeyframe" );

    CHECK( keyframe->GetActionType() == bv::LoopEventAction::LEA_GOTO );
    CHECK( keyframe->GetTotalLoopCount() == 300 );
    CHECK( keyframe->GetTargetTime() == 1.2f );
    
    keyframe->SetTargetTime( 1.4f );
    CHECK( keyframe->GetTargetTime() == 1.4f );
}


TEST( AddKeyframe )
{
    auto timeline = DefaultTimeline::Create( "Timeline", bv::TimeType( 100000000000.0 ), bv::TimelineWrapMethod::TWM_CLAMP, bv::TimelineWrapMethod::TWM_CLAMP );


    // Stop keyframe
    auto stopKeyframe = TimelineEventStop::Create( "StopKeyframe", 1.0f, timeline.get() );
    timeline->AddKeyFrame( stopKeyframe );

    CHECK( stopKeyframe->GetOwnerTimeline() == timeline.get() );

    REQUIRE( timeline->NumKeyFrames() == 1 );
    CHECK( timeline->GetKeyFrameEvent( 0 ) == stopKeyframe.get() );
    CHECK( timeline->GetKeyFrameEvent( "StopKeyframe" ) == stopKeyframe.get() );


    // Add goto Loop keyframe before stop keyframe
    auto gotoKeyframe = TimelineEventLoop::Create( "GotoKeyframe", 0.5f, bv::LoopEventAction::LEA_GOTO, std::numeric_limits< unsigned int >::max(), 0.65f, timeline.get() );
    timeline->AddKeyFrame( gotoKeyframe );

    CHECK( gotoKeyframe->GetOwnerTimeline() == timeline.get() );

    REQUIRE( timeline->NumKeyFrames() == 2 );
    CHECK( timeline->GetKeyFrameEvent( 0 ) == gotoKeyframe.get() );
    CHECK( timeline->GetKeyFrameEvent( "GotoKeyframe" ) == gotoKeyframe.get() );

    // Add restart Loop keyframe goto after stop keyframe
    auto restartKeyframe = TimelineEventLoop::Create( "RestartKeyframe", 1.5f, bv::LoopEventAction::LEA_RESTART, std::numeric_limits< unsigned int >::max(), 0.65f, timeline.get() );
    timeline->AddKeyFrame( restartKeyframe );

    REQUIRE( timeline->NumKeyFrames() == 3 );
    CHECK( timeline->GetKeyFrameEvent( 2 ) == restartKeyframe.get() );
    CHECK( timeline->GetKeyFrameEvent( "RestartKeyframe" ) == restartKeyframe.get() );

    // Try to get non existing keyframe
    CHECK( timeline->GetKeyFrameEvent( 3 ) == nullptr );
}


TEST( StopOnKeyframe )
{
    auto timeline = DefaultTimeline::Create( "Timeline", bv::TimeType( 100000000000.0 ), bv::TimelineWrapMethod::TWM_CLAMP, bv::TimelineWrapMethod::TWM_CLAMP );
    auto stopKeyframe = TimelineEventStop::Create( "StopKeyframe", 1.0f, timeline.get() );
    timeline->AddKeyFrame( stopKeyframe );

    REQUIRE( timeline );
    REQUIRE( stopKeyframe );

    timeline->Play();

    // Simulate bv updates
    timeline->SetGlobalTime( bv::TimeType( 0.0f ) );
    timeline->SetGlobalTime( bv::TimeType( 0.5f ) );
    timeline->SetGlobalTime( bv::TimeType( 0.8f ) );
    timeline->SetGlobalTime( bv::TimeType( 1.2f ) );

    CHECK( timeline->GetLocalTime() == 1.0f );
    
    timeline->Play();
    timeline->SetGlobalTime( bv::TimeType( 1.6f ) );

    CHECK( abs( timeline->GetLocalTime() - 1.4f ) < 0.00001 );
}


}

