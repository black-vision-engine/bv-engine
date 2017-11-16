#include "gtest/gtest.h"

#include "VideoCardDescFactory.h"


#include "Models/BlackMagic/BlackMagicVideoCard.h"
#include "Models/BlueFish/BlueFishVideoCard.h"


using namespace bv::videocards;


// ***********************
//
TEST( Tools_VideoCards, VideoCardDescFactory_RegisterDescCreator )
{
    VideoCardDescFactory factory;

    factory.RegisterDescriptor( bluefish::VideoCardDesc::UID(), &bluefish::VideoCardDesc::CreateDescriptor );

    ASSERT_NE( factory.CreateEmptyDescriptor( bluefish::VideoCardDesc::UID() ), nullptr );
}

