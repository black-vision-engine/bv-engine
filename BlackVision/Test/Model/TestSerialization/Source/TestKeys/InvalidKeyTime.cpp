#include "gtest/gtest.h"


#include "Mathematics/Interpolators/CompositeInterpolator.h"
#include "Mathematics/Transform/MatTransform.h"

#include "Utils/Comparators/ParamComparator.h"
#include "Utils/Serialization/Serialize.h"
#include "Serialization/SerializationHelper.h"

#include "Mathematics/glm_inc.h"


using namespace bv;



// ***********************
// Load xml with invalid time value. Such key should be omitted.
// This test checks keys with both value and time invalid numbers too.
// This test works on float but it should cover all other parameters types too.
TEST( Serialization_ParamValModel, Keys_Float_InvalidTime )
{
    auto actual = Deserialize< CompositeInterpolator< TimeType, float > >( "TestAssets/Serialization/Keys/InvalidTimeValues.xml", "interpolator" );
    auto & keys = actual->GetKeys();

    ASSERT_EQ( keys.size(), 2 );

    EXPECT_EQ( keys[ 0 ].val, 3 );
    EXPECT_EQ( keys[ 1 ].val, -13 );

}






