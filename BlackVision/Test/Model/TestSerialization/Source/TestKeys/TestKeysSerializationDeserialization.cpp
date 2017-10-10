#include "gtest/gtest.h"

#include "Mathematics/Interpolators/CompositeInterpolator.h"

#include "Utils/Comparators/ParamComparator.h"
#include "Utils/Serialization/Serialize.h"

using namespace bv;


// ========================================================================= //
// Simple serialization - deserialization - comparision tests
//                      for different types
// ========================================================================= //


// ***********************
//
TEST( Serialization_ParamValModel, Keys_SerializeDeserializeFloat )
{
    CompositeInterpolator< TimeType, float > expectedInterpolator;

    expectedInterpolator.AddKey( 0.0f, 3.0f );
    expectedInterpolator.AddKey( 1.0f, 3.0f );
    expectedInterpolator.AddKey( 3.0f, 13.0f );
    expectedInterpolator.AddKey( 4.0f, 2.0f );
    expectedInterpolator.AddKey( 7.0f, 5.0f );

    Serialize( expectedInterpolator, "SerDeserFloat.xml" );

    auto actualInterpolator = Deserialize< CompositeInterpolator< TimeType, float > >( "SerDeserFloat.xml", "interpolator" );

    EXPECT_TRUE( ParamComparator::CompareKeys( expectedInterpolator, *( actualInterpolator.get() ) ) );
    EXPECT_EQ( actualInterpolator->GetKeys().size(), 5 );
}

// ***********************
//
TEST( Serialization_ParamValModel, Keys_SerializeDeserializeInt )
{
    CompositeInterpolator< TimeType, int > expectedInterpolator;

    expectedInterpolator.AddKey( 0.0f, 1 );
    expectedInterpolator.AddKey( 1.0f, 3 );
    expectedInterpolator.AddKey( 3.0f, 7 );
    expectedInterpolator.AddKey( 4.0f, 2 );
    expectedInterpolator.AddKey( 7.0f, 10 );

    Serialize( expectedInterpolator, "SerDeserInt.xml" );

    auto actualInterpolator = Deserialize< CompositeInterpolator< TimeType, int > >( "SerDeserInt.xml", "interpolator" );

    EXPECT_TRUE( ParamComparator::CompareKeys( expectedInterpolator, *( actualInterpolator.get() ) ) );
    EXPECT_EQ( actualInterpolator->GetKeys().size(), 5 );
}

// ***********************
//
TEST( Serialization_ParamValModel, Keys_SerializeDeserializeBool )
{
    CompositeInterpolator< TimeType, bool > expectedInterpolator;

    expectedInterpolator.AddKey( 0.0f, true );
    expectedInterpolator.AddKey( 1.0f, false );
    expectedInterpolator.AddKey( 3.0f, false );
    expectedInterpolator.AddKey( 4.0f, true );
    expectedInterpolator.AddKey( 7.0f, false );

    Serialize( expectedInterpolator, "SerDeserInt.xml" );

    auto actualInterpolator = Deserialize< CompositeInterpolator< TimeType, bool > >( "SerDeserInt.xml", "interpolator" );

    EXPECT_TRUE( ParamComparator::CompareKeys( expectedInterpolator, *( actualInterpolator.get() ) ) );
    EXPECT_EQ( actualInterpolator->GetKeys().size(), 5 );
}

