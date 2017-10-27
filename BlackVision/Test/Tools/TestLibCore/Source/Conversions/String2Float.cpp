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

// ***********************
// Converts string containing simple negative int value to float.
TEST( LibCore_String2T, Float_ConvertFromStrNegativeInt )
{
    Expected< float > trueExp = SerializationHelper::String2T< float >( "-943" );
    float trueDef = SerializationHelper::String2T< float >( "-943", 0.0f );

    EXPECT_TRUE( trueExp.IsValid() );
    EXPECT_EQ( trueExp.GetVal(), -943 );
    EXPECT_EQ( trueDef, -943 );
}

// ***********************
// Converts string containing simple float.
TEST( LibCore_String2T, Float_ConvertFromStrWithDot )
{
    Expected< float > trueExp = SerializationHelper::String2T< float >( "123.43002" );
    float trueDef = SerializationHelper::String2T< float >( "123.43002", 0.0f );

    EXPECT_TRUE( trueExp.IsValid() );
    EXPECT_EQ( trueExp.GetVal(), 123.43002f );
    EXPECT_EQ( trueDef, 123.43002f );
}

// ***********************
// Converts string containing simple float.
TEST( LibCore_String2T, Float_ConvertFromStrWithDotNegative )
{
    Expected< float > trueExp = SerializationHelper::String2T< float >( "-123.43002" );
    float trueDef = SerializationHelper::String2T< float >( "-123.43002", 0.0f );

    EXPECT_TRUE( trueExp.IsValid() );
    EXPECT_EQ( trueExp.GetVal(), -123.43002f );
    EXPECT_EQ( trueDef, -123.43002f );
}

// ***********************
// Converts string containing float with negative exponent.
TEST( LibCore_String2T, Float_ConvertStrWithNegativeExponent )
{
    Expected< float > trueExp = SerializationHelper::String2T< float >( "2.43001e-2" );
    float trueDef = SerializationHelper::String2T< float >( "2.43001e-2", 0.0f );

    EXPECT_TRUE( trueExp.IsValid() );
    EXPECT_EQ( trueExp.GetVal(), 2.43001e-2f );
    EXPECT_EQ( trueDef, 2.43001e-2f );
}

// ***********************
// Converts string containing float with exponent.
TEST( LibCore_String2T, Float_ConvertStrWitheExponent )
{
    Expected< float > trueExp = SerializationHelper::String2T< float >( "2.43001e2" );
    float trueDef = SerializationHelper::String2T< float >( "2.43001e2", 0.0f );

    EXPECT_TRUE( trueExp.IsValid() );
    EXPECT_EQ( trueExp.GetVal(), 2.43001e2f );
    EXPECT_EQ( trueDef, 2.43001e2f );
}

// ***********************
// Converts string containing float with exponent. Here we test use of letter E instead of e.
TEST( LibCore_String2T, Float_ConvertStrWitheExponentE )
{
    Expected< float > trueExp = SerializationHelper::String2T< float >( "2.43001E2" );
    float trueDef = SerializationHelper::String2T< float >( "2.43001E2", 0.0f );

    EXPECT_TRUE( trueExp.IsValid() );
    EXPECT_EQ( trueExp.GetVal(), 2.43001e2f );
    EXPECT_EQ( trueDef, 2.43001e2f );
}
