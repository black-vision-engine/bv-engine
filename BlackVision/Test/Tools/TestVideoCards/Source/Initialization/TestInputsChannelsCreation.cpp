#include "gtest/gtest.h"

#include "VideoCardDescFactory.h"

#include "Serialization/BV/XML/BVXMLDeserializer.h"
#include "Serialization/BV/BVDeserializeContext.h"

#include "Models/BlackMagic/BlackMagicVideoCard.h"
#include "Models/BlueFish/BlueFishVideoCard.h"


using namespace bv;
using namespace bv::videocards;


// ***********************
// This tests validates bug https://www.pivotaltracker.com/story/show/155286193.
// If to channels have the same name, they interrupt each other, because they use the same SDI input.
TEST( Tools_VideoCards, Bluefish_2ChannelsTheSameName )
{
    BVXMLDeserializer deser( "TestAssets/VideoCards/Bluefish/2Channels-TheSameName.xml", BVDeserializeContext::CreateContextFromEmptiness() );

    deser.EnterChild( "videocard" );

    bluefish::VideoCardDesc desc;
    desc.Deserialize( deser );

    auto result = desc.Validate();
    EXPECT_FALSE( result.IsValid() );
}

// ***********************
// If to channels of different types (input/output) have the same name, this is completly valid situation.
TEST( Tools_VideoCards, Bluefish_2InOutChannelsTheSameName )
{
    BVXMLDeserializer deser( "TestAssets/VideoCards/Bluefish/2Channels-TheSameName-InOut.xml", BVDeserializeContext::CreateContextFromEmptiness() );

    deser.EnterChild( "videocard" );

    bluefish::VideoCardDesc desc;
    desc.Deserialize( deser );

    auto result = desc.Validate();
    EXPECT_TRUE( result.IsValid() );
}


