#include "gtest/gtest.h"



#include "Mathematics/Transform/MatTransform.h"

#include "Utils/Comparators/ParamComparator.h"
#include "Utils/Serialization/Serialize.h"

#include "Framework/UseLoggerTests.h"

#include "Mathematics/Interpolators/CompositeInterpolator.h"
#include "Mathematics/Interpolators/CompositeInterpolator.inl"


using namespace bv;



// ***********************
// Test serialization of CurveType and pre, post WrapMEthod.
TEST( Serialization_ParamValModel, Interpolators_CompositeInteprolatorParams )
{
    auto actual = Deserialize< CompositeInterpolator< TimeType, float > >( "TestAssets/Serialization/Interpolators/CurveTypeAndWrapMethods.xml", "interpolator" );
    ASSERT_NE( actual, nullptr );

    EXPECT_EQ( actual->GetWrapPreMethod(), WrapMethod::pingPong );
    EXPECT_EQ( actual->GetWrapPostMethod(), WrapMethod::repeat );
    EXPECT_EQ( actual->GetCurveType(), CurveType::CT_ELASTIC_OUT_BOUNCE );
}



