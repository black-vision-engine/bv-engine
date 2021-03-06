#include "gtest/gtest.h"

#include "Serialization/SerializationHelper.h"


using namespace bv;


// ========================================================================= //
// Test fixture for templated integer types
// ========================================================================= //



// ***********************
//
template< class Type >
class LibCore_String2T_Integer : public testing::Test
{
protected:
    LibCore_String2T_Integer() {};
};

// ***********************
//
template< class Type >
class LibCore_String2T_IntegerSigned : public testing::Test
{
protected:
    LibCore_String2T_IntegerSigned() {};
};

// ***********************
//
template< class Type >
class LibCore_String2T_IntegerUnsigned : public testing::Test
{
protected:
    LibCore_String2T_IntegerUnsigned() {};
};

typedef testing::Types< UInt64, Int64, UInt32, Int32, UInt16, Int16, UInt8, Int8 > IntegerTypesList;
typedef testing::Types< Int64, Int32, Int16, Int8 > SignedIntegerTypesList;
typedef testing::Types< UInt64, UInt32, UInt16, UInt8 > UnsignedIntegerTypesList;

TYPED_TEST_CASE( LibCore_String2T_Integer, IntegerTypesList );
TYPED_TEST_CASE( LibCore_String2T_IntegerSigned, SignedIntegerTypesList );
TYPED_TEST_CASE( LibCore_String2T_IntegerUnsigned, UnsignedIntegerTypesList );


// ========================================================================= //
// Tests for different integer types
// ========================================================================= //


// ***********************
// Convert int from valid input data.
TYPED_TEST( LibCore_String2T_Integer, ValidInput )
{
    Expected< TypeParam > intExp = Convert::String2T< TypeParam >( "123" );
    TypeParam intDef = Convert::String2T< TypeParam >( "123", 5 );

    EXPECT_TRUE( intExp.IsValid() );
    EXPECT_EQ( intExp.GetVal(), 123 );
    EXPECT_EQ( intDef, 123 );
}

// ***********************
// Convert int from valid negative input data.
TYPED_TEST( LibCore_String2T_IntegerSigned, NegativeInput )
{
    Expected< TypeParam > intExp = Convert::String2T< TypeParam >( "-123" );
    TypeParam intDef = Convert::String2T< TypeParam >( "-123", 5 );

    EXPECT_TRUE( intExp.IsValid() );
    EXPECT_EQ( intExp.GetVal(), -123 );
    EXPECT_EQ( intDef, -123 );
}

// ***********************
// Convert int from valid negative input data.
TYPED_TEST( LibCore_String2T_IntegerUnsigned, NegativeInput )
{
    Expected< TypeParam > intExp = Convert::String2T< TypeParam >( "-123" );
    TypeParam intDef = Convert::String2T< TypeParam >( "-123", 5 );

    EXPECT_FALSE( intExp.IsValid() );
    EXPECT_EQ( intDef, 5 );
}

// ***********************
// Convert int from floating point number in string.
TYPED_TEST( LibCore_String2T_Integer, ConvertFromFloatStr )
{
    Expected< TypeParam > intExp = Convert::String2T< TypeParam >( "1.0434" );
    TypeParam intDef = Convert::String2T< TypeParam >( "1.0434", 5 );

    EXPECT_FALSE( intExp.IsValid() );
    EXPECT_EQ( intDef, 5 );
}

// ***********************
// Try to convert overflowing number. Conversion should fail. No implicit truncation should be made.
TYPED_TEST( LibCore_String2T_Integer, ConvertOverflowing )
{
    Expected< TypeParam > intExp = Convert::String2T< TypeParam >( "48446744073709551615" );
    TypeParam intDef = Convert::String2T< TypeParam >( "48446744073709551615", 5 );

    EXPECT_FALSE( intExp.IsValid() );
    EXPECT_EQ( intDef, 5 );
}

// ***********************
// Try to convert underflowing number. Conversion should fail. No implicit truncation should be made.
TYPED_TEST( LibCore_String2T_Integer, ConvertUnderflowing )
{
    Expected< TypeParam > intExp = Convert::String2T< TypeParam >( "-48446744073709551615" );
    TypeParam intDef = Convert::String2T< TypeParam >( "-48446744073709551615", 5 );

    EXPECT_FALSE( intExp.IsValid() );
    EXPECT_EQ( intDef, 5 );
}

// ***********************
// Try to convert string with text before number.
TYPED_TEST( LibCore_String2T_Integer, TextBeforeNumber )
{
    Expected< TypeParam > intExp = Convert::String2T< TypeParam >( "bla13" );
    TypeParam intDef = Convert::String2T< TypeParam >( "bla13", 5 );

    EXPECT_FALSE( intExp.IsValid() );
    EXPECT_EQ( intDef, 5 );
}

// ***********************
// Try to convert string with text after number.
TYPED_TEST( LibCore_String2T_Integer, TextAfterNumber )
{
    Expected< TypeParam > intExp = Convert::String2T< TypeParam >( "13bla" );
    TypeParam intDef = Convert::String2T< TypeParam >( "13bla", 5 );

    EXPECT_FALSE( intExp.IsValid() );
    EXPECT_EQ( intDef, 5 );
}

// ***********************
// Try to convert string with text in the middle of number.
TYPED_TEST( LibCore_String2T_Integer, TextInTheMiddleOfNumber )
{
    Expected< TypeParam > intExp = Convert::String2T< TypeParam >( "13bla234" );
    TypeParam intDef = Convert::String2T< TypeParam >( "13bla234", 5 );

    EXPECT_FALSE( intExp.IsValid() );
    EXPECT_EQ( intDef, 5 );
}

// ***********************
// Check if conversion is inversible. T2String should produce value which can be converted back to typed value.
TYPED_TEST( LibCore_String2T_Integer, ConversionReversibility )
{
    Expected< TypeParam > intExp = Convert::String2T< TypeParam >( Convert::T2String( 123 ) );

    EXPECT_TRUE( intExp.IsValid() );
    EXPECT_EQ( intExp.GetVal(), 123 );
}
