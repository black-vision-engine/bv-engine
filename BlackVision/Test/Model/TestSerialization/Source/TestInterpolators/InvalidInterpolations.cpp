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
// Xml contains more keys then expected numbers of interpolators.
// Interpolators with curve_type set in xml should be created.
TEST( Serialization_ParamValModel, Interpolators_MoreKeysThenInterpolators )
{
    auto actual = Deserialize< CompositeInterpolator< TimeType, float > >( "TestAssets/Serialization/Interpolators/NotEnoughInterpolators.xml", "interpolator" );
    ASSERT_NE( actual, nullptr );

    auto & actualEvals = TEST_ACCESSOR( CompositeInterpolator )::GetEvaluators( actual.get() );
    ASSERT_EQ( actualEvals.size(), 4 );

    EXPECT_EQ( actualEvals[ 2 ]->GetType(), EvaluatorType::ET_LINEAR );
    EXPECT_EQ( actualEvals[ 3 ]->GetType(), EvaluatorType::ET_LINEAR );
}

// ***********************
// Keys are not ordered by time. This means, that someon manually modified scene xml.
// There's no way to determine evaluators order. Best option is to leave order as it is.
TEST( Serialization_ParamValModel, Interpolators_BadKeysOrder )
{
    auto actual = Deserialize< CompositeInterpolator< TimeType, float > >( "TestAssets/Serialization/Interpolators/BadKeysOrder.xml", "interpolator" );
    ASSERT_NE( actual, nullptr );

    ASSERT_EQ( actual->GetNumKeys(), 4 );

    auto & actualEvals = TEST_ACCESSOR( CompositeInterpolator )::GetEvaluators( actual.get() );
    ASSERT_EQ( actualEvals.size(), 3 );

    // In this xml all evaluators have different curve type. This helpes to determine their order.
    EXPECT_EQ( actualEvals[ 0 ]->GetCurveType(), CurveType::CT_BEZIER );
    EXPECT_EQ( actualEvals[ 1 ]->GetCurveType(), CurveType::CT_POINT );
    EXPECT_EQ( actualEvals[ 2 ]->GetCurveType(), CurveType::CT_LINEAR );
}

// ***********************
// More interpolators then needed for number of keys in xml.
// Ignore all redundant interpolators.
TEST( Serialization_ParamValModel, Interpolators_ToManyInterpolators )
{
    auto actual = Deserialize< CompositeInterpolator< TimeType, float > >( "TestAssets/Serialization/Interpolators/ToManyInterpolators.xml", "interpolator" );
    ASSERT_NE( actual, nullptr );

    auto & actualEvals = TEST_ACCESSOR( CompositeInterpolator )::GetEvaluators( actual.get() );
    ASSERT_EQ( actualEvals.size(), 2 );
}

// ***********************
// There're no interpolators in xml. We should create default interpolators.
TEST( Serialization_ParamValModel, Interpolators_LackOfInterpolatorsInXML )
{
    auto actual = Deserialize< CompositeInterpolator< TimeType, float > >( "TestAssets/Serialization/Interpolators/LackOfInterpolators.xml", "interpolator" );
    ASSERT_NE( actual, nullptr );

    auto & actualEvals = TEST_ACCESSOR( CompositeInterpolator )::GetEvaluators( actual.get() );
    ASSERT_EQ( actualEvals.size(), 2 );
}


// ***********************
// There're no interpolations marker. We should create default interpolators.
TEST( Serialization_ParamValModel, Interpolators_NoInterpolationsMarkerInXML )
{
    auto actual = Deserialize< CompositeInterpolator< TimeType, float > >( "TestAssets/Serialization/Interpolators/NoInterpolationsMarker.xml", "interpolator" );
    ASSERT_NE( actual, nullptr );

    auto & actualEvals = TEST_ACCESSOR( CompositeInterpolator )::GetEvaluators( actual.get() );
    ASSERT_EQ( actualEvals.size(), 2 );
}

// ***********************
// Interpolator without type should be simply ignored.
TEST( Serialization_ParamValModel, Interpolators_InterpolatorWithoutType )
{
    auto actual = Deserialize< CompositeInterpolator< TimeType, float > >( "TestAssets/Serialization/Interpolators/InterpolatorWithoutType.xml", "interpolator" );
    ASSERT_NE( actual, nullptr );

    auto & actualEvals = TEST_ACCESSOR( CompositeInterpolator )::GetEvaluators( actual.get() );
    ASSERT_EQ( actualEvals.size(), 2 );

    EXPECT_EQ( actualEvals[ 0 ]->GetCurveType(), CurveType::CT_BEZIER );
    EXPECT_EQ( actualEvals[ 1 ]->GetCurveType(), CurveType::CT_LINEAR );
}

// ***********************
// Keys are set in the same time. Interpolator for the second key should be ignored.
// Note: This is no ideal solution since we don't know what someone, who modified xml, actually meant.
// But there's no way to determine, what interpolators should be used.
TEST( Serialization_ParamValModel, Interpolators_DoubledKey )
{
    auto actual = Deserialize< CompositeInterpolator< TimeType, float > >( "TestAssets/Serialization/Interpolators/DoubledKeys.xml", "interpolator" );
    ASSERT_NE( actual, nullptr );

    auto & actualEvals = TEST_ACCESSOR( CompositeInterpolator )::GetEvaluators( actual.get() );
    ASSERT_EQ( actualEvals.size(), 2 );

    EXPECT_EQ( actualEvals[ 0 ]->GetCurveType(), CurveType::CT_POINT );
    EXPECT_EQ( actualEvals[ 1 ]->GetCurveType(), CurveType::CT_QUARTIC_INOUT );
}

// ***********************
// One key has not parsing time value. Key will be ignored. Interpolations will be used
// in order of occurance in file.
// Note: It could be better solution to ignore interpolator the same way as in DoubledKey test.
// I'm not sure if it's worth the work needed for this.
TEST( Serialization_ParamValModel, Interpolators_NotParsingKey )
{
    auto actual = Deserialize< CompositeInterpolator< TimeType, float > >( "TestAssets/Serialization/Interpolators/NotParsingKeyTime.xml", "interpolator" );
    ASSERT_NE( actual, nullptr );

    auto & actualEvals = TEST_ACCESSOR( CompositeInterpolator )::GetEvaluators( actual.get() );
    ASSERT_EQ( actualEvals.size(), 2 );

    EXPECT_EQ( actualEvals[ 0 ]->GetCurveType(), CurveType::CT_POINT );
    EXPECT_EQ( actualEvals[ 1 ]->GetCurveType(), CurveType::CT_LINEAR );
}