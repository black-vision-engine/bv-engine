#include "gtest/gtest.h"

#include "Serialization/SerializationHelper.h"

using namespace bv;



// ***********************
// Converts string containing simple int value to float.
TEST( LibCore_String2T, Float_ConvertFromStrInt )
{
    Expected< float > floatExp = SerializationHelper::String2T< float >( "123" );
    float floatDef = SerializationHelper::String2T< float >( "123", 0.0f );

    EXPECT_TRUE( floatExp.IsValid() );
    EXPECT_EQ( floatExp.GetVal(), 123 );
    EXPECT_EQ( floatDef, 123 );
}

// ***********************
// Converts string containing simple negative int value to float.
TEST( LibCore_String2T, Float_ConvertFromStrNegativeInt )
{
    Expected< float > floatExp = SerializationHelper::String2T< float >( "-943" );
    float floatDef = SerializationHelper::String2T< float >( "-943", 0.0f );

    EXPECT_TRUE( floatExp.IsValid() );
    EXPECT_EQ( floatExp.GetVal(), -943 );
    EXPECT_EQ( floatDef, -943 );
}

// ***********************
// Converts string containing simple float.
TEST( LibCore_String2T, Float_ConvertFromStrWithDot )
{
    Expected< float > floatExp = SerializationHelper::String2T< float >( "123.43002" );
    float floatDef = SerializationHelper::String2T< float >( "123.43002", 0.0f );

    EXPECT_TRUE( floatExp.IsValid() );
    EXPECT_EQ( floatExp.GetVal(), 123.43002f );
    EXPECT_EQ( floatDef, 123.43002f );
}

// ***********************
// Converts string containing simple float.
TEST( LibCore_String2T, Float_ConvertFromStrWithDotNegative )
{
    Expected< float > floatExp = SerializationHelper::String2T< float >( "-123.43002" );
    float floatDef = SerializationHelper::String2T< float >( "-123.43002", 0.0f );

    EXPECT_TRUE( floatExp.IsValid() );
    EXPECT_EQ( floatExp.GetVal(), -123.43002f );
    EXPECT_EQ( floatDef, -123.43002f );
}

// ***********************
// Converts string containing float with negative exponent.
TEST( LibCore_String2T, Float_ConvertStrWithNegativeExponent )
{
    Expected< float > floatExp = SerializationHelper::String2T< float >( "2.43001e-2" );
    float floatDef = SerializationHelper::String2T< float >( "2.43001e-2", 0.0f );

    EXPECT_TRUE( floatExp.IsValid() );
    EXPECT_EQ( floatExp.GetVal(), 2.43001e-2f );
    EXPECT_EQ( floatDef, 2.43001e-2f );
}

// ***********************
// Converts string containing float with exponent.
TEST( LibCore_String2T, Float_ConvertStrWitheExponent )
{
    Expected< float > floatExp = SerializationHelper::String2T< float >( "2.43001e2" );
    float floatDef = SerializationHelper::String2T< float >( "2.43001e2", 0.0f );

    EXPECT_TRUE( floatExp.IsValid() );
    EXPECT_EQ( floatExp.GetVal(), 2.43001e2f );
    EXPECT_EQ( floatDef, 2.43001e2f );
}

// ***********************
// Converts string containing float with exponent. Here we test use of letter E instead of e.
TEST( LibCore_String2T, Float_ConvertStrWitheExponentE )
{
    Expected< float > floatExp = SerializationHelper::String2T< float >( "2.43001E2" );
    float floatDef = SerializationHelper::String2T< float >( "2.43001E2", 0.0f );

    EXPECT_TRUE( floatExp.IsValid() );
    EXPECT_EQ( floatExp.GetVal(), 2.43001e2f );
    EXPECT_EQ( floatDef, 2.43001e2f );
}

// ***********************
// Converts string containing NAN. Not a number ingnores case
TEST( LibCore_String2T, Float_ConvertStrNaN )
{
    Expected< float > floatExp = SerializationHelper::String2T< float >( "NaN" );
    float floatDef = SerializationHelper::String2T< float >( "NaN", 0.0f );

    EXPECT_TRUE( floatExp.IsValid() );
    EXPECT_FALSE( floatExp.GetVal() == floatExp.GetVal() );
    EXPECT_FALSE( floatDef == floatDef );
}

// ***********************
// Converts string containing infinity. Infinity ignores case.
TEST( LibCore_String2T, Float_ConvertStrInifinity )
{
    Expected< float > floatExp = SerializationHelper::String2T< float >( "Inf" );
    float floatDef = SerializationHelper::String2T< float >( "Inf", 0.0f );

    EXPECT_TRUE( floatExp.IsValid() );
    EXPECT_TRUE( std::isinf( floatExp.GetVal() ) );
    EXPECT_TRUE( std::isinf( floatDef ) );

    floatExp = SerializationHelper::String2T< float >( "InFiniTy" );
    floatDef = SerializationHelper::String2T< float >( "InFiniTy", 0.0f );

    EXPECT_TRUE( floatExp.IsValid() );
    EXPECT_TRUE( std::isinf( floatExp.GetVal() ) );
    EXPECT_TRUE( std::isinf( floatDef) );
}
