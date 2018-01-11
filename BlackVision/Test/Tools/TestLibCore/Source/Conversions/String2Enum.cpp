#include "gtest/gtest.h"

#include "Serialization/SerializationHelper.h"


using namespace bv;



// ========================================================================= //
// Enum declaration
// ========================================================================= //

namespace bv
{

// ***********************
//
enum TestEnum
{
    Value1,
    ValueSecond,
    BlablaValue,

    TE_Total
};

DECLARE_ENUM_SERIALIZATION( TestEnum )


namespace Convert
{

// ***********************
//
static const std::pair< TestEnum, const char* > te2s_data[] =
{
    std::make_pair( TestEnum::Value1, "Value1" )
    , std::make_pair( TestEnum::ValueSecond, "ValueSecond" )
    , std::make_pair( TestEnum::BlablaValue, "BlablaValue" )
    , std::make_pair( TestEnum::ValueSecond, "" )
};

IMPLEMENT_ENUM_SERIALIZATION( TestEnum, te2s_data )

}

}


// ========================================================================= //
// Tests
// ========================================================================= //


// ***********************
//
TEST( LibCore_String2T, Enum_ValidInput )
{
    Expected< TestEnum > enumExp = Convert::String2T< TestEnum >( "ValueSecond" );
    TestEnum enumDef = Convert::String2T< TestEnum >( "ValueSecond", TestEnum::TE_Total );

    EXPECT_TRUE( enumExp.IsValid() );
    EXPECT_EQ( enumExp.GetVal(), TestEnum::ValueSecond );
    EXPECT_EQ( enumDef, TestEnum::ValueSecond );

    enumExp = Convert::String2T< TestEnum >( "BlablaValue" );
    enumDef = Convert::String2T< TestEnum >( "BlablaValue", TestEnum::TE_Total );

    EXPECT_TRUE( enumExp.IsValid() );
    EXPECT_EQ( enumExp.GetVal(), TestEnum::BlablaValue );
    EXPECT_EQ( enumDef, TestEnum::BlablaValue );

    enumExp = Convert::String2T< TestEnum >( "Value1" );
    enumDef = Convert::String2T< TestEnum >( "Value1", TestEnum::TE_Total );

    EXPECT_TRUE( enumExp.IsValid() );
    EXPECT_EQ( enumExp.GetVal(), TestEnum::Value1 );
    EXPECT_EQ( enumDef, TestEnum::Value1 );
}

// ***********************
// Empty strings conversion fails.
TEST( LibCore_String2T, Enum_ConvertEmptyString )
{
    Expected< TestEnum > enumExp = Convert::String2T< TestEnum >( "" );
    TestEnum enumDef = Convert::String2T< TestEnum >( "", TestEnum::TE_Total );

    EXPECT_FALSE( enumExp.IsValid() );
    EXPECT_EQ( enumDef, TestEnum::TE_Total );
}

// ***********************
// Valid enum string but with spaces before and after will fail.
TEST( LibCore_String2T, Enum_ConvertEnumWithSpaces )
{
    Expected< TestEnum > enumExp = Convert::String2T< TestEnum >( " BlablaValue " );
    TestEnum enumDef = Convert::String2T< TestEnum >( " BlablaValue ", TestEnum::TE_Total );

    EXPECT_FALSE( enumExp.IsValid() );
    EXPECT_EQ( enumDef, TestEnum::TE_Total );
}

// ***********************
// Check if conversion is inversible. T2String should produce value which can be converted back to typed value.
TEST( LibCore_String2T, Enum_ConversionReversibility )
{
    Expected< TestEnum > enumExp = Convert::String2T< TestEnum >( Convert::T2String( TestEnum::BlablaValue ) );

    EXPECT_TRUE( enumExp.IsValid() );
    EXPECT_EQ( enumExp.GetVal(), TestEnum::BlablaValue );
}



