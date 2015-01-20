#include "Include/MMBuilder.h"
#include "FreeImage.h"

#include <sstream>

#define PERFORM_UNIT_TESTS
#ifdef PERFORM_UNIT_TESTS
//TESTS

// ******************************
//
bool SaveImageBMP( const std::string & lpszPathName, const tools::Image32 & img )
{
	auto inBitmap = FreeImage_ConvertFromRawBits( (BYTE*)img.data, img.width, img.height, img.width* 4, 32, 255, 255, 255 );
	return FreeImage_Save( FREE_IMAGE_FORMAT::FIF_BMP, inBitmap, lpszPathName.c_str() ) != 0;
}

int main()
{
	auto m1 = tools::GenerateMipmaps( "Tests/Resources/checkerbord2.bmp", 50, tools::FilterType::CATMULL_ROM );

	bool success = false;

	int k = 0;

	for( auto i : m1 )
	{
		std::ostringstream ss;
		ss << k;

		success = SaveImageBMP( "Tests/Output/testFreeTypeMM" + ss.str() + ".bmp", i );
		if( !success )
			return -1;

		k++;
	}

	return 0;
}

#endif // PERFORM_UNIT_TESTS
