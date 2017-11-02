#include "gtest/gtest.h"

#include "Serialization/SerializationHelper.h"


using namespace bv;


// ========================================================================= //
// Double conversion. These test have the same content as for float type.
// ========================================================================= //



// ***********************
// Converts string containing simple int value to double.
TEST( LibCore_String2T, Double_ConvertFromStrInt )
{
    Expected< double > doubleExp = SerializationHelper::String2T< double >( "123" );
    double doubleDef = SerializationHelper::String2T< double >( "123", 0.0f );

    EXPECT_TRUE( doubleExp.IsValid() );
    EXPECT_EQ( doubleExp.GetVal(), 123 );
    EXPECT_EQ( doubleDef, 123 );
}

// ***********************
// Converts string containing simple negative int value to double.
TEST( LibCore_String2T, Double_ConvertFromStrNegativeInt )
{
    Expected< double > doubleExp = SerializationHelper::String2T< double >( "-943" );
    double doubleDef = SerializationHelper::String2T< double >( "-943", 0.0f );

    EXPECT_TRUE( doubleExp.IsValid() );
    EXPECT_EQ( doubleExp.GetVal(), -943 );
    EXPECT_EQ( doubleDef, -943 );
}

// ***********************
// Converts string containing simple double.
TEST( LibCore_String2T, Double_ConvertFromStrWithDot )
{
    Expected< double > doubleExp = SerializationHelper::String2T< double >( "123.43002" );
    double doubleDef = SerializationHelper::String2T< double >( "123.43002", 0.0f );

    EXPECT_TRUE( doubleExp.IsValid() );
    EXPECT_EQ( doubleExp.GetVal(), 123.43002 );
    EXPECT_EQ( doubleDef, 123.43002 );
}

// ***********************
// Converts string containing simple double.
TEST( LibCore_String2T, Double_ConvertFromStrWithDotNegative )
{
    Expected< double > doubleExp = SerializationHelper::String2T< double >( "-123.43002" );
    double doubleDef = SerializationHelper::String2T< double >( "-123.43002", 0.0f );

    EXPECT_TRUE( doubleExp.IsValid() );
    EXPECT_EQ( doubleExp.GetVal(), -123.43002 );
    EXPECT_EQ( doubleDef, -123.43002 );
}

// ***********************
// Converts string containing double with negative exponent.
TEST( LibCore_String2T, Double_ConvertStrWithNegativeExponent )
{
    Expected< double > doubleExp = SerializationHelper::String2T< double >( "2.43001e-2" );
    double doubleDef = SerializationHelper::String2T< double >( "2.43001e-2", 0.0f );

    EXPECT_TRUE( doubleExp.IsValid() );
    EXPECT_EQ( doubleExp.GetVal(), 2.43001e-2 );
    EXPECT_EQ( doubleDef, 2.43001e-2 );
}

// ***********************
// Converts string containing double with exponent.
TEST( LibCore_String2T, Double_ConvertStrWitheExponent )
{
    Expected< double > doubleExp = SerializationHelper::String2T< double >( "2.43001e2" );
    double doubleDef = SerializationHelper::String2T< double >( "2.43001e2", 0.0f );

    EXPECT_TRUE( doubleExp.IsValid() );
    EXPECT_EQ( doubleExp.GetVal(), 2.43001e2 );
    EXPECT_EQ( doubleDef, 2.43001e2 );
}

// ***********************
// Converts string containing double with exponent. Here we test use of letter E instead of e.
TEST( LibCore_String2T, Double_ConvertStrWitheExponentE )
{
    Expected< double > doubleExp = SerializationHelper::String2T< double >( "2.43001E2" );
    double doubleDef = SerializationHelper::String2T< double >( "2.43001E2", 0.0f );

    EXPECT_TRUE( doubleExp.IsValid() );
    EXPECT_EQ( doubleExp.GetVal(), 2.43001e2 );
    EXPECT_EQ( doubleDef, 2.43001e2 );
}

// ***********************
// Converts string containing NAN. Not a number ingnores case
TEST( LibCore_String2T, Double_ConvertStrNaN )
{
    Expected< double > doubleExp = SerializationHelper::String2T< double >( "NaN" );
    double doubleDef = SerializationHelper::String2T< double >( "NaN", 0.0f );

    EXPECT_TRUE( doubleExp.IsValid() );
    EXPECT_FALSE( doubleExp.GetVal() == doubleExp.GetVal() );
    EXPECT_FALSE( doubleDef == doubleDef );
}

