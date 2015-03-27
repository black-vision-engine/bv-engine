#pragma once

#include "LibImage.h"
#include <vector>
#include <string>

namespace tools
{

struct Image32
{
	bv::MemoryChunkConstPtr	data;
	bv::UInt32				width;
	bv::UInt32				height;
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


typedef std::vector< Image32 >		Mipmaps;
typedef std::vector< ImageSize >	MipmapsSizes;

Mipmaps				GenerateMipmaps( const Image32 & data, int levelsNum, bv::image::FilterType ft );
Mipmaps				GenerateMipmaps( const std::string & imageFilePath, int levelsNum, bv::image::FilterType ft );
MipmapsSizes		GenerateMipmapsSizes( const ImageSize & origSize );

} // tools