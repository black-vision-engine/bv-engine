#include "gtest/gtest.h"

#include "IO/DirIO.h"

#include "Utils/Serialization/Serialize.h"
#include "Serialization/SerializationHelper.h"

#include "Utils/Comparators/Assets/TextureInputAssetComparator.h"

#include "Assets/Input/VideoInputAssetDesc.h"


using namespace bv;


// ***********************
// Serialize and deserialize VideoInputAssetDesc.
TEST( Serialization_Assets, VideoInput_SerDeser )
{
    InputSlotBinding binding( "slot blabla" );

    auto expectedAssetDesc = VideoInputAssetDesc::Create( 2 );
    Serialize( *( expectedAssetDesc.get() ), "Out/SerDeserVideoInput.xml" );
    auto actualAssetDesc = Deserialize< const VideoInputAssetDesc >( "Out/SerDeserVideoInput.xml", "asset" );

    EXPECT_EQ( actualAssetDesc->GetVideoInputID(), expectedAssetDesc->GetVideoInputID() );
    EXPECT_EQ( actualAssetDesc->GetVideoInputID(), 2 );
}



