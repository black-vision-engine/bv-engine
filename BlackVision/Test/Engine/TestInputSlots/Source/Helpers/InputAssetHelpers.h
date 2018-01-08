#pragma once

#include "TextureHelpers.h"

#include "Engine/Graphics/InputSlots/InputSlots.h"
#include "Engine/Graphics/Resources/Textures/Texture2D.h"

#include "Assets/Input/TextureInputAsset.h"
#include "Assets/Input/TextureInputAssetDesc.h"
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
inline InputsContext     CreateInputContext   ()
{
    InputsContext context;

    context.slots = std::make_shared< InputSlots >();
    context.assetManager = std::make_shared< AssetManager >();

    context.assetManager->RegisterLoader( TextureInputAssetDesc::UID(), std::make_shared< InputAssetLoader >( context.slots ) );

    return context;
}






}	// bv


