#include "Include/MMBuilder.h"
#include "FreeImage.h"

#include <sstream>

#include "gtest/gtest.h"

// ******************************
//
bool SaveImageBMP( const std::string & lpszPathName, const tools::Image32 & img )
{
	auto inBitmap = FreeImage_ConvertFromRawBits( (BYTE*)img.data, img.width, img.height, img.width* 4, 32, 255, 255, 255 );
	return FreeImage_Save( FREE_IMAGE_FORMAT::FIF_BMP, inBitmap, lpszPathName.c_str() ) != 0;
}

TEST( RunMimmaping, RunMimmaping )
{
	auto m1 = tools::GenerateMipmaps( "Resources/checkerbord2.bmp", 50, tools::FilterType::CATMULL_ROM );
	ASSERT_TRUE( m1.size() == 11 );
	ASSERT_TRUE( m1[ 10 ].width == 1 &&  m1[ 10 ].height == 1 );

	bool success = false;

	int k = 0;

	for( auto i : m1 )
	{
		std::ostringstream ss;
		ss << k;

		success = SaveImageBMP( "_Output/testFreeTypeMM" + ss.str() + ".bmp", i );
		ASSERT_TRUE(success);

		k++;
	}
}

int main( int argc, char **argv )
{
	::testing::InitGoogleTest( &argc, argv );
	return RUN_ALL_TESTS();
}
