#include "Include/MMBuilder.h"
#include "FreeImage.h"

#include <cassert>

namespace tools
{

namespace 
{

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

	Image32 ret;

	ret.data = ( char * ) FreeImage_GetBits( inBitmap );
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

	Mipmaps res;
	res.resize( levelsNum );

	res[ 0 ].data		= imgPowOfTwo.data;
	res[ 0 ].height		= imgPowOfTwo.height;
	res[ 0 ].width		= imgPowOfTwo.width;

	if( levelsNum > 1 )
		for( int i = 1; i < levelsNum; ++i )
			res[ i ] = GenerateNextLevelMipmap( res[ 0 ], ft );

	return res;
}

// ******************************
//
Mipmaps				GeneratMipmaps( const std::string & imageFilePah, int levelsNum, FilterType ft )
{
	return Mipmaps();
}

} // tools
