#include "gtest/gtest.h"
#include "Memory/ColorRGBA.h"



using namespace bv;



// ***********************
// Test constructor with int RGBA values.
TEST( LibCore_Memory, ColorRGBA_Constructor_IntRGBA )
{
    ColorRGBA color( 34, 25, 16, 253 );

    EXPECT_EQ( color.R(), 34 );
    EXPECT_EQ( color.G(), 25 );
    EXPECT_EQ( color.B(), 16 );
    EXPECT_EQ( color.A(), 253 );
}

// ***********************
// Test constructor with glm::vec4 values in [0,1] range.
TEST( LibCore_Memory, ColorRGBA_Constructor_glmVec )
{
    ColorRGBA color( glm::vec4( 0.5, 0.2, 0.6, 0.9 ) );

    EXPECT_EQ( color.R(), UInt32( 0.5 * 255.0f ) );
    EXPECT_EQ( color.G(), UInt32( 0.2 * 255.0f ) );
    EXPECT_EQ( color.B(), UInt32( 0.6 * 255.0f ) );
    EXPECT_EQ( color.A(), UInt32( 0.9 * 255.0f ) );
}

// ***********************
//
TEST( LibCore_Memory, ColorRGBA_GetColorInt )
{
    ColorRGBA color( 34, 25, 16, 253 );

    auto intColor = color.GetColorInt();

    EXPECT_EQ( color.R(), intColor.r );
    EXPECT_EQ( color.G(), intColor.g );
    EXPECT_EQ( color.B(), intColor.b );
    EXPECT_EQ( color.A(), intColor.a );
}

// ***********************
//
TEST( LibCore_Memory, ColorRGBA_ColorSetters )
{
    ColorRGBA color( 34, 25, 16, 253 );

    color.SetR( 120 );
    color.SetG( 230 );
    color.SetB( 1 );
    color.SetA( 56 );

    EXPECT_EQ( color.R(), 120 );
    EXPECT_EQ( color.G(), 230 );
    EXPECT_EQ( color.B(), 1 );
    EXPECT_EQ( color.A(), 56 );
}

// ***********************
// ColorRGBA class should clamp values if they exceed 255.
TEST( LibCore_Memory, ColorRGBA_Clamp )
{
    ColorRGBA color( 34, 25, 16, 253 );

    color.SetR( 1200 );
    color.SetG( 2300 );
    color.SetB( 1000 );
    color.SetA( 5600 );

    EXPECT_EQ( color.R(), 255 );
    EXPECT_EQ( color.G(), 255 );
    EXPECT_EQ( color.B(), 255 );
    EXPECT_EQ( color.A(), 255 );
}

// ***********************
// Tests loading colors from raw memory.
TEST( LibCore_Memory, ColorRGBA_LoadFromMemory )
{
    UInt32 intColor = 0xAABBCCDD;
    char * memory = new char[ 100 ];

    memcpy( memory, &intColor, 4 );

    ColorRGBA actual = ColorRGBA::Load( memory );
    ColorRGBA expected = ColorRGBA( 0xDD, 0xCC, 0xBB, 0xAA );

    EXPECT_EQ( actual.GetColorInt(), expected.GetColorInt() );

    delete[] memory;
}

// ***********************
// Tests saving color to memory and loading it back.
TEST( LibCore_Memory, ColorRGBA_SaveAndLoad )
{
    char * memory = new char[ 100 ];

    ColorRGBA expected = ColorRGBA( 24, 34, 54, 64 );
    expected.Save( memory );

    ColorRGBA actual = ColorRGBA::Load( memory );
   
    EXPECT_EQ( actual.GetColorInt(), expected.GetColorInt() );

    delete[] memory;
}

// ***********************
// Operator add test. Colors should be clamped.
TEST( LibCore_Memory, ColorRGBA_OperatorAdd )
{
    ColorRGBA color1 = ColorRGBA( 24, 34, 104, 64 );
    ColorRGBA color2 = ColorRGBA( 200, 200, 250, 64 );

    auto newColor = color1 + color2;

    EXPECT_EQ( newColor.R(), 224 );
    EXPECT_EQ( newColor.G(), 234 );
    EXPECT_EQ( newColor.B(), 255 );
    EXPECT_EQ( newColor.A(), 128 );
}

// ***********************
// Operator devide test.
TEST( LibCore_Memory, ColorRGBA_OperatorDevide )
{
    ColorRGBA color1 = ColorRGBA( 200, 150, 250, 64 );

    auto newColor = color1 / 2;;

    EXPECT_EQ( newColor.R(), 100 );
    EXPECT_EQ( newColor.G(), 75 );
    EXPECT_EQ( newColor.B(), 125 );
    EXPECT_EQ( newColor.A(), 32 );
}
