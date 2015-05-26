#pragma once

#include <vector>
#include <string>

#include "LibImage.h"


namespace tools
{

struct Image
{
	bv::MemoryChunkConstPtr	data;
	bv::UInt32				width;
	bv::UInt32				height;
	bv::UInt32				bpp;
};

struct ImageSize
{
	bv::UInt32		width;
	bv::UInt32		height;

	ImageSize( bv::UInt32 w, bv::UInt32 h )
		: width( w )
		, height( h )
	{}
};


typedef std::vector< Image >		Mipmaps;
typedef std::vector< ImageSize >	MipmapsSizes;

Mipmaps				GenerateMipmaps( const Image & data, int levelsNum, bv::image::FilterType ft );
Mipmaps				GenerateMipmaps( const std::string & imageFilePath, int levelsNum, bv::image::FilterType ft );
MipmapsSizes		GenerateMipmapsSizes( const ImageSize & origSize );

} // tools