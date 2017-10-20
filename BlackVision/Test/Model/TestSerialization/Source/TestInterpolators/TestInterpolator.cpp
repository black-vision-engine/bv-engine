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
//
TEST( Serialization_ParamValModel, Interpolators_CompositeInterpolatorSerDeserFloat )
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
