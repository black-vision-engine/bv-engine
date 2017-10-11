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

// ***********************
// Keys that cannot be parsed to expected type are added with default values as placeholders.
// But this happens only if time value is valid.
TEST( Serialization_ParamValModel, Keys_Vec2_DifferentKeyTypeInXML )
{
    auto actual = Deserialize< CompositeInterpolator< TimeType, glm::vec2 > >( "TestAssets/Serialization/Keys/DifferentKeyType.xml", "interpolator" );
    auto & keys = actual->GetKeys();

    ASSERT_EQ( keys.size(), 11 );

    EXPECT_EQ( keys[ 0 ].val, glm::vec2( 0.0, 0.0 ) );    // Default
    EXPECT_EQ( keys[ 1 ].val, glm::vec2( 0.0, 0.0 ) );    // Default
    EXPECT_EQ( keys[ 2 ].val, glm::vec2( 0.0, 0.0 ) );    // Default
    EXPECT_EQ( keys[ 3 ].val, glm::vec2( 0.0, 0.0 ) );    // Default
    EXPECT_EQ( keys[ 4 ].val, glm::vec2( 0.0, 0.0 ) );    // Default
    EXPECT_EQ( keys[ 5 ].val, glm::vec2( 0.0, 0.0 ) );    // Default
    EXPECT_EQ( keys[ 6 ].val, glm::vec2( 2.0, 3.0 ) );
    EXPECT_EQ( keys[ 7 ].val, glm::vec2( 0.0, 0.0 ) );    // Default
    EXPECT_EQ( keys[ 8 ].val, glm::vec2( 0.0, 0.0 ) );    // Default
    EXPECT_EQ( keys[ 9 ].val, glm::vec2( 2.0, 3.0 ) );
    EXPECT_EQ( keys[ 10 ].val, glm::vec2( 0.0, 0.0 ) );   // Default
}

// ***********************
// Keys that cannot be parsed to expected type are added with default values as placeholders.
// But this happens only if time value is valid.
TEST( Serialization_ParamValModel, Keys_Vec3_DifferentKeyTypeInXML )
{
    auto actual = Deserialize< CompositeInterpolator< TimeType, glm::vec3 > >( "TestAssets/Serialization/Keys/DifferentKeyType.xml", "interpolator" );
    auto & keys = actual->GetKeys();

    ASSERT_EQ( keys.size(), 11 );

    EXPECT_EQ( keys[ 0 ].val, glm::vec3( 0.0, 0.0, 0.0 ) );    // Default
    EXPECT_EQ( keys[ 1 ].val, glm::vec3( 0.0, 0.0, 0.0 ) );    // Default
    EXPECT_EQ( keys[ 2 ].val, glm::vec3( 0.0, 0.0, 0.0 ) );    // Default
    EXPECT_EQ( keys[ 3 ].val, glm::vec3( 0.0, 0.0, 0.0) );     // Default
    EXPECT_EQ( keys[ 4 ].val, glm::vec3( 0.0, 0.0, 0.0 ) );    // Default
    EXPECT_EQ( keys[ 5 ].val, glm::vec3( 0.0, 0.0, 0.0 ) );    // Default
    EXPECT_EQ( keys[ 6 ].val, glm::vec3( 0.0, 0.0, 0.0 ) );    // Default
    EXPECT_EQ( keys[ 7 ].val, glm::vec3( 2.0, 3.0, 13.0 ) );
    EXPECT_EQ( keys[ 8 ].val, glm::vec3( 0.0, 0.0, 0.0 ) );    // Default
    EXPECT_EQ( keys[ 9 ].val, glm::vec3( 0.0, 0.0, 0.0 ) );    // Default
    EXPECT_EQ( keys[ 10 ].val, glm::vec3( 0.0, 0.0, 0.0 ) );   // Default
}

