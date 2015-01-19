#pragma once

#include <vector>

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

typedef std::vector< Image32 >		Mipmaps;

Mipmaps				GenerateMipmaps( const Image32 & data, int levelsNum, FilterType ft );
Mipmaps				GenerateMipmaps( const std::string & imageFilePah, int levelsNum, FilterType ft );

} // tools