#include "gtest/gtest.h"

#include "VideoCardDescFactory.h"

#include "Serialization/BV/XML/BVXMLDeserializer.h"
#include "Serialization/BV/BVDeserializeContext.h"

#include "Models/BlackMagic/BlackMagicVideoCard.h"
#include "Models/BlueFish/BlueFishVideoCard.h"


using namespace bv;
using namespace bv::videocards;


// ***********************
//
TEST( Tools_VideoCards, Bluefish_OutputVideoMode_1080_50_Interlaced )
{
    BVXMLDeserializer deser( "TestAssets/VideoCards/Bluefish/OutputVideoMode_1080_50_Interlaced.xml", BVDeserializeContext::CreateContextFromEmptiness() );

    bluefish::VideoCardDesc desc;
    desc.Deserialize( deser );

    auto result = desc.Validate();
    EXPECT_TRUE( result.IsValid() );
}

// ***********************
//
TEST( Tools_VideoCards, Bluefish_OutputVideoMode_1080_60_Interlaced )
{
    BVXMLDeserializer deser( "TestAssets/VideoCards/Bluefish/OutputVideoMode_1080_60_Interlaced.xml", BVDeserializeContext::CreateContextFromEmptiness() );

    bluefish::VideoCardDesc desc;
    desc.Deserialize( deser );

    auto result = desc.Validate();
    EXPECT_TRUE( result.IsValid() );
}

// ***********************
//
TEST( Tools_VideoCards, Bluefish_OutputVideoMode_1080_60 )
{
    BVXMLDeserializer deser( "TestAssets/VideoCards/Bluefish/OutputVideoMode_1080_60.xml", BVDeserializeContext::CreateContextFromEmptiness() );

    bluefish::VideoCardDesc desc;
    desc.Deserialize( deser );

    auto result = desc.Validate();
    EXPECT_TRUE( result.IsValid() );
}

// ***********************
//
TEST( Tools_VideoCards, Bluefish_OutputVideoMode_1080_50 )
{
    BVXMLDeserializer deser( "TestAssets/VideoCards/Bluefish/OutputVideoMode_1080_50.xml", BVDeserializeContext::CreateContextFromEmptiness() );

    bluefish::VideoCardDesc desc;
    desc.Deserialize( deser );

    auto result = desc.Validate();
    EXPECT_TRUE( result.IsValid() );
}
