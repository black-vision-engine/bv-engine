#include "gtest/gtest.h"


#include "Mathematics/Interpolators/CompositeInterpolator.h"
#include "Mathematics/Transform/MatTransform.h"

#include "Utils/Comparators/ParamComparator.h"
#include "Utils/Serialization/Serialize.h"

#include "Mathematics/glm_inc.h"


using namespace bv;



// ========================================================================= //
// Creating keys with different type than expected. If time value is valid
// keys are created with default value for expected type.
// Note: this test doesn't check all posibilities to provide not parsing key.
// Testing this is responsibility of SerializationHelper::String2T tests.
// ========================================================================= //




// ***********************
// Keys that cannot be parsed to expected type are added with default values as placeholders.
// But this happens only if time value is valid.
TEST( Serialization_ParamValModel, Keys_Float_DifferentKeyTypeInXML )
{
    auto actual = Deserialize< CompositeInterpolator< TimeType, float > >( "TestAssets/Serialization/Keys/DifferentKeyType.xml", "interpolator" );
    auto & keys = actual->GetKeys();

    ASSERT_EQ( keys.size(), 11 );

    EXPECT_EQ( keys[ 0 ].val, 3 );
    EXPECT_TRUE( abs( keys[ 1 ].val - 3.10101010 ) < std::numeric_limits< float >::epsilon() );
    EXPECT_EQ( keys[ 2 ].val, -13 );
    EXPECT_EQ( keys[ 3 ].val, 0.0 );    // Default
    EXPECT_EQ( keys[ 4 ].val, 0.0 );    // Default
    EXPECT_EQ( keys[ 5 ].val, 0.0 );    // Default
    EXPECT_EQ( keys[ 6 ].val, 0.0 );    // Default
    EXPECT_EQ( keys[ 7 ].val, 0.0 );    // Default
    EXPECT_EQ( keys[ 8 ].val, 0.0 );    // Default
    EXPECT_EQ( keys[ 9 ].val, 0.0 );    // Default
    EXPECT_EQ( keys[ 10 ].val, 0.0 );   // Default
}

// ***********************
// Keys that cannot be parsed to expected type are added with default values as placeholders.
// But this happens only if time value is valid.
TEST( Serialization_ParamValModel, Keys_Int_DifferentKeyTypeInXML )
{
    auto actual = Deserialize< CompositeInterpolator< TimeType, int > >( "TestAssets/Serialization/Keys/DifferentKeyType.xml", "interpolator" );
    auto & keys = actual->GetKeys();

    ASSERT_EQ( keys.size(), 11 );

    EXPECT_EQ( keys[ 0 ].val, 3 );
    EXPECT_EQ( keys[ 1 ].val, 0 );    // Default
    EXPECT_EQ( keys[ 2 ].val, -13 );
    EXPECT_EQ( keys[ 3 ].val, 0.0 );    // Default
    EXPECT_EQ( keys[ 4 ].val, 0.0 );    // Default
    EXPECT_EQ( keys[ 5 ].val, 0.0 );    // Default
    EXPECT_EQ( keys[ 6 ].val, 0.0 );    // Default
    EXPECT_EQ( keys[ 7 ].val, 0.0 );    // Default
    EXPECT_EQ( keys[ 8 ].val, 0.0 );    // Default
    EXPECT_EQ( keys[ 9 ].val, 0.0 );    // Default
    EXPECT_EQ( keys[ 10 ].val, 0.0 );   // Default
}

// ***********************
// Keys that cannot be parsed to expected type are added with default values as placeholders.
// But this happens only if time value is valid.
TEST( Serialization_ParamValModel, Keys_Bool_DifferentKeyTypeInXML )
{
    auto actual = Deserialize< CompositeInterpolator< TimeType, bool > >( "TestAssets/Serialization/Keys/DifferentKeyType.xml", "interpolator" );
    auto & keys = actual->GetKeys();

    ASSERT_EQ( keys.size(), 11 );

    EXPECT_EQ( keys[ 0 ].val, false );    // Default
    EXPECT_EQ( keys[ 1 ].val, false );    // Default
    EXPECT_EQ( keys[ 2 ].val, false );    // Default
    EXPECT_EQ( keys[ 3 ].val, false );    // Default
    EXPECT_EQ( keys[ 4 ].val, true );
    EXPECT_EQ( keys[ 5 ].val, false );
    EXPECT_EQ( keys[ 6 ].val, false );    // Default
    EXPECT_EQ( keys[ 7 ].val, false );    // Default
    EXPECT_EQ( keys[ 8 ].val, false );    // Default
    EXPECT_EQ( keys[ 9 ].val, false );    // Default
    EXPECT_EQ( keys[ 10 ].val, false );   // Default
}


