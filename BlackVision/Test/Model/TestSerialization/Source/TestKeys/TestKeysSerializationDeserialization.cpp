#include "gtest/gtest.h"

#include "Mathematics/Interpolators/CompositeInterpolator.h"
#include "Mathematics/Transform/MatTransform.h"

#include "Utils/Comparators/ParamComparator.h"
#include "Utils/Serialization/Serialize.h"

#include "Mathematics/glm_inc.h"


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

    Serialize( expectedInterpolator, "SerDeserBool.xml" );

    auto actualInterpolator = Deserialize< CompositeInterpolator< TimeType, bool > >( "SerDeserBool.xml", "interpolator" );

    EXPECT_TRUE( ParamComparator::CompareKeys( expectedInterpolator, *( actualInterpolator.get() ) ) );
    EXPECT_EQ( actualInterpolator->GetKeys().size(), 5 );
}

// ***********************
//
TEST( Serialization_ParamValModel, Keys_SerializeDeserializeString )
{
    CompositeInterpolator< TimeType, std::string > expectedInterpolator;

    expectedInterpolator.AddKey( 0.0f, "Blaaa" );
    expectedInterpolator.AddKey( 1.0f, "Dobre coo" );
    expectedInterpolator.AddKey( 3.0f, "Œmieszne wartoœæi" );
    expectedInterpolator.AddKey( 4.0f, "z¿ó³k³a kiœæ" );
    expectedInterpolator.AddKey( 7.0f, "ŸdŸb³o" );

    Serialize( expectedInterpolator, "SerDeserString.xml" );

    auto actualInterpolator = Deserialize< CompositeInterpolator< TimeType, std::string > >( "SerDeserString.xml", "interpolator" );

    EXPECT_TRUE( ParamComparator::CompareKeys( expectedInterpolator, *( actualInterpolator.get() ) ) );
    EXPECT_EQ( actualInterpolator->GetKeys().size(), 5 );
}


// ***********************
//
TEST( Serialization_ParamValModel, Keys_SerializeDeserializeWString )
{
    CompositeInterpolator< TimeType, std::wstring > expectedInterpolator;

    expectedInterpolator.AddKey( 0.0f, L"Blaaa" );
    expectedInterpolator.AddKey( 1.0f, L"Dobre coo" );
    expectedInterpolator.AddKey( 3.0f, L"Œmieszne wartoœæi" );
    expectedInterpolator.AddKey( 4.0f, L"z¿ó³k³a kiœæ" );
    expectedInterpolator.AddKey( 7.0f, L"ŸdŸb³o" );

    Serialize( expectedInterpolator, "SerDeserWString.xml" );

    auto actualInterpolator = Deserialize< CompositeInterpolator< TimeType, std::wstring > >( "SerDeserWString.xml", "interpolator" );

    EXPECT_TRUE( ParamComparator::CompareKeys( expectedInterpolator, *( actualInterpolator.get() ) ) );
    EXPECT_EQ( actualInterpolator->GetKeys().size(), 5 );
}

// ***********************
//
TEST( Serialization_ParamValModel, Keys_SerializeDeserializeVec2 )
{
    CompositeInterpolator< TimeType, glm::vec2 > expectedInterpolator;

    expectedInterpolator.AddKey( 0.0f, glm::vec2( 2.0, 3.0 ) );
    expectedInterpolator.AddKey( 1.0f, glm::vec2( 22.0, 37.0 ) );
    expectedInterpolator.AddKey( 3.0f, glm::vec2( 0.0, 0.00001 ) );
    expectedInterpolator.AddKey( 4.0f, glm::vec2( 0.0, -34.0 ) );
    expectedInterpolator.AddKey( 7.0f, glm::vec2( 22.34315325, 3.1231 ) );

    Serialize( expectedInterpolator, "SerDeserVec2.xml" );

    auto actualInterpolator = Deserialize< CompositeInterpolator< TimeType, glm::vec2 > >( "SerDeserVec2.xml", "interpolator" );

    EXPECT_TRUE( ParamComparator::CompareKeys( expectedInterpolator, *( actualInterpolator.get() ) ) );
    EXPECT_EQ( actualInterpolator->GetKeys().size(), 5 );
}

// ***********************
//
TEST( Serialization_ParamValModel, Keys_SerializeDeserializeVec3 )
{
    CompositeInterpolator< TimeType, glm::vec3 > expectedInterpolator;

    expectedInterpolator.AddKey( 0.0f, glm::vec3( 2.0, 3.0, 1.0 ) );
    expectedInterpolator.AddKey( 1.0f, glm::vec3( 22.0, 37.0, -15 ) );
    expectedInterpolator.AddKey( 3.0f, glm::vec3( 0.0, 0.00001, 0.1 ) );
    expectedInterpolator.AddKey( 4.0f, glm::vec3( 0.0, -34.0, 13.00002 ) );
    expectedInterpolator.AddKey( 7.0f, glm::vec3( 22.34315325, 3.1231, 15.000006 ) );

    Serialize( expectedInterpolator, "SerDeserVec3.xml" );

    auto actualInterpolator = Deserialize< CompositeInterpolator< TimeType, glm::vec3 > >( "SerDeserVec3.xml", "interpolator" );

    EXPECT_TRUE( ParamComparator::CompareKeys( expectedInterpolator, *( actualInterpolator.get() ) ) );
    EXPECT_EQ( actualInterpolator->GetKeys().size(), 5 );
}

