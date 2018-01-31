#include "gtest/gtest.h"
#include "Framework/FrameworkTest.h"

#include "Engine/Graphics/InputSlots/InputSlots.h"
#include "Engine/Graphics/Resources/Textures/Texture2D.h"

#include "Assets/Input/TextureInputAsset.h"
#include "Assets/Input/TextureInputAssetDesc.h"
#include "Assets/Input/Loaders/InputAssetLoader.h"

#include "Assets/AssetManager.h"

#include "Helpers/TextureHelpers.h"
#include "Helpers/InputAssetHelpers.h"



using namespace bv;



// ***********************
// Create TextureInputAsset with binding by slot index.
TEST( Engine_InputSlots, TextureInputAsset_Creation_BindByIndex )
{
    auto context = CreateInputContext();

    Texture2DPtr tex1 = CreateFakeTexture( 20, 30 );
    Expected< SlotIndex > slot1Idx = context.slots->RegisterSource( InputSlot( tex1 ), "Source1" );

    ASSERT_TRUE( slot1Idx.IsValid() );

    auto desc = TextureInputAssetDesc::Create( InputSlotBinding( slot1Idx ) );
    auto asset = context.assetManager->LoadAsset( desc );

    ASSERT_NE( asset, nullptr );
    ASSERT_EQ( asset->GetUID(), TextureInputAsset::UID() );

    auto typedAsset = std::static_pointer_cast< const TextureInputAsset >( asset );

    EXPECT_EQ( typedAsset->GetTexture(), tex1 );
}

// ***********************
// Create TextureInputAsset with binding by slot name.
TEST( Engine_InputSlots, TextureInputAsset_Creation_BindByName )
{
    auto context = CreateInputContext();

    Texture2DPtr tex1 = CreateFakeTexture( 20, 30 );
    Expected< SlotIndex > slot1Idx = context.slots->RegisterSource( InputSlot( tex1 ), "Source1" );

    ASSERT_TRUE( slot1Idx.IsValid() );

    auto desc = TextureInputAssetDesc::Create( InputSlotBinding( "Source1" ) );
    auto asset = context.assetManager->LoadAsset( desc );

    ASSERT_NE( asset, nullptr );
    ASSERT_EQ( asset->GetUID(), TextureInputAsset::UID() );

    auto typedAsset = std::static_pointer_cast< const TextureInputAsset >( asset );

    EXPECT_EQ( typedAsset->GetTexture(), tex1 );
}

// ***********************
// Create TextureInputAsset providing index to empty slot. Asset should return fallback texture
// generated by InputSlot object.
TEST( Engine_InputSlots, TextureInputAsset_Creation_EmptySlot )
{
    auto context = CreateInputContext();

    // InputSlot should return valid fallback slot. Input slot 30 doesn't exist.
    ASSERT_NE( context.slots->GetFallbackSlot().Texture, nullptr );
    ASSERT_FALSE( context.slots->AccessSource( 30 ).IsValid() );

    // Not existing input slot.
    auto desc = TextureInputAssetDesc::Create( InputSlotBinding( 30 ) );
    auto asset = context.assetManager->LoadAsset( desc );

    // Asset should be created even if slot is empty.
    ASSERT_NE( asset, nullptr );
    ASSERT_EQ( asset->GetUID(), TextureInputAsset::UID() );

    auto typedAsset = std::static_pointer_cast< const TextureInputAsset >( asset );

    ASSERT_EQ( typedAsset->GetTexture(), context.slots->GetFallbackSlot().Texture );
}

// ***********************
// If slot was removed, asset should load fallback texture. UpdateID should change.
POST_RENDER_FRAMEWORK_TEST( Engine_InputSlots, TextureInputAsset_SlotRemoved )
{
    static auto context = CreateInputContext();
    static UInt64 lastUpdateID = 0;

    if( GetFrameNumber() == 0 )
    {
        Expected< SlotIndex > slot1Idx = CreateSlot( context, "Source1" );
        auto asset = LoadTextureAsset( context, "Source1" );                // Load asset internally. Second call will load the same asset.

        lastUpdateID = asset->GetUpdateID();

        // This should send event.
        context.slots->UnregisterSource( slot1Idx );

        EndTestAfterThisFrame( false );
    }
    else
    {
        auto asset = LoadTextureAsset( context, "Source1" );
        EXPECT_EQ( asset->GetTexture(), context.slots->GetFallbackSlot().Texture );
        EXPECT_GT( asset->GetUpdateID(), lastUpdateID );

        EndTestAfterThisFrame( true );
    }
}

// ***********************
// Slot is removed and then new slot is added under the same index.
// Asset should update itself and load texture from new slot. UpdateID should change.
POST_RENDER_FRAMEWORK_TEST( Engine_InputSlots, TextureInputAsset_SlotChanged )
{
    static auto context = CreateInputContext();
    static Expected< SlotIndex > slotIdx;
    static Texture2DPtr tex = CreateFakeTexture( 20, 30 );
    static UInt64 lastUpdateID = 0;

    if( GetFrameNumber() == 0 )
    {
        slotIdx = CreateSlot( context, "Source1" );
        auto asset = LoadTextureAsset( context, slotIdx );  // Load asset internally. Second call will load the same asset.

        lastUpdateID = asset->GetUpdateID();

        // This should 2 send event.
        context.slots->UnregisterSource( slotIdx );
        ASSERT_EQ( context.slots->RegisterSource( InputSlot( tex ), "Source2" ).GetVal(), slotIdx.GetVal() );   // This should register source under the same indexs.

        EndTestAfterThisFrame( false );
    }
    else
    {
        auto asset = LoadTextureAsset( context, slotIdx );
        EXPECT_EQ( asset->GetTexture(), tex );
        EXPECT_GT( asset->GetUpdateID(), lastUpdateID );

        EndTestAfterThisFrame( true );
    }
}
