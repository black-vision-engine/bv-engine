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

