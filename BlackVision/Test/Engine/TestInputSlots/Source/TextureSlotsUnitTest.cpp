#include "gtest/gtest.h"

#include "Engine/Graphics/InputSlots/InputSlots.h"
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
    InputSlots slots;

    Texture2DPtr tex1 = CreateFakeTexture( 20, 30 );
    Texture2DPtr tex2 = CreateFakeTexture( 50, 50 );

    Expected< SlotIndex > slot1Idx = slots.RegisterSource( InputSlot( tex1 ), "Source1" );
    Expected< SlotIndex > slot2Idx = slots.RegisterSource( InputSlot( tex2 ), "Source2" );

    ASSERT_TRUE( slot1Idx.IsValid() );
    ASSERT_TRUE( slot2Idx.IsValid() );

    ASSERT_TRUE( slots.AccessSource( slot1Idx ).IsValid() );
    ASSERT_TRUE( slots.AccessSource( slot2Idx ).IsValid() );

    EXPECT_EQ( tex1, slots.AccessSource( slot1Idx ).GetVal().Texture );
    EXPECT_EQ( tex2, slots.AccessSource( slot2Idx ).GetVal().Texture );
    EXPECT_EQ( nullptr, slots.AccessSource( slot1Idx ).GetVal().Audio );
    EXPECT_EQ( nullptr, slots.AccessSource( slot2Idx ).GetVal().Audio );
}

// ***********************
// Registration fails if someone tries to register source with the same name.
TEST( Engine_TextureSlot, RegisterSourceWithSameName )
{
    InputSlots slots;

    Texture2DPtr tex1 = CreateFakeTexture( 20, 30 );
    Texture2DPtr tex2 = CreateFakeTexture( 50, 50 );

    ASSERT_TRUE( slots.RegisterSource( InputSlot( tex1 ), "Source1" ).IsValid() );
    ASSERT_FALSE( slots.RegisterSource( InputSlot( tex2 ), "Source1" ).IsValid() );
}

// ***********************
// Registration fails if someone passes nullptr
TEST( Engine_TextureSlot, RegisterNullTexture )
{
    InputSlots slots;
    ASSERT_FALSE( slots.RegisterSource( InputSlot(), "Source1" ).IsValid() );
}

// ***********************
//
TEST( Engine_TextureSlot, AccessSourceByName )
{
    InputSlots slots;

    Texture2DPtr tex1 = CreateFakeTexture( 20, 30 );
    Texture2DPtr tex2 = CreateFakeTexture( 50, 50 );

    ASSERT_TRUE( slots.RegisterSource( InputSlot( tex1 ), "Source1" ).IsValid() );
    ASSERT_TRUE( slots.RegisterSource( InputSlot( tex2 ), "Source2" ).IsValid() );

    ASSERT_TRUE( slots.AccessSource( "Source1" ).IsValid() );
    ASSERT_TRUE( slots.AccessSource( "Source2" ).IsValid() );

    EXPECT_EQ( tex1, slots.AccessSource( "Source1" ).GetVal().Texture );
    EXPECT_EQ( tex2, slots.AccessSource( "Source2" ).GetVal().Texture );
}

// ***********************
//
TEST( Engine_TextureSlot, UnregisterSourceByIdx )
{
    InputSlots slots;

    Texture2DPtr tex1 = CreateFakeTexture( 20, 30 );
    Texture2DPtr tex2 = CreateFakeTexture( 50, 50 );

    Expected< SlotIndex > slot1Idx = slots.RegisterSource( InputSlot( tex1 ), "Source1" );
    Expected< SlotIndex > slot2Idx = slots.RegisterSource( InputSlot( tex2 ), "Source2" );

    ASSERT_TRUE( slot1Idx.IsValid() );
    ASSERT_TRUE( slot2Idx.IsValid() );

    ASSERT_TRUE( slots.AccessSource( slot1Idx ).IsValid() );
    ASSERT_TRUE( slots.AccessSource( slot2Idx ).IsValid() );

    ASSERT_EQ( tex1, slots.AccessSource( slot1Idx ).GetVal().Texture );
    ASSERT_EQ( tex2, slots.AccessSource( slot2Idx ).GetVal().Texture );

    ASSERT_TRUE( slots.UnregisterSource( slot1Idx ) );
    ASSERT_TRUE( slots.AccessSource( slot1Idx ).IsValid() );
    ASSERT_EQ( slots.AccessSource( slot1Idx ).GetVal().Texture, nullptr );

    ASSERT_TRUE( slots.UnregisterSource( slot2Idx ) );
    ASSERT_TRUE( slots.AccessSource( slot2Idx ).IsValid() );
    ASSERT_EQ( slots.AccessSource( slot2Idx ).GetVal().Texture, nullptr );
}

