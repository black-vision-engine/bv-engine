#include "gtest/gtest.h"

#include "Engine/Graphics/InputSlots/TextureSlots.h"
#include "Engine/Graphics/Resources/Textures/Texture2D.h"

using namespace bv;


// ========================================================================= //
// Helpers
// ========================================================================= //

// ***********************
//
Texture2DPtr        CreateFakeTexture   ( UInt32 width, UInt32 height )
{
    return std::make_shared< Texture2D >( TextureFormat::F_A8R8G8B8, width, height, DataBuffer::Semantic::S_TEXTURE_DYNAMIC, 1 );
}


// ========================================================================= //
// Tests
// ========================================================================= //



// ***********************
//
TEST( Engine_TextureSlot, RegisterSource )
{
    TextureSlots slots;

    Texture2DPtr tex1 = CreateFakeTexture( 20, 30 );
    Texture2DPtr tex2 = CreateFakeTexture( 50, 50 );

    auto slot1Idx = slots.RegisterSource( tex1, "Source1" );
    auto slot2Idx = slots.RegisterSource( tex2, "Source2" );

    ASSERT_NE( slot1Idx, TextureSlots::sInvalidIdx );
    ASSERT_NE( slot2Idx, TextureSlots::sInvalidIdx );

    EXPECT_EQ( tex1, slots.AccessSource( slot1Idx ) );
    EXPECT_EQ( tex2, slots.AccessSource( slot2Idx ) );
}
