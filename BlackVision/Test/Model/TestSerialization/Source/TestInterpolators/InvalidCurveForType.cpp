#include "gtest/gtest.h"



#include "Mathematics/Transform/MatTransform.h"

#include "Utils/Comparators/ParamComparator.h"
#include "Utils/Serialization/SerializeInteprolator.h"

#include "Framework/UseLoggerTests.h"


#include "Mathematics/Interpolators/CompositeInterpolator.h"
#include "Mathematics/Interpolators/CompositeInterpolator.inl"


using namespace bv;




// ***********************
// Creating interpolator linear interpolator for string returns nullptr.
TEST( Serialization_ParamValModel, Interpolators_LinearCurveForString )
{
    auto interpolator = CreateDummyInterpolator( CurveType::CT_LINEAR, Key< TimeType, std::string >( 0.0, std::string() ), Key< TimeType, std::string >( 2.0, std::string() ), std::numeric_limits< float >::epsilon() );
    EXPECT_EQ( interpolator, nullptr );
}

// ***********************
// Creating interpolator linear interpolator for Wstring returns nullptr.
TEST( Serialization_ParamValModel, Interpolators_LinearCurveForwString )
{
    auto interpolator = CreateDummyInterpolator( CurveType::CT_LINEAR, Key< TimeType, std::wstring >( 0.0, std::wstring() ), Key< TimeType, std::wstring >( 2.0, std::wstring() ), std::numeric_limits< float >::epsilon() );
    EXPECT_EQ( interpolator, nullptr );
}

// ***********************
// Create interpolator with linear interpolation for string type.
// This is invalid combination. Interpolator should use default value for this type.
TEST( Serialization_ParamValModel, Interpolators_LinearCurveForStringXML )
{
    std::shared_ptr< CompositeInterpolator< TimeType, std::string > > actual;
    
    EXPECT_NO_THROW( {
    actual = DeserializeFloatCompositeInterpolator( "TestAssets/Serialization/Interpolators/InvalidCurveForString.xml" );
    } );

    ASSERT_NE( actual, nullptr );

    EXPECT_EQ( actual->GetCurveType(), CurveType::CT_POINT );

    auto & actualEvals = TEST_ACCESSOR( CompositeInterpolator )::GetEvaluators( actual.get() );
    ASSERT_EQ( actualEvals.size(), 1 );

    EXPECT_EQ( actualEvals[ 0 ]->GetType(), EvaluatorType::ET_CONSTANT );
    EXPECT_EQ( actualEvals[ 0 ]->GetCurveType(), CurveType::CT_POINT);
}

// ***********************
// One interpolation has linear interpolator which is invalid for string type.
// Deserialization should create default interpolator instead of linear.
TEST( Serialization_ParamValModel, Interpolators_InvalidCurveHandling )
{
    std::shared_ptr< CompositeInterpolator< TimeType, std::string > > actual;

    EXPECT_NO_THROW( {
        actual = DeserializeFloatCompositeInterpolator( "TestAssets/Serialization/Interpolators/InvalidCurveHandling.xml" );
    } );

    ASSERT_NE( actual, nullptr );

    auto & actualEvals = TEST_ACCESSOR( CompositeInterpolator )::GetEvaluators( actual.get() );
    ASSERT_EQ( actualEvals.size(), 3 );

    EXPECT_EQ( actualEvals[ 0 ]->GetCurveType(), CurveType::CT_POINT );
    EXPECT_EQ( actualEvals[ 1 ]->GetCurveType(), CurveType::CT_POINT );
    EXPECT_EQ( actualEvals[ 2 ]->GetCurveType(), CurveType::CT_POINT );
}

// ***********************
// This test contains both errors - invalid curve type and doubled corresponding keys.
TEST( Serialization_ParamValModel, Interpolators_InvalidCurveAndDoubledKey )
{
    std::shared_ptr< CompositeInterpolator< TimeType, std::string > > actual;

    EXPECT_NO_THROW( {
        actual = DeserializeFloatCompositeInterpolator( "TestAssets/Serialization/Interpolators/InvalidCurveAndDoubledKey.xml" );
    } );

    ASSERT_NE( actual, nullptr );

    auto & actualEvals = TEST_ACCESSOR( CompositeInterpolator )::GetEvaluators( actual.get() );
    ASSERT_EQ( actualEvals.size(), 2 );

    EXPECT_EQ( actualEvals[ 0 ]->GetCurveType(), CurveType::CT_POINT );
    EXPECT_EQ( actualEvals[ 1 ]->GetCurveType(), CurveType::CT_POINT );
}


