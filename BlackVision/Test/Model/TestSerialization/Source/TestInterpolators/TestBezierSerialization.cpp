#include "gtest/gtest.h"



#include "Mathematics/Transform/MatTransform.h"

#include "Utils/Comparators/ParamComparator.h"
#include "Utils/Serialization/SerializeInteprolator.h"

#include "Framework/UseLoggerTests.h"

#include "Utils/Accessors/EvaluatorsAccessors.h"
#include "Mathematics/Interpolators/Functions/BezierFunction.h"

#include "Mathematics/Interpolators/CompositeInterpolator.h"
#include "Mathematics/Interpolators/CompositeInterpolator.inl"


using namespace bv;


TimeType tolerance = 0.00001f;



// ***********************
// Create evaluator with not default values, serialize and deserialize.
// Evaluators should be equal.
TEST( Serialization_ParamValModel, Evaluators_Bezier_SerializeDeserialize )
{
    Key< TimeType, float > k1( 0.0, 1.0f );
    Key< TimeType, float > k2( 3.0, 5.0f );

    auto expected = std::make_shared< BezierEvaluator< TimeType, float > >( CurveType::CT_BEZIER, k1, k2, Key< TimeType, float >( 0.15f, 1.0f ), Key< TimeType, float >( 0.60f, 5.0f ), tolerance );

    Serialize( *( expected.get() ), "BezierSerDeser.xml" );

    auto actual = DeserializeBezier( "BezierSerDeser.xml", CurveType::CT_BEZIER, k1, k2 );
    EXPECT_TRUE( ParamComparator::CompareEvaluators( *( expected.get() ), *( actual.get() ) ) );
}

// ***********************
// If any value lacks in xml, bezier should be created with default values.
TEST( Serialization_ParamValModel, Evaluators_Bezier_NoV1Param )
{
    Key< TimeType, float > k1( 0.0, 1.0f );
    Key< TimeType, float > k2( 3.0, 5.0f );

    auto actual = DeserializeBezier( "TestAssets/Serialization/Interpolators/BezierLackOfDelta1.xml", CurveType::CT_BEZIER, k1, k2 );
    auto expected = CreateDummyInterpolator< TimeType, float >( CurveType::CT_BEZIER, k1, k2, tolerance );

    EXPECT_TRUE( ParamComparator::CompareEvaluators( *( expected.get() ), *( actual.get() ) ) );
}

// ***********************
// If any value lacks in xml, bezier should be created with default values.
TEST( Serialization_ParamValModel, Evaluators_Bezier_NoV2Param )
{
    Key< TimeType, float > k1( 0.0, 1.0f );
    Key< TimeType, float > k2( 3.0, 5.0f );

    auto actual = DeserializeBezier( "TestAssets/Serialization/Interpolators/BezierLackOfDelta2.xml", CurveType::CT_BEZIER, k1, k2 );
    auto expected = CreateDummyInterpolator< TimeType, float >( CurveType::CT_BEZIER, k1, k2, tolerance );

    EXPECT_TRUE( ParamComparator::CompareEvaluators( *( expected.get() ), *( actual.get() ) ) );
}

// ***********************
// If any value lacks in xml, bezier should be created with default values.
TEST( Serialization_ParamValModel, Evaluators_Bezier_InvalidT1 )
{
    Key< TimeType, float > k1( 0.0, 1.0f );
    Key< TimeType, float > k2( 3.0, 5.0f );

    auto actual = DeserializeBezier( "TestAssets/Serialization/Interpolators/BezierInvalidT1.xml", CurveType::CT_BEZIER, k1, k2 );
    auto expected = CreateDummyInterpolator< TimeType, float >( CurveType::CT_BEZIER, k1, k2, tolerance );

    EXPECT_TRUE( ParamComparator::CompareEvaluators( *( expected.get() ), *( actual.get() ) ) );
}

// ***********************
// If any value lacks in xml, bezier should be created with default values.
TEST( Serialization_ParamValModel, Evaluators_Bezier_InvalidT2 )
{
    Key< TimeType, float > k1( 0.0, 1.0f );
    Key< TimeType, float > k2( 3.0, 5.0f );

    auto actual = DeserializeBezier( "TestAssets/Serialization/Interpolators/BezierInvalidT2.xml", CurveType::CT_BEZIER, k1, k2 );
    auto expected = CreateDummyInterpolator< TimeType, float >( CurveType::CT_BEZIER, k1, k2, tolerance );

    EXPECT_TRUE( ParamComparator::CompareEvaluators( *( expected.get() ), *( actual.get() ) ) );
}

// ***********************
// If any value lacks in xml, bezier should be created with default values.
TEST( Serialization_ParamValModel, Evaluators_Bezier_InvalidVal1 )
{
    Key< TimeType, float > k1( 0.0, 1.0f );
    Key< TimeType, float > k2( 3.0, 5.0f );

    auto actual = DeserializeBezier( "TestAssets/Serialization/Interpolators/BezierInvalidVal1.xml", CurveType::CT_BEZIER, k1, k2 );
    auto expected = CreateDummyInterpolator< TimeType, float >( CurveType::CT_BEZIER, k1, k2, tolerance );

    EXPECT_TRUE( ParamComparator::CompareEvaluators( *( expected.get() ), *( actual.get() ) ) );
}

// ***********************
// If any value lacks in xml, bezier should be created with default values.
TEST( Serialization_ParamValModel, Evaluators_Bezier_InvalidVal2 )
{
    Key< TimeType, float > k1( 0.0, 1.0f );
    Key< TimeType, float > k2( 3.0, 5.0f );

    auto actual = DeserializeBezier( "TestAssets/Serialization/Interpolators/BezierInvalidVal2.xml", CurveType::CT_BEZIER, k1, k2 );
    auto expected = CreateDummyInterpolator< TimeType, float >( CurveType::CT_BEZIER, k1, k2, tolerance );

    EXPECT_TRUE( ParamComparator::CompareEvaluators( *( expected.get() ), *( actual.get() ) ) );
}