// ***********************
// Keys that cannot be parsed to expected type are added with default values as placeholders.
// But this happens only if time value is valid.
TEST( Serialization_ParamValModel, Keys_Vec4_DifferentKeyTypeInXML )
{
    auto actual = Deserialize< CompositeInterpolator< TimeType, glm::vec4 > >( "TestAssets/Serialization/Keys/DifferentKeyType.xml", "interpolator" );
    auto & keys = actual->GetKeys();

    ASSERT_EQ( keys.size(), 11 );

    EXPECT_EQ( keys[ 0 ].val, glm::vec4( 0.0, 0.0, 0.0, 0.0 ) );    // Default
    EXPECT_EQ( keys[ 1 ].val, glm::vec4( 0.0, 0.0, 0.0, 0.0 ) );    // Default
    EXPECT_EQ( keys[ 2 ].val, glm::vec4( 0.0, 0.0, 0.0, 0.0 ) );    // Default
    EXPECT_EQ( keys[ 3 ].val, glm::vec4( 0.0, 0.0, 0.0, 0.0 ) );    // Default
    EXPECT_EQ( keys[ 4 ].val, glm::vec4( 0.0, 0.0, 0.0, 0.0 ) );    // Default
    EXPECT_EQ( keys[ 5 ].val, glm::vec4( 0.0, 0.0, 0.0, 0.0 ) );    // Default
    EXPECT_EQ( keys[ 6 ].val, glm::vec4( 0.0, 0.0, 0.0, 0.0 ) );    // Default
    EXPECT_EQ( keys[ 7 ].val, glm::vec4( 0.0, 0.0, 0.0, 0.0 ) );    // Default
    EXPECT_EQ( keys[ 8 ].val, glm::vec4( 2.0, 3.0, 13.0, -1.0 ) );  // Default
    EXPECT_EQ( keys[ 9 ].val, glm::vec4( 0.0, 0.0, 0.0, 0.0 ) );    // Default
    EXPECT_EQ( keys[ 10 ].val, glm::vec4( 0.0, 0.0, 0.0, 0.0 ) );   // Default
}

// ***********************
// Keys that cannot be parsed to expected type are added with default values as placeholders.
// But this happens only if time value is valid.
TEST( Serialization_ParamValModel, Keys_String_DifferentKeyTypeInXML )
{
    auto actual = Deserialize< CompositeInterpolator< TimeType, std::string > >( "TestAssets/Serialization/Keys/DifferentKeyType.xml", "interpolator" );
    auto & keys = actual->GetKeys();

    ASSERT_EQ( keys.size(), 11 );

    EXPECT_EQ( keys[ 0 ].val, "3" );
    EXPECT_EQ( keys[ 1 ].val, "3.10101010" );
    EXPECT_EQ( keys[ 2 ].val,  "-13" );
    EXPECT_EQ( keys[ 3 ].val, "string in xml" );
    EXPECT_EQ( keys[ 4 ].val, "true" );
    EXPECT_EQ( keys[ 5 ].val, "false" );
    EXPECT_EQ( keys[ 6 ].val, "2.0, 3.0" );
    EXPECT_EQ( keys[ 7 ].val, "2.0, 3.0, 13.0" );
    EXPECT_EQ( keys[ 8 ].val, "2.0, 3.0, 13.0, -1.0" );
    EXPECT_EQ( keys[ 9 ].val, "2,3" );
    EXPECT_EQ( keys[ 10 ].val, "wstring z ¿ó³t¹ gêœl¹" );
}

// ***********************
// Keys that cannot be parsed to expected type are added with default values as placeholders.
// But this happens only if time value is valid.
TEST( Serialization_ParamValModel, Keys_WString_DifferentKeyTypeInXML )
{
    auto actual = Deserialize< CompositeInterpolator< TimeType, std::wstring > >( "TestAssets/Serialization/Keys/DifferentKeyType.xml", "interpolator" );
    auto & keys = actual->GetKeys();

    ASSERT_EQ( keys.size(), 11 );

    EXPECT_EQ( keys[ 0 ].val, L"3" );
    EXPECT_EQ( keys[ 1 ].val, L"3.10101010" );
    EXPECT_EQ( keys[ 2 ].val, L"-13" );
    EXPECT_EQ( keys[ 3 ].val, L"string in xml" );
    EXPECT_EQ( keys[ 4 ].val, L"true" );
    EXPECT_EQ( keys[ 5 ].val, L"false" );
    EXPECT_EQ( keys[ 6 ].val, L"2.0, 3.0" );
    EXPECT_EQ( keys[ 7 ].val, L"2.0, 3.0, 13.0" );
    EXPECT_EQ( keys[ 8 ].val, L"2.0, 3.0, 13.0, -1.0" );
    EXPECT_EQ( keys[ 9 ].val, L"2,3" );
    EXPECT_EQ( keys[ 10 ].val, L"wstring z ¿ó³t¹ gêœl¹" );
}

