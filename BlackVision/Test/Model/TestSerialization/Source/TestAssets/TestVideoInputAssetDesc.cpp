#include "gtest/gtest.h"

#include "IO/DirIO.h"

#include "Utils/Serialization/Serialize.h"
#include "Serialization/SerializationHelper.h"

#include "Utils/Comparators/Assets/TextureInputAssetComparator.h"

#include "Assets/Input/VideoInput/VideoInputTextureAssetDesc.h"
#include "Assets/Input/VideoInput/VideoInputAssetDesc.h"


using namespace bv;


// ========================================================================= //
// VideoInputTextureAssetDesc
// ========================================================================= //


// ***********************
// Serialize and deserialize VideoInputTextureAssetDesc.
TEST( Serialization_Assets, VideoInputTextureAssetDesc_SerDeser )
{
    auto expectedAssetDesc = VideoInputTextureAssetDesc::Create( 2, videocards::VideoType::Key );
    Serialize( *( expectedAssetDesc.get() ), "Out/SerDeserVideoInputTexture.xml" );
    auto actualAssetDesc = DeserializeAsset< const VideoInputTextureAssetDesc >( "Out/SerDeserVideoInputTexture.xml", "asset" );

    EXPECT_EQ( actualAssetDesc->GetVideoInputID(), expectedAssetDesc->GetVideoInputID() );
    EXPECT_EQ( actualAssetDesc->GetVideoInputID(), 2 );
    EXPECT_EQ( actualAssetDesc->GetVideoType(), videocards::VideoType::Key );
}

// ***********************
// Lack of VideoInputID attribute results in nullptr descriptor.
TEST( Serialization_Assets, VideoInputTextureAssetDesc_NoVideoInputID )
{
    auto actual = DeserializeAsset< const TextureInputAssetDesc >( "TestAssets/Serialization/Assets/VideoInputSlot/NoVideoInputID.xml", "asset" );
    ASSERT_EQ( actual, nullptr );
}

// ***********************
// Lack of VideoType attribute results in nullptr descriptor.
TEST( Serialization_Assets, VideoInputTextureAssetDesc_NoVideoType )
{
    auto actual = DeserializeAsset< const TextureInputAssetDesc >( "TestAssets/Serialization/Assets/VideoInputSlot/NoVideoType.xml", "asset" );
    ASSERT_EQ( actual, nullptr );
}



// ========================================================================= //
// VideoInputAssetDesc
// ========================================================================= //


// ***********************
// Serialize and deserialize VideoInputAssetDesc.
TEST( Serialization_Assets, VideoInputAssetDesc_SerDeser )
{
    auto expectedAssetDesc = VideoInputAssetDesc::Create( 2, 3 );
    Serialize( *( expectedAssetDesc.get() ), "Out/SerDeserVideoInput.xml" );
    auto actualAssetDesc = DeserializeAsset< const VideoInputAssetDesc >( "Out/SerDeserVideoInput.xml", "asset" );

    EXPECT_EQ( actualAssetDesc->GetVideoInputFillID(), expectedAssetDesc->GetVideoInputFillID() );
    EXPECT_EQ( actualAssetDesc->GetVideoInputFillID(), 2 );

    EXPECT_EQ( actualAssetDesc->GetVideoInputKeyID(), expectedAssetDesc->GetVideoInputKeyID() );
    EXPECT_EQ( actualAssetDesc->GetVideoInputKeyID(), 3 );
}

// ***********************
// Lack of VideoInputID attribute results in nullptr descriptor.
TEST( Serialization_Assets, VideoInputAssetDesc_NoVideoInputID )
{
    auto actual = DeserializeAsset< const VideoInputAssetDesc >( "TestAssets/Serialization/Assets/VideoInputSlot/VideoInputAsset_NoVideoInputID.xml", "asset" );
    ASSERT_EQ( actual, nullptr );
}
