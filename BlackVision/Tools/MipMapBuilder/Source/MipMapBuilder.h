#pragma once

#include <vector>
#include <string>

namespace tools
{

enum class FilterType
{
	BOX = 0,
	BILINEAR,
	B_SPLINE,
	BICUBIC,
	CATMULL_ROM,
	LANCZOS
};

struct Image32
{
	char *				data;
	unsigned int		width;
	unsigned int		height;
};

struct ImageSize
{
	unsigned int		width;
	unsigned int		height;

	ImageSize( unsigned int w, unsigned int h )
		: width( w )
		, height( h )
	{}
};


typedef std::vector< Image32 >		Mipmaps;
typedef std::vector< ImageSize >	MipmapsSizes;

Mipmaps				GenerateMipmaps( const Image32 & data, int levelsNum, FilterType ft );
Mipmaps				GenerateMipmaps( const std::string & imageFilePath, int levelsNum, FilterType ft );
MipmapsSizes		GenerateMipmapsSizes( const ImageSize & origSize );

} // tools