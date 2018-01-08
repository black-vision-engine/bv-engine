#include "gtest/gtest.h"

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
TEST( Engine_InputSlots, Asset_Creation_BindByIndex )
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
TEST( Engine_InputSlots, Asset_Creation_BindByName )
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


