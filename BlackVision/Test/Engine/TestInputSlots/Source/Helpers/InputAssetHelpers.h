#pragma once

#include "TextureHelpers.h"

#include "Engine/Graphics/InputSlots/InputSlots.h"
#include "Engine/Graphics/Resources/Textures/Texture2D.h"

#include "Assets/Input/TextureInputAsset.h"
#include "Assets/Input/TextureInputAssetDesc.h"
#include "Assets/Input/AudioInputAssetDesc.h"
#include "Assets/Input/Loaders/InputAssetLoader.h"

#include "Assets/AssetManager.h"


namespace bv
{

DEFINE_PTR_TYPE( AssetManager )


// ***********************
//
struct InputsContext
{
    AssetManagerPtr     assetManager;
    InputSlotsPtr       slots;
};



// ***********************
//
inline InputsContext                CreateInputContext   ()
{
    InputsContext context;

    context.slots = std::make_shared< InputSlots >();
    context.assetManager = std::make_shared< AssetManager >();

    context.assetManager->RegisterLoader( TextureInputAssetDesc::UID(), std::make_shared< InputAssetLoader >( context.slots ) );
    context.assetManager->RegisterLoader( AudioInputAssetDesc::UID(), std::make_shared< InputAssetLoader >( context.slots ) );

    return context;
}

// ***********************
//
inline void                         ClearContext        ( InputsContext & ctx )
{
    ctx.assetManager = nullptr;
    ctx.slots = nullptr;
}

// ***********************
//
inline Expected< SlotIndex >        CreateSlot              ( const InputsContext & ctx, const std::string & name )
{
    Texture2DPtr tex1 = CreateFakeTexture( 20, 30 );
    Expected< SlotIndex > slot1Idx = ctx.slots->RegisterSource( InputSlot( tex1 ), name );

    EXPECT_TRUE( slot1Idx.IsValid() );

    return slot1Idx;
}

// ***********************
//
inline TextureInputAssetConstPtr    LoadTextureAsset        ( const InputsContext & ctx, const std::string & name )
{
    auto desc = TextureInputAssetDesc::Create( InputSlotBinding( name ) );
    auto asset = ctx.assetManager->LoadAsset( desc );

    return std::static_pointer_cast< const TextureInputAsset >( asset );
}

// ***********************
//
inline TextureInputAssetConstPtr    LoadTextureAsset        ( const InputsContext & ctx, SlotIndex slotIdx )
{
    auto desc = TextureInputAssetDesc::Create( InputSlotBinding( slotIdx ) );
    auto asset = ctx.assetManager->LoadAsset( desc );

    return std::static_pointer_cast< const TextureInputAsset >( asset );
}

// ***********************
//
inline TextureInputAssetConstPtr    CreateSlotAndAsset      ( const InputsContext & ctx )
{
    Expected< SlotIndex > slot1Idx = CreateSlot( ctx, "Source1" );
    return LoadTextureAsset( ctx, slot1Idx );
}



}	// bv


