#include "gtest/gtest.h"

#include "Serialization/SerializationHelper.h"
#include "Mathematics/glm_inc.h"



using namespace bv;




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

