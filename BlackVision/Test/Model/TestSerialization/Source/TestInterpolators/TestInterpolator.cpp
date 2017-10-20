#include "gtest/gtest.h"



#include "Mathematics/Transform/MatTransform.h"

#include "Utils/Comparators/ParamComparator.h"
#include "Utils/Serialization/Serialize.h"

#include "Framework/UseLoggerTests.h"

#include "Mathematics/Interpolators/CompositeInterpolator.h"
#include "Mathematics/Interpolators/CompositeInterpolator.inl"

#include "Utils/Accessors/CompositeInterpolatorAccessor.h"


using namespace bv;



// ***********************
// Test serialization of CurveType and pre, post WrapMEthod.
TEST( Serialization_ParamValModel, Interpolators_CompositeInterpolatorParams )
{
    auto actual = Deserialize< CompositeInterpolator< TimeType, float > >( "TestAssets/Serialization/Interpolators/CurveTypeAndWrapMethods.xml", "interpolator" );
    ASSERT_NE( actual, nullptr );

    EXPECT_EQ( actual->GetWrapPreMethod(), WrapMethod::pingPong );
    EXPECT_EQ( actual->GetWrapPostMethod(), WrapMethod::repeat );
    EXPECT_EQ( actual->GetCurveType(), CurveType::CT_ELASTIC_OUT_BOUNCE );
}


// ***********************
// Test compares CompositeInterpolator evaluators serialization and deserialization.
// This is hight level tests that checks only if result of the serialization is the same as original.
TEST( Serialization_ParamValModel, Interpolators_CompositeInterpolator_SerDeserFloat )
{
    CompositeInterpolator< TimeType, float > expected;

    expected.AddKey( 0.0f, 3.0f );

    expected.SetAddedKeyCurveType( CurveType::CT_BEZIER );
    expected.AddKey( 1.0f, 3.0f );

    expected.SetAddedKeyCurveType( CurveType::CT_COSINE_LIKE );
    expected.AddKey( 3.0f, 13.0f );

    expected.SetAddedKeyCurveType( CurveType::CT_CUBIC_IN );
    expected.AddKey( 4.0f, 2.0f );

    expected.SetAddedKeyCurveType( CurveType::CT_CUBIC_OUT );
    expected.AddKey( 7.0f, 5.0f );

    Serialize( expected, "InterpolatorSerDeserFloat.xml" );

    auto actual = Deserialize< CompositeInterpolator< TimeType, float > >( "InterpolatorSerDeserFloat.xml", "interpolator" );
    EXPECT_TRUE( ParamComparator::CompareEvaluators( expected, *( actual.get() ) ) );

    EXPECT_EQ( TEST_ACCESSOR( CompositeInterpolator )::GetEvaluators( actual.get() ).size(), 4 );
}

// ***********************
// Test compares CompositeInterpolator evaluators serialization and deserialization.
// This is hight level tests that checks only if result of the serialization is the same as original.
TEST( Serialization_ParamValModel, Interpolators_CompositeInterpolator_SerDeserInt )
{
    CompositeInterpolator< TimeType, int > expected;

    expected.AddKey( 0.0f, 1 );

    expected.SetAddedKeyCurveType( CurveType::CT_LINEAR );
    expected.AddKey( 1.0f, 3 );

    expected.SetAddedKeyCurveType( CurveType::CT_CUBIC_INTOUT );
    expected.AddKey( 3.0f, 7 );

    expected.SetAddedKeyCurveType( CurveType::CT_ELASTIC_IN );
    expected.AddKey( 4.0f, 2 );

    expected.SetAddedKeyCurveType( CurveType::CT_ELASTIC_IN_BOUNCE );
    expected.AddKey( 7.0f, 10 );

    Serialize( expected, "InterpolatorSerDeserInt.xml" );

    auto actual = Deserialize< CompositeInterpolator< TimeType, int > >( "InterpolatorSerDeserInt.xml", "interpolator" );
    EXPECT_TRUE( ParamComparator::CompareEvaluators( expected, *( actual.get() ) ) );

    EXPECT_EQ( TEST_ACCESSOR( CompositeInterpolator )::GetEvaluators( actual.get() ).size(), 4 );
}

