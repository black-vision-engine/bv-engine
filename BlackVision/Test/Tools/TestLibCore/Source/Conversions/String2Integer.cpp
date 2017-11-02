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
class LibCore_String2T_Integer_Signed : public testing::Test
{
protected:
    LibCore_String2T_Integer_Signed() {};
};

// ***********************
//
template< class Type >
class LibCore_String2T_Integer_Unsigned : public testing::Test
{
protected:
    LibCore_String2T_Integer_Unsigned() {};
};

typedef testing::Types< UInt64, Int64, UInt32, Int32, UInt16, Int16, UInt8, Int8 > IntegerTypesList;
typedef testing::Types< Int64, Int32, Int16, Int8 > SignedIntegerTypesList;
typedef testing::Types< UInt64, UInt32, UInt16, UInt8 > UnsignedIntegerTypesList;

TYPED_TEST_CASE( LibCore_String2T_Integer, IntegerTypesList );
TYPED_TEST_CASE( LibCore_String2T_Integer_Signed, SignedIntegerTypesList );
TYPED_TEST_CASE( LibCore_String2T_Integer_Unsigned, UnsignedIntegerTypesList );


// ========================================================================= //
// Tests for different integer types
// ========================================================================= //


// ***********************
// Convert int from valid input data.
TYPED_TEST( LibCore_String2T_Integer, ValidInput )
{
    Expected< TypeParam > intExp = SerializationHelper::String2T< TypeParam >( "123" );
    TypeParam intDef = SerializationHelper::String2T< TypeParam >( "123", 5 );

    EXPECT_TRUE( intExp.IsValid() );
    EXPECT_EQ( intExp.GetVal(), 123 );
    EXPECT_EQ( intDef, 123 );
}

// ***********************
// Convert int from valid negative input data.
TYPED_TEST( LibCore_String2T_Integer_Signed, NegativeInput )
{
    Expected< TypeParam > intExp = SerializationHelper::String2T< TypeParam >( "-123" );
    TypeParam intDef = SerializationHelper::String2T< TypeParam >( "-123", 5 );

    EXPECT_TRUE( intExp.IsValid() );
    EXPECT_EQ( intExp.GetVal(), -123 );
    EXPECT_EQ( intDef, -123 );
}

// ***********************
// Convert int from valid negative input data.
TYPED_TEST( LibCore_String2T_Integer_Unsigned, NegativeInput )
{
    Expected< TypeParam > intExp = SerializationHelper::String2T< TypeParam >( "-123" );
    TypeParam intDef = SerializationHelper::String2T< TypeParam >( "-123", 5 );

    EXPECT_FALSE( intExp.IsValid() );
    EXPECT_EQ( intDef, 5 );
}

// ***********************
// Convert int from valid negative input data.
TYPED_TEST( LibCore_String2T_Integer, ConvertFromFloatStr )
{
    Expected< TypeParam > intExp = SerializationHelper::String2T< TypeParam >( "1.0434" );
    TypeParam intDef = SerializationHelper::String2T< TypeParam >( "1.0434", 5 );

    EXPECT_FALSE( intExp.IsValid() );
    EXPECT_EQ( intDef, 5 );
}
