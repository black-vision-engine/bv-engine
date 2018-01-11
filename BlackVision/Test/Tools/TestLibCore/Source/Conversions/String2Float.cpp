#include "gtest/gtest.h"

#include "Serialization/SerializationHelper.h"

using namespace bv;



// ***********************
// Converts string containing simple int value to float.
TEST( LibCore_String2T, Float_ConvertFromStrInt )
{
    Expected< float > floatExp = Convert::String2T< float >( "123" );
    float floatDef = Convert::String2T< float >( "123", 0.0f );

    EXPECT_TRUE( floatExp.IsValid() );
    EXPECT_EQ( floatExp.GetVal(), 123 );
    EXPECT_EQ( floatDef, 123 );
}

// ***********************
// Converts string containing simple negative int value to float.
TEST( LibCore_String2T, Float_ConvertFromStrNegativeInt )
{
    Expected< float > floatExp = Convert::String2T< float >( "-943" );
    float floatDef = Convert::String2T< float >( "-943", 0.0f );

    EXPECT_TRUE( floatExp.IsValid() );
    EXPECT_EQ( floatExp.GetVal(), -943 );
    EXPECT_EQ( floatDef, -943 );
}

// ***********************
// Converts string containing simple float.
TEST( LibCore_String2T, Float_ConvertFromStrWithDot )
{
    Expected< float > floatExp = Convert::String2T< float >( "123.43002" );
    float floatDef = Convert::String2T< float >( "123.43002", 0.0f );

    EXPECT_TRUE( floatExp.IsValid() );
    EXPECT_EQ( floatExp.GetVal(), 123.43002f );
    EXPECT_EQ( floatDef, 123.43002f );
}

// ***********************
// Converts string containing simple float.
TEST( LibCore_String2T, Float_ConvertFromStrWithDotNegative )
{
    Expected< float > floatExp = Convert::String2T< float >( "-123.43002" );
    float floatDef = Convert::String2T< float >( "-123.43002", 0.0f );

    EXPECT_TRUE( floatExp.IsValid() );
    EXPECT_EQ( floatExp.GetVal(), -123.43002f );
    EXPECT_EQ( floatDef, -123.43002f );
}

// ***********************
// Converts string containing float with negative exponent.
TEST( LibCore_String2T, Float_ConvertStrWithNegativeExponent )
{
    Expected< float > floatExp = Convert::String2T< float >( "2.43001e-2" );
    float floatDef = Convert::String2T< float >( "2.43001e-2", 0.0f );

    EXPECT_TRUE( floatExp.IsValid() );
    EXPECT_EQ( floatExp.GetVal(), 2.43001e-2f );
    EXPECT_EQ( floatDef, 2.43001e-2f );
}

// ***********************
// Converts string containing float with exponent.
TEST( LibCore_String2T, Float_ConvertStrWitheExponent )
{
    Expected< float > floatExp = Convert::String2T< float >( "2.43001e2" );
    float floatDef = Convert::String2T< float >( "2.43001e2", 0.0f );

    EXPECT_TRUE( floatExp.IsValid() );
    EXPECT_EQ( floatExp.GetVal(), 2.43001e2f );
    EXPECT_EQ( floatDef, 2.43001e2f );
}

// ***********************
// Converts string containing float with exponent. Here we test use of letter E instead of e.
TEST( LibCore_String2T, Float_ConvertStrWitheExponentE )
{
    Expected< float > floatExp = Convert::String2T< float >( "2.43001E2" );
    float floatDef = Convert::String2T< float >( "2.43001E2", 0.0f );

    EXPECT_TRUE( floatExp.IsValid() );
    EXPECT_EQ( floatExp.GetVal(), 2.43001e2f );
    EXPECT_EQ( floatDef, 2.43001e2f );
}

