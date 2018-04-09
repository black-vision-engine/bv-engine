#include "gtest/gtest.h"

#include "VideoCardDescFactory.h"

#include "Serialization/BV/XML/BVXMLDeserializer.h"
#include "Serialization/BV/BVDeserializeContext.h"

#include "Models/BlackMagic/BlackMagicVideoCard.h"
#include "Models/BlueFish/BlueFishVideoCard.h"


using namespace bv;
using namespace bv::videocards;



// ***********************
// Test config with reference mode which isn't one of ReferenceMode enum values.
// Descriptor validation should fail.
TEST( Tools_VideoCards, Bluefish_InvalidReferenceMode )
{
    BVXMLDeserializer deser( "TestAssets/VideoCards/Bluefish/InvalidReferenceMode.xml", BVDeserializeContext::CreateContextFromEmptiness() );
    deser.EnterChild( "videocard" );

    bluefish::VideoCardDesc desc;
    desc.Deserialize( deser );

    auto result = desc.Validate();
    EXPECT_TRUE( !result.IsValid() );
}


