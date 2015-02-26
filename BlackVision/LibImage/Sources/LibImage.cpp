#include "LibImage.h"

#include "LibImage.h"

#include "FreeImage.h"

#include "System/FileIO.h"

#include <cassert>

namespace bv { namespace image
{

ImageProperties GetImageProps( const std::string & imageFilePath )
{
	if( !File::Exists( imageFilePath ) )
	{
		ImageProperties iprops;
		iprops.error = std::string("File doesn't exist");
		return iprops;
	}

	auto size = File::Size( imageFilePath );

	FREE_IMAGE_FORMAT fiff = FreeImage_GetFileType( imageFilePath.c_str(), (int) size );

	if( fiff == FIF_UNKNOWN )
	{
		ImageProperties iprops;
		iprops.error = std::string("Unknown file format");
		return iprops;
	}

	FIBITMAP * bitmap = FreeImage_Load( fiff, imageFilePath.c_str(), BMP_DEFAULT );

	if( bitmap == nullptr )
	{
		ImageProperties iprops;
		iprops.error = std::string("Cannot load file: ") + imageFilePath;
		return iprops;
	}

	auto bbp		= FreeImage_GetBPP( bitmap );
	auto colorType	= FreeImage_GetColorType( bitmap );

	TextureFormat format;

	if( colorType == FIC_RGBALPHA && bbp == 32 )
		format = bv::TextureFormat::F_A8R8G8B8;
	else if ( colorType == FIC_RGB && bbp == 24 )
		format = bv::TextureFormat::F_R8G8B8;
	else if ( bbp == 8 )
		format = bv::TextureFormat::F_A8;
	else
	{
		ImageProperties iprops;
		iprops.error = std::string("Image format not supported");
		return iprops;
	}

	ImageProperties iprops;

	iprops.format = format;
	iprops.width = ( bv::SizeType )FreeImage_GetWidth( bitmap );
	iprops.height = ( bv::SizeType )FreeImage_GetHeight( bitmap );

	return iprops;
}


} // image
} // bv