// ***********************
// Converts string containing NAN. Not a number ingnores case
TEST( LibCore_String2T, Float_ConvertStrNaN )
{
    Expected< float > floatExp = Convert::String2T< float >( "NaN" );
    float floatDef = Convert::String2T< float >( "NaN", 0.0f );

    EXPECT_TRUE( floatExp.IsValid() );
    EXPECT_FALSE( floatExp.GetVal() == floatExp.GetVal() );
    EXPECT_FALSE( floatDef == floatDef );
}

// ***********************
// Converts string containing infinity. Infinity ignores case.
TEST( LibCore_String2T, Float_ConvertStrInifinity )
{
    Expected< float > floatExp = Convert::String2T< float >( "Inf" );
    float floatDef = Convert::String2T< float >( "Inf", 0.0f );

    EXPECT_TRUE( floatExp.IsValid() );
    EXPECT_TRUE( std::isinf( floatExp.GetVal() ) );
    EXPECT_TRUE( std::isinf( floatDef ) );

    floatExp = Convert::String2T< float >( "InFiniTy" );
    floatDef = Convert::String2T< float >( "InFiniTy", 0.0f );

    EXPECT_TRUE( floatExp.IsValid() );
    EXPECT_TRUE( std::isinf( floatExp.GetVal() ) );
    EXPECT_TRUE( std::isinf( floatDef) );
}

// ***********************
// Converts string containing float preceded by illigal text.
TEST( LibCore_String2T, Float_TextBeforeFloat )
{
    Expected< float > floatExp = Convert::String2T< float >( "blabla3.14" );
    float floatDef = Convert::String2T< float >( "blabla3.14", 0.0f );

    EXPECT_FALSE( floatExp.IsValid() );
    EXPECT_EQ( floatDef, 0.0f );
}

// ***********************
// Converts string containing float followed by illigal text.
TEST( LibCore_String2T, Float_TextAfterFloat )
{
    Expected< float > floatExp = Convert::String2T< float >( "3.14blabla" );
    float floatDef = Convert::String2T< float >( "3.14blabla", 0.0f );

    EXPECT_FALSE( floatExp.IsValid() );
    EXPECT_EQ( floatDef, 0.0f );
}

// ***********************
// Converts string containing infinity text followed by float.
TEST( LibCore_String2T, Float_InfinityBeforeFloat )
{
    Expected< float > floatExp = Convert::String2T< float >( "Inf3.14" );
    float floatDef = Convert::String2T< float >( "Inf3.14", 0.0f );

    EXPECT_FALSE( floatExp.IsValid() );
    EXPECT_EQ( floatDef, 0.0f );
}

// ***********************
// Converts string containing float preceded by illigal text with space.
// Whole string passed to function must be float. Function never ignores anything.
TEST( LibCore_String2T, Float_TextSpaceFloat )
{
    Expected< float > floatExp = Convert::String2T< float >( "blabla 3.14" );
    float floatDef = Convert::String2T< float >( "blabla 3.14", 0.0f );

    EXPECT_FALSE( floatExp.IsValid() );
    EXPECT_EQ( floatDef, 0.0f );
}

// ***********************
// Check if conversion is inversible. T2String should produce value which can be converted back to typed value.
TEST( LibCore_String2T, Float_ConversionReversibility )
{
    Expected< float > floatExp = Convert::String2T< float >( Convert::T2String( 1.034423f ) );

    EXPECT_TRUE( floatExp.IsValid() );
    EXPECT_EQ( floatExp.GetVal(), 1.034423f );
}


// ========================================================================= //
// Test only for float different then in String2Double
// ========================================================================= //


// ***********************
// Convertes string containing floating point number greater then float range.
// To big number is converted to infinity.
TEST( LibCore_String2T, Float_NumberGreaterThenFloatRange )
{
    Expected< float > floatExp = Convert::String2T< float >( "3.02e100" );
    float floatDef = Convert::String2T< float >( "3.02e100", 0.0f );

    EXPECT_TRUE( floatExp.IsValid() );
    EXPECT_TRUE( std::isinf( floatExp.GetVal() ) );
    EXPECT_TRUE( std::isinf( floatDef ) );
}

