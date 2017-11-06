#include "gtest/gtest.h"

#include "Mathematics/Interpolators/KeyTraits.h"


using namespace bv;


// ========================================================================= //
// These tests check key traits behavior. Since there's almost no logic in 
// key traits, tests should only warn you if you changed something important.
// ========================================================================= //


// ***********************
//
TEST( Model_ParamValModel, Keys_Float_ValidKey )
{
    EXPECT_FALSE( IsValidKey< float >( 0.0f, 1.0f ) );
}

// ***********************
//
TEST( Model_ParamValModel, Keys_Float_TimeNaN )
{
    EXPECT_FALSE( IsValidKey< float >( std::numeric_limits< float >::quiet_NaN(), 1.0f ) );
}

// ***********************
//
TEST( Model_ParamValModel, Keys_Float_TimeInifinity )
{
    EXPECT_FALSE( IsValidKey< float >( std::numeric_limits< float >::infinity(), 1.0f ) );
}

// ***********************
//
TEST( Model_ParamValModel, Keys_Float_ValueNaN )
{
    EXPECT_FALSE( IsValidKey< float >( 1.0f, std::numeric_limits< float >::quiet_NaN() ) );
}

// ***********************
//
TEST( Model_ParamValModel, Keys_Float_ValueInfinity )
{
    EXPECT_FALSE( IsValidKey< float >( 1.0f, std::numeric_limits< float >::infinity() ) );
}
