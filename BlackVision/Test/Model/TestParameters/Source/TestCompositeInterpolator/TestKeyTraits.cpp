#include "gtest/gtest.h"

#include "Mathematics/Interpolators/KeyTraits.h"


using namespace bv;


// ========================================================================= //
// These tests check key traits behavior. Since there's almost no logic in 
// key traits, tests should only warn you if you changed something important.
// ========================================================================= //


// ***********************
// Float

// ***********************
//
TEST( Model_ParamValModel, Keys_Float_ValidKey )
{
    EXPECT_TRUE( IsValidKey< float >( 0.0f, 1.0f ) );
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

// ***********************
// Double

// ***********************
//
TEST( Model_ParamValModel, Keys_Double_ValidKey )
{
    EXPECT_TRUE( IsValidKey< double >( 0.0, 1.0 ) );
}

// ***********************
//
TEST( Model_ParamValModel, Keys_Double_TimeNaN )
{
    EXPECT_FALSE( IsValidKey< double >( std::numeric_limits< float >::quiet_NaN(), 1.0 ) );
}

// ***********************
//
TEST( Model_ParamValModel, Keys_Double_TimeInifinity )
{
    EXPECT_FALSE( IsValidKey< double >( std::numeric_limits< float >::infinity(), 1.0 ) );
}

// ***********************
//
TEST( Model_ParamValModel, Keys_Double_ValueNaN )
{
    EXPECT_FALSE( IsValidKey< double >( 1.0, std::numeric_limits< double >::quiet_NaN() ) );
}

// ***********************
//
TEST( Model_ParamValModel, Keys_Double_ValueInfinity )
{
    EXPECT_FALSE( IsValidKey< double >( 1.0, std::numeric_limits< double >::infinity() ) );
}

// ***********************
// Int

// ***********************
//
TEST( Model_ParamValModel, Keys_Int_ValidKey )
{
    EXPECT_TRUE( IsValidKey< int >( 0.0, 3 ) );
}

// ***********************
//
TEST( Model_ParamValModel, Keys_Int_TimeNaN )
{
    EXPECT_FALSE( IsValidKey< int >( std::numeric_limits< float >::quiet_NaN(), 2 ) );
}

// ***********************
//
TEST( Model_ParamValModel, Keys_Int_TimeInifinity )
{
    EXPECT_FALSE( IsValidKey< int >( std::numeric_limits< float >::infinity(), 3 ) );
}

// ***********************
// string

// ***********************
//
TEST( Model_ParamValModel, Keys_String_ValidKey )
{
    EXPECT_TRUE( IsValidKey< std::string >( 0.0, std::string( "" ) ) );
}

// ***********************
//
TEST( Model_ParamValModel, Keys_String_TimeNaN )
{
    EXPECT_FALSE( IsValidKey< std::string >( std::numeric_limits< float >::quiet_NaN(), std::string( "" ) ) );
}

// ***********************
//
TEST( Model_ParamValModel, Keys_String_TimeInifinity )
{
    EXPECT_FALSE( IsValidKey< std::string >( std::numeric_limits< float >::infinity(), std::string( "" ) ) );
}

// ***********************
// vec2

// ***********************
//
TEST( Model_ParamValModel, Keys_Vec2_ValidKey )
{
    EXPECT_TRUE( IsValidKey< glm::vec2 >( 0.0f, glm::vec2() ) );
}

// ***********************
//
TEST( Model_ParamValModel, Keys_Vec2_TimeNaN )
{
    EXPECT_FALSE( IsValidKey< glm::vec2 >( std::numeric_limits< float >::quiet_NaN(), glm::vec2() ) );
}

// ***********************
//
TEST( Model_ParamValModel, Keys_Vec2_TimeInifinity )
{
    EXPECT_FALSE( IsValidKey< glm::vec2 >( std::numeric_limits< float >::infinity(), glm::vec2() ) );
}

// ***********************
//
TEST( Model_ParamValModel, Keys_Vec2_ValueNaN )
{
    EXPECT_FALSE( IsValidKey< glm::vec2 >( 1.0f, glm::vec2( std::numeric_limits< float >::quiet_NaN(), 1.0 ) ) );
}

// ***********************
//
TEST( Model_ParamValModel, Keys_Vec2_ValueInfinity )
{
    EXPECT_FALSE( IsValidKey< glm::vec2 >( 1.0f, glm::vec2( std::numeric_limits< float >::infinity(), 2.0f ) ) );
}


// ***********************
// vec3

// ***********************
//
TEST( Model_ParamValModel, Keys_Vec3_ValidKey )
{
    EXPECT_TRUE( IsValidKey< glm::vec3 >( 0.0f, glm::vec3() ) );
}

// ***********************
//
TEST( Model_ParamValModel, Keys_Vec3_TimeNaN )
{
    EXPECT_FALSE( IsValidKey< glm::vec3 >( std::numeric_limits< float >::quiet_NaN(), glm::vec3() ) );
}

// ***********************
//
TEST( Model_ParamValModel, Keys_Vec3_TimeInifinity )
{
    EXPECT_FALSE( IsValidKey< glm::vec3 >( std::numeric_limits< float >::infinity(), glm::vec3() ) );
}

// ***********************
//
TEST( Model_ParamValModel, Keys_Vec3_ValueNaN )
{
    EXPECT_FALSE( IsValidKey< glm::vec3 >( 1.0f, glm::vec3( std::numeric_limits< float >::quiet_NaN(), 1.0f, 2.0f ) ) );
}

// ***********************
//
TEST( Model_ParamValModel, Keys_Vec3_ValueInfinity )
{
    EXPECT_FALSE( IsValidKey< glm::vec3 >( 1.0f, glm::vec3( std::numeric_limits< float >::infinity(), 2.0f, 1.0f ) ) );
}

// ***********************
// vec4

// ***********************
//
TEST( Model_ParamValModel, Keys_Vec4_ValidKey )
{
    EXPECT_TRUE( IsValidKey< glm::vec4 >( 0.0f, glm::vec4() ) );
}

// ***********************
//
TEST( Model_ParamValModel, Keys_Vec4_TimeNaN )
{
    EXPECT_FALSE( IsValidKey< glm::vec4 >( std::numeric_limits< float >::quiet_NaN(), glm::vec4() ) );
}

// ***********************
//
TEST( Model_ParamValModel, Keys_Vec4_TimeInifinity )
{
    EXPECT_FALSE( IsValidKey< glm::vec4 >( std::numeric_limits< float >::infinity(), glm::vec4() ) );
}

// ***********************
//
TEST( Model_ParamValModel, Keys_Vec4_ValueNaN )
{
    EXPECT_FALSE( IsValidKey< glm::vec4 >( 1.0f, glm::vec4( std::numeric_limits< float >::quiet_NaN(), 1.0f, 2.0f, 1.0f ) ) );
}

// ***********************
//
TEST( Model_ParamValModel, Keys_Vec4_ValueInfinity )
{
    EXPECT_FALSE( IsValidKey< glm::vec4 >( 1.0f, glm::vec4( std::numeric_limits< float >::infinity(), 2.0f, 1.0f, 1.0f ) ) );
}


