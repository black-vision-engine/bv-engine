#include "gtest/gtest.h"

#include "Serialization/SerializationHelper.h"
#include "Mathematics/glm_inc.h"


// ========================================================================= //
// Test vectors (glm:: vec2, vec3, vec4) conversion. Internal implementation
// uses String2T< float >, so Float tests cover all posible float formats issues.
// ========================================================================= //




using namespace bv;


// ***********************
//
template< class Type >
class LibCore_String2T_Vec : public testing::Test
{
protected:
    LibCore_String2T_Vec() {};
};


typedef testing::Types< glm::vec2, glm::vec3, glm::vec4 > VecTypesList;

TYPED_TEST_CASE( LibCore_String2T_Vec, VecTypesList );



// ***********************
// Basic vec2 conversion test.
TEST( LibCore_String2T, Vec2_ValidInput )
{
    Expected< glm::vec2 > vecExp = SerializationHelper::String2T< glm::vec2 >( "123.05, 321.01" );
    glm::vec2 vecDef = SerializationHelper::String2T< glm::vec2 >( "123.05, 321.01", glm::vec2() );

    EXPECT_TRUE( vecExp.IsValid() );
    EXPECT_EQ( vecExp.GetVal(), glm::vec2( 123.05, 321.01 ) );
    EXPECT_EQ( vecDef, glm::vec2( 123.05, 321.01 ) );
}

// ***********************
// Basic vec3 conversion test.
TEST( LibCore_String2T, Vec3_ValidInput )
{
    Expected< glm::vec3 > vecExp = SerializationHelper::String2T< glm::vec3 >( "123.05, 321.01, 1.0032" );
    glm::vec3 vecDef = SerializationHelper::String2T< glm::vec3 >( "123.05, 321.01, 1.0032", glm::vec3() );

    EXPECT_TRUE( vecExp.IsValid() );
    EXPECT_EQ( vecExp.GetVal(), glm::vec3( 123.05, 321.01, 1.0032 ) );
    EXPECT_EQ( vecDef, glm::vec3( 123.05, 321.01, 1.0032 ) );
}

// ***********************
// Basic vec4 conversion test.
TEST( LibCore_String2T, Vec4_ValidInput )
{
    Expected< glm::vec4 > vecExp = SerializationHelper::String2T< glm::vec4 >( "123.05, 321.01, 1.0032, -15" );
    glm::vec4 vecDef = SerializationHelper::String2T< glm::vec4 >( "123.05, 321.01, 1.0032, -15", glm::vec4() );

    EXPECT_TRUE( vecExp.IsValid() );
    EXPECT_EQ( vecExp.GetVal(), glm::vec4( 123.05, 321.01, 1.0032, -15 ) );
    EXPECT_EQ( vecDef, glm::vec4( 123.05, 321.01, 1.0032, -15 ) );
}


// ***********************
// One component conversion fail, causes whole conversion fail.
TEST( LibCore_String2T, Vec2_OneInvalidComponent )
{
    Expected< glm::vec2 > vecExp = SerializationHelper::String2T< glm::vec2 >( "123.05, bla bla" );
    glm::vec2 vecDef = SerializationHelper::String2T< glm::vec2 >( "123.05, bla bla", glm::vec2() );

    EXPECT_FALSE( vecExp.IsValid() );
    EXPECT_EQ( vecDef, glm::vec2() );
}

// ***********************
// One component conversion fail, causes whole conversion fail.
TEST( LibCore_String2T, Vec3_OneInvalidComponent )
{
    Expected< glm::vec3 > vecExp = SerializationHelper::String2T< glm::vec3 >( "123.05, bla bla, 1.0032" );
    glm::vec3 vecDef = SerializationHelper::String2T< glm::vec3 >( "123.05, bla bla, 1.0032", glm::vec3() );

    EXPECT_FALSE( vecExp.IsValid() );
    EXPECT_EQ( vecDef, glm::vec3() );
}

// ***********************
// One component conversion fail, causes whole conversion fail.
TEST( LibCore_String2T, Vec4_OneInvalidComponent )
{
    Expected< glm::vec4 > vecExp = SerializationHelper::String2T< glm::vec4 >( "123.05, 321.01, bla bla, -15" );
    glm::vec4 vecDef = SerializationHelper::String2T< glm::vec4 >( "123.05, 321.01, bla bla, -15", glm::vec4() );

    EXPECT_FALSE( vecExp.IsValid() );
    EXPECT_EQ( vecDef, glm::vec4() );
}


// ========================================================================= //
// Generic vector tests
// ========================================================================= //

// ***********************
//
template< typename ParamType >
ParamType           GetTestValue    ()
{    return ParamType();    }

// ***********************
//
template<> glm::vec2    GetTestValue< glm::vec2 >    ()  { return glm::vec2( 123.05, 321.01 ); }
template<> glm::vec3    GetTestValue< glm::vec3 >    ()  { return glm::vec3( 123.05, 321.01, 1.0032 ); }
template<> glm::vec4    GetTestValue< glm::vec4 >    ()  { return glm::vec4( 123.05, 321.01, 1.0032, -15 ); }


// ***********************
// To many vector components for type.
TYPED_TEST( LibCore_String2T_Vec, ToManyComponents )
{
    Expected< TypeParam > vecExp = SerializationHelper::String2T< TypeParam >( "123.05, 321.01, 1.0032, -15, -12" );
    TypeParam vecDef = SerializationHelper::String2T< TypeParam >( "123.05, 321.01, 1.0032, -15, -12", TypeParam() );

    EXPECT_FALSE( vecExp.IsValid() );
    EXPECT_EQ( vecDef, TypeParam() );
}

// ***********************
// To few vector components for type.
TYPED_TEST( LibCore_String2T_Vec, ToFewComponents )
{
    Expected< TypeParam > vecExp = SerializationHelper::String2T< TypeParam >( "123.05" );
    TypeParam vecDef = SerializationHelper::String2T< TypeParam >( "123.05", TypeParam() );

    EXPECT_FALSE( vecExp.IsValid() );
    EXPECT_EQ( vecDef, TypeParam() );
}

// ***********************
// Check if conversion is inversible. T2String should produce value which can be converted back to typed value.
TYPED_TEST( LibCore_String2T_Vec, ConversionReversibility )
{
    Expected< TypeParam > vecExp = SerializationHelper::String2T< TypeParam >( SerializationHelper::T2String( GetTestValue< TypeParam >() ) );

    EXPECT_TRUE( vecExp.IsValid() );
    EXPECT_EQ( vecExp.GetVal(), GetTestValue< TypeParam >() );
}
