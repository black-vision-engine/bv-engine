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


