#include "gtest/gtest.h"

#include "Engine/Graphics/InputSlots/InputSlots.h"
#include "Engine/Graphics/Resources/Textures/Texture2D.h"

#include "Assets/Input/TextureInputAsset.h"
#include "Assets/Input/TextureInputAssetDesc.h"
#include "Assets/Input/Loaders/InputAssetLoader.h"

#include "Assets/Input/VideoInput/VideoInputAsset.h"
#include "Assets/Input/VideoInput/VideoInputAssetDesc.h"
#include "Assets/Input/VideoInput/VideoInputTextureAsset.h"
#include "Assets/Input/VideoInput/VideoInputTextureAssetDesc.h"
#include "Assets/Input/VideoInput/VideoInputAudioAssetDesc.h"
#include "Assets/Input/VideoInput/VideoInputAudioAsset.h"

#include "Assets/AssetManager.h"


#include "Helpers/TextureHelpers.h"
#include "Helpers/AVFrameHelpers.h"
#include "Helpers/VideoInputAssetHelpers.h"



using namespace bv;


// ***********************
// Create VideoInputTextureAsset bound to Fill texture.
TEST( Engine_InputSlots, VideoInputTextureAsset_Creation_Fill )
{
    auto context = CreateVideoInputContext();

    videocards::VideoInputChannelDesc channel1( 1, 0, "BlueFish", "A", CreateDefaultAVFrame() );
    ASSERT_TRUE( context.videoSlots->RegisterVideoInputChannel( channel1 ) );

    auto desc = VideoInputTextureAssetDesc::Create( 0, videocards::VideoType::Fill );
    auto asset = context.assetManager->LoadAsset( desc );

    ASSERT_NE( asset, nullptr );
    ASSERT_EQ( asset->GetUID(), VideoInputTextureAsset::UID() );

    auto typedAsset = std::static_pointer_cast< const VideoInputTextureAsset >( asset );

    auto slot = context.slots->AccessSource( context.videoSlots->GetSlotIndex( 0 ) );
    ASSERT_TRUE( slot.IsValid() );
    
    EXPECT_EQ( typedAsset->GetTexture(), slot.GetVal().Texture );
}

// ***********************
// Create VideoInputAudioAsset.
TEST( Engine_InputSlots, VideoInputAudioAsset_Creation )
{
    auto context = CreateVideoInputContext();

    videocards::VideoInputChannelDesc channel1( 1, 0, "BlueFish", "A", CreateDefaultAVFrame() );
    ASSERT_TRUE( context.videoSlots->RegisterVideoInputChannel( channel1 ) );

    auto desc = VideoInputAudioAssetDesc::Create( 0 );
    auto asset = context.assetManager->LoadAsset( desc );

    ASSERT_NE( asset, nullptr );
    ASSERT_EQ( asset->GetUID(), VideoInputAudioAsset::UID() );

    auto typedAsset = std::static_pointer_cast< const VideoInputAudioAsset >( asset );

    auto slot = context.slots->AccessSource( context.videoSlots->GetSlotIndex( 0 ) );
    ASSERT_TRUE( slot.IsValid() );

    EXPECT_EQ( typedAsset->GetFrame(), slot.GetVal().Audio->GetData() );
}