// ***********************
// Converts string containing infinity. Infinity ignores case.
TEST( LibCore_String2T, Double_ConvertStrInifinity )
{
    Expected< double > doubleExp = SerializationHelper::String2T< double >( "Inf" );
    double doubleDef = SerializationHelper::String2T< double >( "Inf", 0.0f );

    EXPECT_TRUE( doubleExp.IsValid() );
    EXPECT_TRUE( std::isinf( doubleExp.GetVal() ) );
    EXPECT_TRUE( std::isinf( doubleDef ) );

    doubleExp = SerializationHelper::String2T< double >( "InFiniTy" );
    doubleDef = SerializationHelper::String2T< double >( "InFiniTy", 0.0f );

    EXPECT_TRUE( doubleExp.IsValid() );
    EXPECT_TRUE( std::isinf( doubleExp.GetVal() ) );
    EXPECT_TRUE( std::isinf( doubleDef ) );
}

// ***********************
// Converts string containing double preceded by illigal text.
TEST( LibCore_String2T, Double_TextBeforeDouble )
{
    Expected< double > doubleExp = SerializationHelper::String2T< double >( "blabla3.14" );
    double doubleDef = SerializationHelper::String2T< double >( "blabla3.14", 0.0f );

    EXPECT_FALSE( doubleExp.IsValid() );
    EXPECT_EQ( doubleDef, 0.0 );
}

// ***********************
// Converts string containing double followed by illigal text.
TEST( LibCore_String2T, Double_TextAfterDouble )
{
    Expected< double > doubleExp = SerializationHelper::String2T< double >( "3.14blabla" );
    double doubleDef = SerializationHelper::String2T< double >( "3.14blabla", 0.0f );

    EXPECT_FALSE( doubleExp.IsValid() );
    EXPECT_EQ( doubleDef, 0.0 );
}

// ***********************
// Converts string containing infinity text followed by double.
TEST( LibCore_String2T, Double_InfinityBeforeDouble )
{
    Expected< double > doubleExp = SerializationHelper::String2T< double >( "Inf3.14" );
    double doubleDef = SerializationHelper::String2T< double >( "Inf3.14", 0.0f );

    EXPECT_FALSE( doubleExp.IsValid() );
    EXPECT_EQ( doubleDef, 0.0 );
}

// ***********************
// Converts string containing double preceded by illigal text with space.
// Whole string passed to function must be double. Function never ignores anything.
TEST( LibCore_String2T, Double_TextSpaceDouble )
{
    Expected< double > doubleExp = SerializationHelper::String2T< double >( "blabla 3.14" );
    double doubleDef = SerializationHelper::String2T< double >( "blabla 3.14", 0.0f );

    EXPECT_FALSE( doubleExp.IsValid() );
    EXPECT_EQ( doubleDef, 0.0 );
}

// ========================================================================= //
// Test only for double different then in String2Float
// ========================================================================= //


// ***********************
// Convertes string containing floating point number greater then float range.
TEST( LibCore_String2T, Double_NumberGreaterThenFloatRange )
{
    Expected< double > doubleExp = SerializationHelper::String2T< double >( "3.02e100" );
    double doubleDef = SerializationHelper::String2T< double >( "3.02e100", 0.0f );

    EXPECT_TRUE( doubleExp.IsValid() );
    EXPECT_EQ( doubleExp.GetVal(), 3.02e100 );
    EXPECT_EQ( doubleDef, 3.02e100 );
}

// ***********************
// Convertes string containing floating point number greater then double range.
// To big number is converted to infinity.
TEST( LibCore_String2T, Double_NumberGreaterDoubleFloatRange )
{
    Expected< double > doubleExp = SerializationHelper::String2T< double >( "3.02e500" );
    double doubleDef = SerializationHelper::String2T< double >( "3.02e500", 0.0f );

    EXPECT_TRUE( doubleExp.IsValid() );
    EXPECT_TRUE( std::isinf( doubleExp.GetVal() ) );
    EXPECT_TRUE( std::isinf( doubleDef ) );
}
