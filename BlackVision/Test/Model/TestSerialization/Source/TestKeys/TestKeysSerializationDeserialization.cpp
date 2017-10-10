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
    CompositeInterpolator< float, TimeType > expectedInterpolator;

    expectedInterpolator.AddKey( 0.0f, 3.0f );
    expectedInterpolator.AddKey( 1.0f, 3.0f );
    expectedInterpolator.AddKey( 3.0f, 13.0f );
    expectedInterpolator.AddKey( 4.0f, 2.0f );
    expectedInterpolator.AddKey( 7.0f, 5.0f );

    Serialize( expectedInterpolator, "SerDeserFloat.xml" );

    auto actualInterpolator = Deserialize< CompositeInterpolator< float, TimeType > >( "SerDeserFloat.xml", "interpolator" );

    EXPECT_TRUE( ParamComparator::CompareKeys( expectedInterpolator, *( actualInterpolator.get() ) ) );
    EXPECT_EQ( actualInterpolator->GetKeys().size(), 5 );
}
