#include "MipMapBuilder.h"
#include "LibImage.h"
#include "Memory/MemoryChunk.h"
#include <sstream>

#include "gtest/gtest.h"

// ******************************
//
TEST( RunMimmaping, RunMimmaping )
{
	auto m1 = tools::GenerateMipmaps( "Resources/checkerbord2.bmp", 50, bv::image::FilterType::FT_CATMULL_ROM );
	ASSERT_TRUE( m1.size() == 11 );
	ASSERT_TRUE( m1[ 10 ].width == 1 &&  m1[ 10 ].height == 1 );

	bool success = false;

	int k = 0;

	for( auto i : m1 )
	{
		std::ostringstream ss;
		ss << k;

		success = bv::image::SaveBMPImage( "_Output/testFreeTypeMM" + ss.str() + ".bmp", i.data, i.width, i.height, 32 );
		ASSERT_TRUE(success);

		k++;
	}
}

int main( int argc, char **argv )
{
	::testing::InitGoogleTest( &argc, argv );
	return RUN_ALL_TESTS();
}
