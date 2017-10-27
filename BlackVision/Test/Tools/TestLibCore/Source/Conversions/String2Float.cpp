#include "gtest/gtest.h"

#include "Serialization/SerializationHelper.h"

using namespace bv;



// ***********************
// Converts string containing simple int value to float.
TEST( LibCore_String2T, Float_ConvertFromStrInt )
{
    Expected< float > trueExp = SerializationHelper::String2T< float >( "123" );
    float trueDef = SerializationHelper::String2T< float >( "123", 0.0f );

    EXPECT_TRUE( trueExp.IsValid() );
    EXPECT_EQ( trueExp.GetVal(), 123 );
    EXPECT_EQ( trueDef, 123 );
}
