#include "gtest/gtest.h"

#include "Engine/Graphics/InputSlots/InputSlots.h"
#include "Engine/Graphics/Resources/Textures/Texture2D.h"

#include "Engine/Models/Plugins/Channels/PixelShader/GPUTextureDescriptor.h"

#include "Assets/Input/TextureInputAsset.h"
#include "Assets/Input/TextureInputAssetDesc.h"
#include "Assets/Input/Loaders/InputAssetLoader.h"

#include "Assets/AssetManager.h"

#include "Helpers/TextureHelpers.h"
#include "Helpers/InputAssetHelpers.h"


using namespace bv;



// ***********************
// GetUID function returns TextureInputAsset pointer. Some parts of code depend on this.
TEST( Engine_InputSlots, GPUTextureDescriptor_GetUIDReturnsAsset )
{
    auto context = CreateInputContext();
    auto asset = CreateSlotAndAsset( context );

    model::GPUTextureDescriptor gpuTexDesc( asset, "SlotTex0" );

    EXPECT_EQ( gpuTexDesc.GetUID(), (uintptr_t)asset.get() );
}



