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
    expected.AddKey( 3.0f, "�mieszne warto��i" );
    expected.AddKey( 4.0f, "z��k�a ki��" );
    expected.AddKey( 7.0f, "�d�b�o" );

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
    expected.AddKey( 3.0f, L"�mieszne warto��i" );
    expected.AddKey( 4.0f, L"z��k�a ki��" );
    expected.AddKey( 7.0f, L"�d�b�o" );

    Serialize( expected, "InterpolatorSerDeserWString.xml" );

    auto actual = Deserialize< CompositeInterpolator< TimeType, std::wstring > >( "InterpolatorSerDeserWString.xml", "interpolator" );
    EXPECT_TRUE( ParamComparator::CompareEvaluators( expected, *( actual.get() ) ) );

    EXPECT_EQ( TEST_ACCESSOR( CompositeInterpolator )::GetEvaluators( actual.get() ).size(), 4 );
}




