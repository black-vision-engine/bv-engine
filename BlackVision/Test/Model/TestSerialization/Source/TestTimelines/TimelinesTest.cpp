#include "gtest/gtest.h"

#include "Engine/Models/Timeline/TimelineHelper.h"

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
