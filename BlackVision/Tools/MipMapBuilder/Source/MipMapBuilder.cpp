#include "MipMapBuilder.h"
#include "FreeImage.h"

#include <cassert>

namespace tools
{

namespace
{

// ******************************
//
FIBITMAP * LoadImage( const std::string & lpszPathName )
{
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType( lpszPathName.c_str(), 0 );

	if( fif == FIF_UNKNOWN )
		fif = FreeImage_GetFIFFromFilename( lpszPathName.c_str() );

	if( ( fif != FIF_UNKNOWN ) && FreeImage_FIFSupportsReading( fif ) )
	{
		FIBITMAP * dib = FreeImage_Load(fif, lpszPathName.c_str() );
		dib = FreeImage_ConvertTo32Bits( dib );
		return dib;
	}

	return nullptr;
}

// ******************************
//
bool		IsPowerOfTwo( unsigned int x )
{
	return (x & (x - 1)) == 0;
}

// ******************************
//
unsigned int RoundUpToPowerOfTwo( unsigned int v )
{
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v++;

	return v;
}

// ******************************
//
FREE_IMAGE_FILTER ToFIFilter( FilterType ft )
{
	switch ( ft )
	{
	case FilterType::BOX:
		return FREE_IMAGE_FILTER::FILTER_BOX;
	case FilterType::BILINEAR:
		return FREE_IMAGE_FILTER::FILTER_BILINEAR;
	case FilterType::B_SPLINE:
		return FREE_IMAGE_FILTER::FILTER_BSPLINE;
	case FilterType::BICUBIC:
		return FREE_IMAGE_FILTER::FILTER_BICUBIC;
	case FilterType::CATMULL_ROM:
		return FREE_IMAGE_FILTER::FILTER_CATMULLROM;
	case FilterType::LANCZOS:
		return FREE_IMAGE_FILTER::FILTER_LANCZOS3;
	default:
		assert( false && "Unreachable" );
		return FREE_IMAGE_FILTER::FILTER_BOX;
	}
}

// ******************************
//
Image32		Resize( const Image32 & in, unsigned int newWidth, unsigned int newHeight, FilterType ft )
{
	auto inBitmap = FreeImage_ConvertFromRawBits( (BYTE*)in.data, in.width, in.height, in.width * 4, 32, 255, 255, 255 );

	auto outBitmap = FreeImage_Rescale( inBitmap, newWidth, newHeight, ToFIFilter( ft ) );

	outBitmap = FreeImage_ConvertTo32Bits( outBitmap );

	Image32 ret;

	auto numBytes = newWidth * newHeight * 4;

    char * pixels = new char[ numBytes ]; // FIXME: Use normal allocation to free it with free not delete []
    memcpy( pixels, FreeImage_GetBits( outBitmap ), numBytes );

	ret.data = pixels;
	ret.width = newWidth;
	ret.height = newHeight;

	return ret;
}

// ******************************
//
Image32		EnlargeImageToPowerOfTwo( const Image32 & in, FilterType ft )
{
	if( IsPowerOfTwo( in.width ) && IsPowerOfTwo( in.height ) )
		return in;
	
	unsigned int newWidth	= RoundUpToPowerOfTwo( in.width );
	unsigned int newHeight	= RoundUpToPowerOfTwo( in.height );

	assert( IsPowerOfTwo( newWidth ) && IsPowerOfTwo( newHeight ) );


	return Resize( in, newWidth, newHeight, ft );
}

// ******************************
//
Image32		GenerateNextLevelMipmap( const Image32 & in, FilterType ft )
{
	assert( IsPowerOfTwo( in.width ) && IsPowerOfTwo( in.height ) );

	auto newWidth	= in.width > 1 ? in.width / 2 : 1;
	auto newHeight	= in.height > 1 ? in.height / 2 : 1;

	if( newWidth == in.width &&  newHeight == in.height )
		return in;

	return Resize( in, newWidth, newHeight, ft );
}


} //anonymous

// ******************************
//
Mipmaps				GenerateMipmaps( const Image32 & data, int levelsNum, FilterType ft )
{
	auto imgPowOfTwo = EnlargeImageToPowerOfTwo( data, ft );

	Mipmaps res( 1 );

	res[ 0 ].data		= imgPowOfTwo.data;
	res[ 0 ].height		= imgPowOfTwo.height;
	res[ 0 ].width		= imgPowOfTwo.width;

	if( levelsNum > 1 )
		for( int i = 1; i < levelsNum; ++i ) 
		{
			if( res[ i - 1 ].width > 1 || res[ i - 1 ].height > 1 )
				res.push_back( GenerateNextLevelMipmap( res[ i - 1 ], ft ) );
			else
				break;
		}

	return res;
}

// ******************************
//
Mipmaps				GenerateMipmaps( const std::string & imageFilePath, int levelsNum, FilterType ft )
{
	auto fiBitmap = LoadImage(  imageFilePath );

	if( !fiBitmap )
		return Mipmaps();

	assert( FreeImage_GetBPP( fiBitmap ) == 32 );

	auto w = FreeImage_GetWidth( fiBitmap );
	auto h = FreeImage_GetHeight( fiBitmap );

	auto numBytes = w * h * 4;

    char * pixels = new char[ numBytes ]; // FIXME: Use normal allocation to free it with free not delete []
    memcpy( pixels, FreeImage_GetBits( fiBitmap ), numBytes );

	Image32 img;
	img.data	= pixels;
	img.width	= FreeImage_GetWidth( fiBitmap );
	img.height	= FreeImage_GetHeight( fiBitmap );

	return GenerateMipmaps( img, levelsNum, ft );
}

// ******************************
//
MipmapsSizes		GenerateMipmapsSizes( const ImageSize & origSize )
{
	MipmapsSizes ret;
	unsigned int newWidth	= RoundUpToPowerOfTwo( origSize.width );
	unsigned int newHeight	= RoundUpToPowerOfTwo( origSize.height );

	ret.push_back( ImageSize( newWidth, newHeight ) );

	while( newWidth > 1 || newHeight > 1 )
	{
		newWidth	= newWidth > 1 ? newWidth / 2 : 1;
		newHeight	= newHeight > 1 ? newHeight / 2 : 1;
		ret.push_back( ImageSize( newWidth, newHeight ) );
	}

	return ret;
}

} // tools
