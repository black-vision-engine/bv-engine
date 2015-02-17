#include "ImageProps.h"

#include "FreeImage.h"

#include <cassert>

ImageProps		GetImageProps( const std::string & imagePath )
{
	FIBITMAP * bitmap = FreeImage_Load( FIF_PNG, imagePath.c_str(), BMP_DEFAULT );
	
	auto type	= FreeImage_GetImageType( bitmap );
	auto bbp	= FreeImage_GetBPP( bitmap );

	ImageProps iprops;


	if( type == FIT_BITMAP && bbp == 32 )
		iprops.format = bv::TextureFormat::F_A8R8G8B8;
	else
		assert( !"image format not supported" );

	iprops.width = ( bv::SizeType )FreeImage_GetWidth( bitmap );
	iprops.height = ( bv::SizeType )FreeImage_GetHeight( bitmap );

	return iprops;
}