// ***********************
// Test compares CompositeInterpolator evaluators serialization and deserialization.
// This is hight level tests that checks only if result of the serialization is the same as original.
TEST( Serialization_ParamValModel, Interpolators_CompositeInterpolator_SerDeserBool )
{
    CompositeInterpolator< TimeType, bool > expected;

    expected.SetAddedKeyCurveType( CurveType::CT_POINT );
    expected.AddKey( 0.0f, true );
    expected.AddKey( 1.0f, false );
    expected.AddKey( 3.0f, false );
    expected.AddKey( 4.0f, true );
    expected.AddKey( 7.0f, false );

    Serialize( expected, "InterpolatorSerDeserBool.xml" );

    auto actual = Deserialize< CompositeInterpolator< TimeType, bool > >( "InterpolatorSerDeserBool.xml", "interpolator" );
    EXPECT_TRUE( ParamComparator::CompareEvaluators( expected, *( actual.get() ) ) );

    EXPECT_EQ( TEST_ACCESSOR( CompositeInterpolator )::GetEvaluators( actual.get() ).size(), 4 );
}


// ***********************
// Test compares CompositeInterpolator evaluators serialization and deserialization.
// This is hight level tests that checks only if result of the serialization is the same as original.
TEST( Serialization_ParamValModel, Interpolators_CompositeInterpolator_SerDeserString )
{
    CompositeInterpolator< TimeType, std::string > expected;

    expected.SetAddedKeyCurveType( CurveType::CT_POINT );
    expected.AddKey( 0.0f, "Blaaa" );
    expected.AddKey( 1.0f, "Dobre coo" );
    expected.AddKey( 3.0f, "Œmieszne wartoœæi" );
    expected.AddKey( 4.0f, "z¿ó³k³a kiœæ" );
    expected.AddKey( 7.0f, "ŸdŸb³o" );

    Serialize( expected, "InterpolatorSerDeserString.xml" );

    auto actual = Deserialize< CompositeInterpolator< TimeType, std::string > >( "InterpolatorSerDeserString.xml", "interpolator" );
    EXPECT_TRUE( ParamComparator::CompareEvaluators( expected, *( actual.get() ) ) );

    EXPECT_EQ( TEST_ACCESSOR( CompositeInterpolator )::GetEvaluators( actual.get() ).size(), 4 );
}

// ***********************
// Test compares CompositeInterpolator evaluators serialization and deserialization.
// This is hight level tests that checks only if result of the serialization is the same as original.
TEST( Serialization_ParamValModel, Interpolators_CompositeInterpolator_SerDeserWString )
{
    CompositeInterpolator< TimeType, std::wstring > expected;

    expected.SetAddedKeyCurveType( CurveType::CT_POINT );
    expected.AddKey( 0.0f, L"Blaaa" );
    expected.AddKey( 1.0f, L"Dobre coo" );
    expected.AddKey( 3.0f, L"Œmieszne wartoœæi" );
    expected.AddKey( 4.0f, L"z¿ó³k³a kiœæ" );
    expected.AddKey( 7.0f, L"ŸdŸb³o" );

    Serialize( expected, "InterpolatorSerDeserWString.xml" );

    auto actual = Deserialize< CompositeInterpolator< TimeType, std::wstring > >( "InterpolatorSerDeserWString.xml", "interpolator" );
    EXPECT_TRUE( ParamComparator::CompareEvaluators( expected, *( actual.get() ) ) );

    EXPECT_EQ( TEST_ACCESSOR( CompositeInterpolator )::GetEvaluators( actual.get() ).size(), 4 );
}


// ***********************
// Test compares CompositeInterpolator evaluators serialization and deserialization.
// This is hight level tests that checks only if result of the serialization is the same as original.
TEST( Serialization_ParamValModel, Interpolators_CompositeInterpolator_SerDeserVec2 )
{
    CompositeInterpolator< TimeType, glm::vec2 > expected;

    expected.AddKey( 0.0f, glm::vec2( 2.0, 3.0 ) );

    expected.SetAddedKeyCurveType( CurveType::CT_ELASTIC_IN_BOUNCE );
    expected.AddKey( 1.0f, glm::vec2( 22.0, 37.0 ) );

    expected.SetAddedKeyCurveType( CurveType::CT_ELASTIC_OUT );
    expected.AddKey( 3.0f, glm::vec2( 0.0, 0.00001 ) );

    expected.SetAddedKeyCurveType( CurveType::CT_ELASTIC_OUT_BOUNCE );
    expected.AddKey( 4.0f, glm::vec2( 0.0, -34.0 ) );

    expected.SetAddedKeyCurveType( CurveType::CT_LINEAR );
    expected.AddKey( 7.0f, glm::vec2( 22.34315325, 3.1231 ) );

    Serialize( expected, "InterpolatorSerDeserVec2.xml" );

    auto actual = Deserialize< CompositeInterpolator< TimeType, glm::vec2 > >( "InterpolatorSerDeserVec2.xml", "interpolator" );
    EXPECT_TRUE( ParamComparator::CompareEvaluators( expected, *( actual.get() ) ) );

    EXPECT_EQ( TEST_ACCESSOR( CompositeInterpolator )::GetEvaluators( actual.get() ).size(), 4 );
}

