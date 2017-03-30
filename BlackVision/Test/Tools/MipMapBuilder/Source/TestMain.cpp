#include "MipMapBuilder.h"
#include "LibImage.h"
#include "Memory/MemoryChunk.h"
#include "IO/DirIO.h"
#include <sstream>

#include "gtest/gtest.h"

auto imagePath = "TestAssets/MipMapBuilder/checkerboard2.png";
auto TestOutputDir = "_Output";

// ******************************
//
TEST( RunMimmaping, RunMimmaping )
{
	auto m1 = tools::GenerateMipmaps( imagePath, 50, bv::image::FilterType::FT_CATMULL_ROM );
	ASSERT_TRUE( m1.size() == 11 );
	ASSERT_TRUE( m1[ 10 ].width == 1 &&  m1[ 10 ].height == 1 );

	bool success = false;

	int k = 0;

    if( !bv::Dir::Exists( TestOutputDir ) )
    {
        bv::Dir::CreateDir( TestOutputDir );
    }

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