// ***********************
//
TEST( Engine_TextureSlot, UnregisterSourceByName )
{
    InputSlots slots;

    Texture2DPtr tex1 = CreateFakeTexture( 20, 30 );
    Texture2DPtr tex2 = CreateFakeTexture( 50, 50 );

    Expected< SlotIndex > slot1Idx = slots.RegisterSource( InputSlot( tex1 ), "Source1" );
    Expected< SlotIndex > slot2Idx = slots.RegisterSource( InputSlot( tex2 ), "Source2" );

    ASSERT_TRUE( slot1Idx.IsValid() );
    ASSERT_TRUE( slot2Idx.IsValid() );

    ASSERT_TRUE( slots.AccessSource( slot1Idx ).IsValid() );
    ASSERT_TRUE( slots.AccessSource( slot2Idx ).IsValid() );

    ASSERT_EQ( tex1, slots.AccessSource( slot1Idx ).GetVal().Texture );
    ASSERT_EQ( tex2, slots.AccessSource( slot2Idx ).GetVal().Texture );

    ASSERT_TRUE( slots.UnregisterSource( "Source1" ) );
    ASSERT_TRUE( slots.AccessSource( slot1Idx ).IsValid() );
    ASSERT_EQ( slots.AccessSource( slot1Idx ).GetVal().Texture, nullptr );

    ASSERT_TRUE( slots.UnregisterSource( "Source2" ) );
    ASSERT_TRUE( slots.AccessSource( slot2Idx ).IsValid() );
    ASSERT_EQ( slots.AccessSource( slot2Idx ).GetVal().Texture, nullptr );
}

// ***********************
// If we pass non existing name to Unregister function, it should return false.
TEST( Engine_TextureSlot, UnregisterSourceInvalidName )
{
    InputSlots slots;

    Texture2DPtr tex1 = CreateFakeTexture( 20, 30 );
    Texture2DPtr tex2 = CreateFakeTexture( 50, 50 );

    ASSERT_TRUE( slots.RegisterSource( InputSlot( tex1 ), "Source1" ).IsValid() );
    ASSERT_TRUE( slots.RegisterSource( InputSlot( tex2 ), "Source2" ).IsValid() );

    ASSERT_FALSE( slots.UnregisterSource( "bla bla" ) );
    ASSERT_TRUE( slots.AccessSource( "Source1" ).IsValid() );
    ASSERT_TRUE( slots.AccessSource( "Source2" ).IsValid() );
    ASSERT_NE( slots.AccessSource( "Source1" ).GetVal().Texture, nullptr );
    ASSERT_NE( slots.AccessSource( "Source2" ).GetVal().Texture, nullptr );
}

// ***********************
// If we pass non existing slotIndex to Unregister function, it should return false.
TEST( Engine_TextureSlot, UnregisterSourceInvalidIdx )
{
    InputSlots slots;

    Texture2DPtr tex1 = CreateFakeTexture( 20, 30 );
    Texture2DPtr tex2 = CreateFakeTexture( 50, 50 );

    ASSERT_TRUE( slots.RegisterSource( InputSlot( tex1 ), "Source1" ).IsValid() );
    ASSERT_TRUE( slots.RegisterSource( InputSlot( tex2 ), "Source2" ).IsValid() );

    ASSERT_FALSE( slots.UnregisterSource( 500 ) );
    ASSERT_TRUE( slots.AccessSource( "Source1" ).IsValid() );
    ASSERT_TRUE( slots.AccessSource( "Source2" ).IsValid() );
    ASSERT_NE( slots.AccessSource( "Source1" ).GetVal().Texture, nullptr );
    ASSERT_NE( slots.AccessSource( "Source2" ).GetVal().Texture, nullptr );
}
