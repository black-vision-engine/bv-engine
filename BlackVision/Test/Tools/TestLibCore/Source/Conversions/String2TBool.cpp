#include "gtest/gtest.h"

#include "Serialization/SerializationHelper.h"

using namespace bv;



// ***********************
// Check String2T for valid bool inputs.
// Note: we only support bool values written strictly as { true, false }. Test is case sensitive.
TEST( LibCore_String2T, Bool_ValidInput )
{
    Expected< bool > trueExp = Convert::String2T< bool >( "true" );
    bool trueDef = Convert::String2T< bool >( "true", false );

    EXPECT_TRUE( trueExp.IsValid() );
    EXPECT_EQ( trueExp.GetVal(), true );
    EXPECT_EQ( trueDef, true );
    
    Expected< bool > falseExp = Convert::String2T< bool >( "false" );
    bool falseDef = Convert::String2T< bool >( "false", true );

    EXPECT_TRUE( falseExp.IsValid() );
    EXPECT_EQ( falseExp.GetVal(), false );
    EXPECT_EQ( falseDef, false );
}

// ***********************
// Test for invalid input for bool.
TEST( LibCore_String2T, Bool_InvaliInput )
{
    Expected< bool > trueExp = Convert::String2T< bool >( "bllaa" );
    bool trueDef = Convert::String2T< bool >( "bllaa", false );

    EXPECT_FALSE( trueExp.IsValid() );
    EXPECT_EQ( trueDef, false );
}

// ***********************
// Test for invalid input for bool.
TEST( LibCore_String2T, Bool_TrueMixedWithText )
{
    Expected< bool > trueExp = Convert::String2T< bool >( "trueblabla" );
    bool trueDef = Convert::String2T< bool >( "trueblabla", false );

    EXPECT_FALSE( trueExp.IsValid() );
    EXPECT_EQ( trueDef, false );
}

// ***********************
// Test for invalid input for bool.
TEST( LibCore_String2T, Bool_FalseMixedWithText )
{
    Expected< bool > trueExp = Convert::String2T< bool >( "falseblabla" );
    bool trueDef = Convert::String2T< bool >( "falseblabla", true );

    EXPECT_FALSE( trueExp.IsValid() );
    EXPECT_EQ( trueDef, true );
}

// ***********************
// Test for invalid input for bool.
TEST( LibCore_String2T, Bool_TrueAfterText )
{
    Expected< bool > trueExp = Convert::String2T< bool >( "bla true" );
    bool trueDef = Convert::String2T< bool >( "bla true", false );

    EXPECT_FALSE( trueExp.IsValid() );
    EXPECT_EQ( trueDef, false );
}

// ***********************
// Test for invalid input for bool.
TEST( LibCore_String2T, Bool_FalseAfterText )
{
    Expected< bool > trueExp = Convert::String2T< bool >( "bla false" );
    bool trueDef = Convert::String2T< bool >( "bla fals", true );

    EXPECT_FALSE( trueExp.IsValid() );
    EXPECT_EQ( trueDef, true );
}

// ***********************
//
TEST( LibCore_String2T, Bool_True2String )
{
    std::string boolVal = Convert::T2String( true );
    EXPECT_EQ( boolVal, "true" );
}

// ***********************
//
TEST( LibCore_String2T, Bool_False2String )
{
    std::string boolVal = Convert::T2String( false );
    EXPECT_EQ( boolVal, "false" );
}

// ***********************
// Check if conversion is inversible. T2String should produce value which can be converted back to typed value.
TEST( LibCore_String2T, Bool_ConversionReversibility )
{
    Expected< bool > expBool = Convert::String2T< bool >( Convert::T2String( false ) );
    
    EXPECT_TRUE( expBool.IsValid() );
    EXPECT_EQ( expBool.GetVal(), false );

    expBool = Convert::String2T< bool >( Convert::T2String( true ) );

    EXPECT_TRUE( expBool.IsValid() );
    EXPECT_EQ( expBool.GetVal(), true );
}


