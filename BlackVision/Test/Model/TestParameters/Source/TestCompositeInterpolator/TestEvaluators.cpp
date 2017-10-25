#include "gtest/gtest.h"


#include "Mathematics/Interpolators/CompositeInterpolator.h"
#include "Utils/Accessors/CompositeInterpolatorAccessor.h"



using namespace bv;



// ***********************
// Simple AddKey function test to check if evaluators are correct.
TEST( Model_ParamValModel, CompositeInterpolator_AddKeysWithDifferentCurves )
{
    CompositeInterpolator< TimeType, float > interpolator;
    interpolator.AddKey( 1.0f, 2.0f );

    auto & evals = TEST_ACCESSOR( CompositeInterpolator )::GetEvaluators( &interpolator );

    // Add key with specyfied curve.
    interpolator.SetAddedKeyCurveType( CurveType::CT_BEZIER );
    interpolator.AddKey( 1.5f, 2.0f );

    EXPECT_EQ( evals[ 0 ]->GetCurveType(), CurveType::CT_BEZIER );

    interpolator.SetAddedKeyCurveType( CurveType::CT_COSINE_LIKE );
    interpolator.AddKey( 2.0f, 2.0f );

    EXPECT_EQ( evals[ 1 ]->GetCurveType(), CurveType::CT_COSINE_LIKE );

    interpolator.SetAddedKeyCurveType( CurveType::CT_CUBIC_IN );
    interpolator.AddKey( 3.0f, 2.0f );

    EXPECT_EQ( evals[ 2 ]->GetCurveType(), CurveType::CT_CUBIC_IN );

    interpolator.SetAddedKeyCurveType( CurveType::CT_CUBIC_INTOUT );
    interpolator.AddKey( 4.0f, 2.0f );

    EXPECT_EQ( evals[ 3 ]->GetCurveType(), CurveType::CT_CUBIC_INTOUT );

    interpolator.SetAddedKeyCurveType( CurveType::CT_CUBIC_OUT );
    interpolator.AddKey( 5.0f, 2.0f );

    EXPECT_EQ( evals[ 4 ]->GetCurveType(), CurveType::CT_CUBIC_OUT );

    interpolator.SetAddedKeyCurveType( CurveType::CT_ELASTIC_IN );
    interpolator.AddKey( 6.0f, 2.0f );

    EXPECT_EQ( evals[ 5 ]->GetCurveType(), CurveType::CT_ELASTIC_IN );

    interpolator.SetAddedKeyCurveType( CurveType::CT_ELASTIC_IN_BOUNCE );
    interpolator.AddKey( 7.0f, 2.0f );

    EXPECT_EQ( evals[ 6 ]->GetCurveType(), CurveType::CT_ELASTIC_IN_BOUNCE );

    interpolator.SetAddedKeyCurveType( CurveType::CT_ELASTIC_OUT );
    interpolator.AddKey( 8.0f, 2.0f );

    EXPECT_EQ( evals[ 7 ]->GetCurveType(), CurveType::CT_ELASTIC_OUT );

    interpolator.SetAddedKeyCurveType( CurveType::CT_ELASTIC_OUT_BOUNCE );
    interpolator.AddKey( 9.0f, 2.0f );

    EXPECT_EQ( evals[ 8 ]->GetCurveType(), CurveType::CT_ELASTIC_OUT_BOUNCE );

    interpolator.SetAddedKeyCurveType( CurveType::CT_LINEAR );
    interpolator.AddKey( 10.0f, 2.0f );

    EXPECT_EQ( evals[ 9 ]->GetCurveType(), CurveType::CT_LINEAR );

    interpolator.SetAddedKeyCurveType( CurveType::CT_POINT );
    interpolator.AddKey( 11.0f, 2.0f );

    EXPECT_EQ( evals[ 10 ]->GetCurveType(), CurveType::CT_POINT );

    interpolator.SetAddedKeyCurveType( CurveType::CT_QUARTIC_INOUT );
    interpolator.AddKey( 12.0f, 2.0f );

    EXPECT_EQ( evals[ 11 ]->GetCurveType(), CurveType::CT_QUARTIC_INOUT );
}

// ***********************
// SetGlobalCurveType should change all curves in all interpolators.
TEST( Model_ParamValModel, CompositeInterpolator_ChangeAllCurves )
{
    CompositeInterpolator< TimeType, float > interpolator;
    interpolator.AddKey( 1.0f, 2.0f );

    auto & evals = TEST_ACCESSOR( CompositeInterpolator )::GetEvaluators( &interpolator );

    // Add keys with different curves
    interpolator.SetAddedKeyCurveType( CurveType::CT_BEZIER );
    interpolator.AddKey( 1.5f, 2.0f );

    interpolator.SetAddedKeyCurveType( CurveType::CT_POINT );
    interpolator.AddKey( 2.5f, 2.0f );

    interpolator.SetAddedKeyCurveType( CurveType::CT_ELASTIC_OUT );
    interpolator.AddKey( 3.5f, 2.0f );

    interpolator.SetGlobalCurveType( CurveType::CT_LINEAR );

    EXPECT_EQ( evals[ 0 ]->GetCurveType(), CurveType::CT_LINEAR );
    EXPECT_EQ( evals[ 1 ]->GetCurveType(), CurveType::CT_LINEAR );
    EXPECT_EQ( evals[ 2 ]->GetCurveType(), CurveType::CT_LINEAR );
}