// ***********************
// Test compares CompositeInterpolator evaluators serialization and deserialization.
// This is hight level tests that checks only if result of the serialization is the same as original.
TEST( Serialization_ParamValModel, Interpolators_CompositeInterpolator_SerDeserVec3 )
{
    CompositeInterpolator< TimeType, glm::vec3 > expected;

    expected.AddKey( 0.0f, glm::vec3( 2.0, 3.0, 1.0 ) );

    expected.SetAddedKeyCurveType( CurveType::CT_QUARTIC_INOUT );
    expected.AddKey( 1.0f, glm::vec3( 22.0, 37.0, -15 ) );

    expected.SetAddedKeyCurveType( CurveType::CT_ELASTIC_OUT_BOUNCE );
    expected.AddKey( 3.0f, glm::vec3( 0.0, 0.00001, 0.1 ) );

    expected.SetAddedKeyCurveType( CurveType::CT_CUBIC_IN );
    expected.AddKey( 4.0f, glm::vec3( 0.0, -34.0, 13.00002 ) );

    expected.SetAddedKeyCurveType( CurveType::CT_ELASTIC_IN );
    expected.AddKey( 7.0f, glm::vec3( 22.34315325, 3.1231, 15.000006 ) );

    Serialize( expected, "InterpolatorSerDeserVec3.xml" );

    auto actual = Deserialize< CompositeInterpolator< TimeType, glm::vec3 > >( "InterpolatorSerDeserVec3.xml", "interpolator" );
    EXPECT_TRUE( ParamComparator::CompareEvaluators( expected, *( actual.get() ) ) );

    EXPECT_EQ( TEST_ACCESSOR( CompositeInterpolator )::GetEvaluators( actual.get() ).size(), 4 );
}

// ***********************
// Test compares CompositeInterpolator evaluators serialization and deserialization.
// This is hight level tests that checks only if result of the serialization is the same as original.
TEST( Serialization_ParamValModel, Interpolators_CompositeInterpolator_SerDeserVec4 )
{
    CompositeInterpolator< TimeType, glm::vec4 > expected;

    expected.AddKey( 0.0f, glm::vec4( 2.0, 3.0, 1.0, 0.0 ) );

    expected.SetAddedKeyCurveType( CurveType::CT_COSINE_LIKE );
    expected.AddKey( 1.0f, glm::vec4( 22.0, 37.0, -15, -11 ) );

    expected.SetAddedKeyCurveType( CurveType::CT_ELASTIC_OUT_BOUNCE );
    expected.AddKey( 3.0f, glm::vec4( 0.0, 0.00001, 0.1, -0.000012 ) );

    expected.SetAddedKeyCurveType( CurveType::CT_QUARTIC_INOUT );
    expected.AddKey( 4.0f, glm::vec4( 0.0, -34.0, 13.00002, 1.01111 ) );

    expected.SetAddedKeyCurveType( CurveType::CT_BEZIER );
    expected.AddKey( 7.0f, glm::vec4( 22.34315325, 3.1231, 15.000006, 0.9999999999 ) );

    Serialize( expected, "InterpolatorSerDeserVec4.xml" );

    auto actual = Deserialize< CompositeInterpolator< TimeType, glm::vec4 > >( "InterpolatorSerDeserVec4.xml", "interpolator" );
    EXPECT_TRUE( ParamComparator::CompareEvaluators( expected, *( actual.get() ) ) );

    EXPECT_EQ( TEST_ACCESSOR( CompositeInterpolator )::GetEvaluators( actual.get() ).size(), 4 );
}

