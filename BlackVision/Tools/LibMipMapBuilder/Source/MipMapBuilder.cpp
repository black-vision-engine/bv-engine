#include "MipMapBuilder.h"
#include "LibImage.h"
#include "tools/Utils.h"
#include <cassert>

namespace tools
{

namespace
{

// ******************************
//
Image		EnlargeImageToPowerOfTwo( const Image & in, bv::image::FilterType ft )
{
	if( bv::IsPowerOfTwo( in.width ) && bv::IsPowerOfTwo( in.height ) )
		return in;
	
	auto newWidth	= bv::RoundUpToPowerOfTwo( in.width );
	auto newHeight	= bv::RoundUpToPowerOfTwo( in.height );

	assert( bv::IsPowerOfTwo( newWidth ) && bv::IsPowerOfTwo( newHeight ) );

	auto data = bv::image::Resize( in.data, in.width, in.height, in.bpp, newWidth, newHeight, ft );

	Image ret = { data, newWidth, newHeight, in.bpp };

	return ret;
}

// ******************************
//
Image		GenerateNextLevelMipmap( const Image & in, bv::image::FilterType ft )
{
	assert( bv::IsPowerOfTwo( in.width ) && bv::IsPowerOfTwo( in.height ) );

	auto newWidth	= in.width > 1 ? in.width / 2 : 1;
	auto newHeight	= in.height > 1 ? in.height / 2 : 1;

	if( newWidth == in.width &&  newHeight == in.height )
		return in;

	auto data = Resize( in.data, in.width, in.height, in.bpp, newWidth, newHeight, ft );

	//bv::image::SaveBMPImage( std::to_string( newWidth ) + std::to_string( newHeight ) + ".bmp", data, newWidth, newHeight, 32 );

	Image ret = { data, newWidth, newHeight, in.bpp };

	return ret;
}


} //anonymous

// ******************************
//
Mipmaps				GenerateMipmaps( const Image & data, int levelsNum, bv::image::FilterType ft )
{
	auto imgPowOfTwo = EnlargeImageToPowerOfTwo( data, ft );

	Mipmaps res( 1 );

	res[ 0 ].data		= imgPowOfTwo.data;
	res[ 0 ].height		= imgPowOfTwo.height;
	res[ 0 ].width		= imgPowOfTwo.width;
	res[ 0 ].bpp		= imgPowOfTwo.bpp;

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
Mipmaps				GenerateMipmaps( const std::string & imageFilePath, int levelsNum, bv::image::FilterType ft )
{
	auto props = bv::image::GetImageProps( imageFilePath );

	if( !props.error.empty() )
	{
		return Mipmaps();
	}

	bv::UInt32 w;
	bv::UInt32 h;
	bv::UInt32 bbp;
	bv::UInt32 channelNum;

	auto imgMC = bv::image::LoadImage( imageFilePath, &w, &h, &bbp, &channelNum );
	
	if( !imgMC )
		return Mipmaps();

	Image img = { imgMC, props.width, props.height, bbp };

	return GenerateMipmaps( img, levelsNum, ft );
}

// ******************************
//
MipmapsSizes		GenerateMipmapsSizes( const ImageSize & origSize )
{
	MipmapsSizes ret;
	auto newWidth	= bv::RoundUpToPowerOfTwo( origSize.width );
	auto newHeight	= bv::RoundUpToPowerOfTwo( origSize.height );

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
