#include "gtest/gtest.h"

#include "Engine/Graphics/InputSlots/InputSlots.h"
#include "Engine/Graphics/Resources/Textures/Texture2D.h"
#include "Engine/Audio/Resources/AudioBuffer.h"

#include "Helpers/TextureHelpers.h"

using namespace bv;




// ========================================================================= //
// Tests
// ========================================================================= //



// ***********************
//
TEST( Engine_InputSlots, RegisterSource )
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
TEST( Engine_InputSlots, RegisterSourceWithSameName )
{
    InputSlots slots;

    Texture2DPtr tex1 = CreateFakeTexture( 20, 30 );
    Texture2DPtr tex2 = CreateFakeTexture( 50, 50 );

    ASSERT_TRUE( slots.RegisterSource( InputSlot( tex1 ), "Source1" ).IsValid() );
    ASSERT_FALSE( slots.RegisterSource( InputSlot( tex2 ), "Source1" ).IsValid() );
}

// ***********************
// Registration fails if someone passes nullptr
TEST( Engine_InputSlots, RegisterNullTexture )
{
    InputSlots slots;
    ASSERT_FALSE( slots.RegisterSource( InputSlot(), "Source1" ).IsValid() );
}

// ***********************
//
TEST( Engine_InputSlots, AccessSourceByName )
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
TEST( Engine_InputSlots, UnregisterSourceByIdx )
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
    ASSERT_FALSE( slots.AccessSource( slot1Idx ).IsValid() );

    ASSERT_TRUE( slots.UnregisterSource( slot2Idx ) );
    ASSERT_FALSE( slots.AccessSource( slot2Idx ).IsValid() );
}

// ***********************
//
TEST( Engine_InputSlots, UnregisterSourceByName )
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
    ASSERT_FALSE( slots.AccessSource( slot1Idx ).IsValid() );

    ASSERT_TRUE( slots.UnregisterSource( "Source2" ) );
    ASSERT_FALSE( slots.AccessSource( slot2Idx ).IsValid() );
}

// ***********************
// If we pass non existing name to Unregister function, it should return false.
TEST( Engine_InputSlots, UnregisterSourceInvalidName )
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
TEST( Engine_InputSlots, UnregisterSourceInvalidIdx )
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


// ***********************
//
TEST( Engine_InputSlots, RegisterSource_Audio )
{
    InputSlots slots;

    Texture2DPtr tex1 = CreateFakeTexture( 20, 30 );
    audio::AudioBufferPtr audio1 = audio::AudioBuffer::Create( MemoryChunk::Create( 3840 ), 48000, AudioFormat::STEREO16, false );

    Expected< SlotIndex > slot1Idx = slots.RegisterSource( InputSlot( tex1, audio1 ), "Source1" );

    ASSERT_TRUE( slot1Idx.IsValid() );
    ASSERT_TRUE( slots.AccessSource( slot1Idx ).IsValid() );

    EXPECT_EQ( tex1, slots.AccessSource( slot1Idx ).GetVal().Texture );
    EXPECT_EQ( audio1, slots.AccessSource( slot1Idx ).GetVal().Audio );
}
