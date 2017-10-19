#include "gtest/gtest.h"


#include "Mathematics/Interpolators/CompositeInterpolator.h"
#include "Mathematics/Transform/MatTransform.h"

#include "Utils/Comparators/ParamComparator.h"


#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"
#include "Engine/Models/Timeline/TimelineHelper.h"


using namespace bv;



// ***********************
// Interpolator shouldn't crash, when evaluating value without keys.
TEST( Model_ParamValModel, CompositeInterpolator_EvaluateZeroKeys )
{
    CompositeInterpolator< TimeType, float > interpolator;

    EXPECT_NO_THROW( { interpolator.Evaluate( 0.0f ); } );
    EXPECT_NO_THROW( { interpolator.Evaluate( 1.0f ); } );
    EXPECT_NO_THROW( { interpolator.Evaluate( -1.0f ); } );
    EXPECT_NO_THROW( { interpolator.Evaluate( 100.0f ); } );
    EXPECT_NO_THROW( { interpolator.Evaluate( -100.0f ); } );
}

// ***********************
// Interpolator shouldn't crash, when evaluating negative time values.
TEST( Model_ParamValModel, CompositeInterpolator_NegativeKeys )
{
    CompositeInterpolator< TimeType, float > interpolator;

    interpolator.AddKey( -1.0f, 2.0f );
    interpolator.AddKey( 1.0f, 4.0f );

    float result = 1000;

    EXPECT_NO_THROW( { result = interpolator.Evaluate( 0.0f ); } );

    EXPECT_NEAR( result, 3.0f, 0.00001f );
}
