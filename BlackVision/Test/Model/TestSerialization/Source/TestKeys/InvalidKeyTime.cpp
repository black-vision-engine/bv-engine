#include "gtest/gtest.h"


#include "Mathematics/Interpolators/CompositeInterpolator.h"
#include "Mathematics/Transform/MatTransform.h"

#include "Utils/Comparators/ParamComparator.h"
#include "Utils/Serialization/Serialize.h"
#include "Serialization/SerializationHelper.h"

#include "Mathematics/glm_inc.h"


using namespace bv;



// ***********************
// Load xml with invalid time value. Such key should be omitted.
// This test checks keys with both value and time invalid numbers too.
// This test works on float but it should cover all other parameters types too.
TEST( Serialization_ParamValModel, Keys_Float_InvalidTime )
{
    auto actual = Deserialize< CompositeInterpolator< TimeType, float > >( "TestAssets/Serialization/Keys/InvalidTimeValues.xml", "interpolator" );
    auto & keys = actual->GetKeys();

    ASSERT_EQ( keys.size(), 2 );

    EXPECT_EQ( keys[ 0 ].val, 3 );
    EXPECT_EQ( keys[ 1 ].val, -13 );

}

// ***********************
// Load xml with invalid time keys values.
TEST( Serialization_ParamValModel, Keys_Float_AllInvalidKeys )
{
    auto actual = Deserialize< CompositeInterpolator< TimeType, float > >( "TestAssets/Serialization/Keys/AllInvalidKeys.xml", "interpolator" );
    auto & keys = actual->GetKeys();

    ASSERT_EQ( keys.size(), 0 );
}

// ***********************
// If there are two keys in the same time, the second key overrides first.
TEST( Serialization_ParamValModel, Keys_TwoKeysInTheSameTime )
{
    auto actual = Deserialize< CompositeInterpolator< TimeType, float > >( "TestAssets/Serialization/Keys/KeysInTheSameTime.xml", "interpolator" );
    auto & keys = actual->GetKeys();

    ASSERT_EQ( keys.size(), 4 );

    EXPECT_EQ( keys[ 2 ].t, 2.0f );
    EXPECT_EQ( keys[ 2 ].val, -13.0f );

    EXPECT_EQ( keys[ 3 ].t, 4.0f );
    EXPECT_EQ( keys[ 3 ].val, 5.0f );
}

// ***********************
// Keys should be sorted by time. If they are not deserialization should give them right order.
// Order of interpolators in this case will be tested in interpolators tests.
TEST( Serialization_ParamValModel, Keys_InvalidKeysOrder )
{
    auto actual = Deserialize< CompositeInterpolator< TimeType, float > >( "TestAssets/Serialization/Keys/BadKeysOrder.xml", "interpolator" );
    auto & keys = actual->GetKeys();

    ASSERT_EQ( keys.size(), 4 );

    EXPECT_EQ( keys[ 0 ].t, 0.0f );
    EXPECT_EQ( keys[ 0 ].val, 3.0f );

    EXPECT_EQ( keys[ 1 ].t, 1.5f );
    EXPECT_EQ( keys[ 1 ].val, 4.0f );

    EXPECT_EQ( keys[ 2 ].t, 2.0f );
    EXPECT_EQ( keys[ 2 ].val, 5.0f );

    EXPECT_EQ( keys[ 3 ].t, 4.0f );
    EXPECT_EQ( keys[ 3 ].val, -13.0f );
}


