#include "gtest/gtest.h"

#include "IO/DirIO.h"

#include "Utils/Serialization/Serialize.h"
#include "Serialization/SerializationHelper.h"

#include "Utils/Comparators/Assets/TextureInputAssetComparator.h"

#include "Assets/Input/VideoInput/VideoInputTextureAssetDesc.h"


using namespace bv;


// ***********************
// Serialize and deserialize VideoInputAssetDesc.
TEST( Serialization_Assets, VideoInput_SerDeser )
{
    auto expectedAssetDesc = VideoInputTextureAssetDesc::Create( 2, videocards::VideoType::Key );
    Serialize( *( expectedAssetDesc.get() ), "Out/SerDeserVideoInput.xml" );
    auto actualAssetDesc = DeserializeAsset< const VideoInputTextureAssetDesc >( "Out/SerDeserVideoInput.xml", "asset" );

    EXPECT_EQ( actualAssetDesc->GetVideoInputID(), expectedAssetDesc->GetVideoInputID() );
    EXPECT_EQ( actualAssetDesc->GetVideoInputID(), 2 );
    EXPECT_EQ( actualAssetDesc->GetVideoType(), videocards::VideoType::Key );
}

// ***********************
// Lack of VideoInputID attribute results in nullptr descriptor.
TEST( Serialization_Assets, VideoInput_NoVideoInputID )
{
    auto actual = DeserializeAsset< const TextureInputAssetDesc >( "TestAssets/Serialization/Assets/VideoInputSlot/NoVideoInputID.xml", "asset" );
    ASSERT_EQ( actual, nullptr );
}

// ***********************
// Lack of VideoType attribute results in nullptr descriptor.
TEST( Serialization_Assets, VideoInput_NoVideoType )
{
    auto actual = DeserializeAsset< const TextureInputAssetDesc >( "TestAssets/Serialization/Assets/VideoInputSlot/NoVideoType.xml", "asset" );
    ASSERT_EQ( actual, nullptr );
}

