#include "gtest/gtest.h"

#include "Engine/Models/Timeline/TimelineHelper.h"
#include "Engine/Models/Timeline/Dynamic/TimelineEventStop.h"
#include "Engine/Models/Timeline/Dynamic/TimelineEventLoop.h"
#include "Engine/Models/Timeline/Dynamic/TimelineEventNull.h"
#include "Engine/Models/Timeline/Dynamic/TimelineEventTrigger.h"

#include "Utils/Comparators/TimelineComparator.h"

#include "Utils/Serialization/SerializeTimeline.h"


using namespace bv;
using namespace model;

TEST( Serialization_Timelines, SerDeser_TimelineHelperConst )
{
    ConstTimeEvaluatorPtr expectedTimeline = model::TimelineHelper::CreateConstTimeEvaluator( "ConstTest", 444.f );

    Serialize( *expectedTimeline, "SerDeserConst.xml" );

    auto actualTimeline = DeserializeTimeline( "SerDeserConst.xml" );

    ASSERT_TRUE( actualTimeline );
    EXPECT_TRUE( TimelineComparator::CompareTimelines( *expectedTimeline, *actualTimeline ) );
}

TEST( Serialization_Timelines, SerDeser_TimelineHelperOffset )
{
    auto expectedTimeline = TimelineHelper::CreateOffsetTimeEvaluator( "OffsetTest", 44.f, 21.37f );

    Serialize( *expectedTimeline, "SerDeserOffset.xml" );

    auto actualTimeline = DeserializeTimeline( "SerDeserOffset.xml" );

    ASSERT_TRUE( actualTimeline );
    EXPECT_TRUE( TimelineComparator::CompareTimelines( *expectedTimeline, *actualTimeline ) );
}

TEST( Serialization_Timelines, SerDeser_TimelineHelperDefault )
{
    auto expectedTimeline = TimelineHelper::CreateDefaultTimeline( "DefaultTest", 4.f, TimelineWrapMethod::TWM_MIRROR, TimelineWrapMethod::TWM_REPEAT );

    Serialize( *expectedTimeline, "SerDeserDefault.xml" );

    auto actualTimeline = DeserializeTimeline( "SerDeserDefault.xml" );

    ASSERT_TRUE( actualTimeline );
    EXPECT_TRUE( TimelineComparator::CompareTimelines( *expectedTimeline, *actualTimeline ) );
}

TEST( Serialization_Timelines, SerDeser_SomeKeyframes )
{
    auto expectedTimeline = TimelineHelper::CreateDefaultTimeline( "KeyframesTest", 10.f, TimelineWrapMethod::TWM_CLAMP, TimelineWrapMethod::TWM_MIRROR );

    auto stopKeyframe = TimelineEventStop::Create( "StopKeyframe", 1.0f, expectedTimeline.get() );
    expectedTimeline->AddKeyFrame( stopKeyframe );

    auto gotoKeyframe = TimelineEventLoop::Create( "GotoKeyframe", 0.5f, bv::LoopEventAction::LEA_GOTO, std::numeric_limits< unsigned int >::max(), 0.65f, expectedTimeline.get() );
    expectedTimeline->AddKeyFrame( gotoKeyframe );

    auto restartKeyframe = TimelineEventLoop::Create( "RestartKeyframe", 1.5f, bv::LoopEventAction::LEA_RESTART, std::numeric_limits< unsigned int >::max(), 0.65f, expectedTimeline.get() );
    expectedTimeline->AddKeyFrame( restartKeyframe );

    auto reverseKeyframe = TimelineEventLoop::Create( "ReversesKeyframe", 2.0f, bv::LoopEventAction::LEA_REVERSE, 1, 0.2f, expectedTimeline.get() );
    expectedTimeline->AddKeyFrame( reverseKeyframe );

    auto nullKeyframe = TimelineEventNull::Create( "NullKeyframe", 3.0f, expectedTimeline.get() );
    expectedTimeline->AddKeyFrame( nullKeyframe );

    auto triggerKeyframe = TimelineEventTrigger::Create( "TriggerKeyframe", 4.0f, "CoolEvents", expectedTimeline.get() );
    expectedTimeline->AddKeyFrame( triggerKeyframe );

    Serialize( *expectedTimeline, "SerDeserKeyframes.xml" );

    auto actualTimeline = DeserializeTimeline( "SerDeserKeyframes.xml" );

    ASSERT_TRUE( actualTimeline );
    EXPECT_TRUE( TimelineComparator::CompareTimelines( *expectedTimeline, *actualTimeline ) );
}