// ***********************
//
TEST( Serialization_ParamValModel, Keys_SerializeDeserializeVec4 )
{
    CompositeInterpolator< TimeType, glm::vec4 > expectedInterpolator;

    expectedInterpolator.AddKey( 0.0f, glm::vec4( 2.0, 3.0, 1.0, 0.0 ) );
    expectedInterpolator.AddKey( 1.0f, glm::vec4( 22.0, 37.0, -15, -11 ) );
    expectedInterpolator.AddKey( 3.0f, glm::vec4( 0.0, 0.00001, 0.1, -0.000012 ) );
    expectedInterpolator.AddKey( 4.0f, glm::vec4( 0.0, -34.0, 13.00002, 1.01111 ) );
    expectedInterpolator.AddKey( 7.0f, glm::vec4( 22.34315325, 3.1231, 15.000006, 0.9999999999 ) );

    Serialize( expectedInterpolator, "SerDeserVec4.xml" );

    auto actualInterpolator = Deserialize< CompositeInterpolator< TimeType, glm::vec4 > >( "SerDeserVec4.xml", "interpolator" );

    EXPECT_TRUE( ParamComparator::CompareKeys( expectedInterpolator, *( actualInterpolator.get() ) ) );
    EXPECT_EQ( actualInterpolator->GetKeys().size(), 5 );
}

// ***********************
//
TEST( Serialization_ParamValModel, Keys_SerializeDeserializeTransform )
{
    CompositeTransform expected;
    expected.SetTranslation( glm::vec3( 2.0, 3.0, 1.0 ), 0.0f );
    expected.SetTranslation( glm::vec3( 22.0, 37.0, -15 ), 2.0f );
    expected.SetTranslation( glm::vec3( 0.0, 0.00001, 0.1 ), 3.0f );
    expected.SetTranslation( glm::vec3( 0.0, -34.0, 13.00002 ), 4.0f );
    expected.SetTranslation( glm::vec3( 22.34315325, 3.1231, 15.000006 ), 7.0f );

    expected.SetRotation( glm::vec3( 2.0, 3.0, 1.0 ), 0.0f );
    expected.SetRotation( glm::vec3( 22.0, 37.0, -15 ), 1.0f );
    expected.SetRotation( glm::vec3( 0.0, 0.00001, 0.1 ), 5.0f );
    expected.SetRotation( glm::vec3( 0.0, -34.0, 13.00002 ), 6.0f );
    expected.SetRotation( glm::vec3( 22.34315325, 3.1231, 15.000006 ), 7.0f );

    expected.SetScale( glm::vec3( 2.0, 3.0, 1.0 ), 0.0f );
    expected.SetScale( glm::vec3( 22.0, 37.0, -15 ), 1.5f );
    expected.SetScale( glm::vec3( 0.0, 0.00001, 0.1 ), 2.0f );
    expected.SetScale( glm::vec3( 0.0, -34.0, 13.00002 ), 5.0f );
    expected.SetScale( glm::vec3( 22.34315325, 3.1231, 15.000006 ), 8.0f );

    expected.SetCenter( glm::vec3( 2.0, 3.0, 1.0 ), 0.0f );
    expected.SetCenter( glm::vec3( 22.0, 37.0, -15 ), 1.0f );
    expected.SetCenter( glm::vec3( 0.0, 0.00001, 0.1 ), 3.0f );
    expected.SetCenter( glm::vec3( 0.0, -34.0, 13.00002 ), 4.0f );
    expected.SetCenter( glm::vec3( 22.34315325, 3.1231, 15.000006 ), 7.0f );

    Serialize( expected, "SerDeserTransform.xml" );

    auto actual = Deserialize< CompositeTransform >( "SerDeserTransform.xml", "composite_transform" );
    auto & actualRef = *( actual.get() );

    EXPECT_TRUE( ParamComparator::CompareTransformKeys( expected, *( actual.get() ) ) );


    EXPECT_EQ( TEST_ACCESSOR( CompositeTransform )::TranslationX( actualRef ).GetNumKeys(), 5 );
    EXPECT_EQ( TEST_ACCESSOR( CompositeTransform )::TranslationY( actualRef ).GetNumKeys(), 5 );
    EXPECT_EQ( TEST_ACCESSOR( CompositeTransform )::TranslationZ( actualRef ).GetNumKeys(), 5 );

    EXPECT_EQ( TEST_ACCESSOR( CompositeTransform )::Pitch( actualRef ).GetNumKeys(), 5 );
    EXPECT_EQ( TEST_ACCESSOR( CompositeTransform )::Yaw( actualRef ).GetNumKeys(), 5 );
    EXPECT_EQ( TEST_ACCESSOR( CompositeTransform )::Roll( actualRef ).GetNumKeys(), 5 );

    EXPECT_EQ( TEST_ACCESSOR( CompositeTransform )::ScaleX( actualRef ).GetNumKeys(), 5 );
    EXPECT_EQ( TEST_ACCESSOR( CompositeTransform )::ScaleY( actualRef ).GetNumKeys(), 5 );
    EXPECT_EQ( TEST_ACCESSOR( CompositeTransform )::ScaleZ( actualRef ).GetNumKeys(), 5 );

    EXPECT_EQ( TEST_ACCESSOR( CompositeTransform )::CenterX( actualRef ).GetNumKeys(), 5 );
    EXPECT_EQ( TEST_ACCESSOR( CompositeTransform )::CenterY( actualRef ).GetNumKeys(), 5 );
    EXPECT_EQ( TEST_ACCESSOR( CompositeTransform )::CenterZ( actualRef ).GetNumKeys(), 5 );
}

