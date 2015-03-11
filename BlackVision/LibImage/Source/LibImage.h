#pragma once

#include "CoreDEF.h"

namespace bv { namespace image
{

enum class ImageFormat : int
{
    IF_A8R8G8B8 = 0	,
    IF_R8G8B8       ,
    IF_A8           ,
    IF_TOTAL
};

struct ImageProperties
{
	UInt32			width;
	UInt32			height;
	ImageFormat		format;
	std::string		error;
};

	
ImageProperties	GetImageProps( const std::string & imageFilePath );


} // image
} // bv