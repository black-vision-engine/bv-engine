#pragma once

#include "System/BasicTypes.h" // FIXME: Add project LibCore and move all such a functionality there.
#include "Engine/Types/Enums.h" // FIXME: Add project LibCore and move all such a functionality there.

namespace bv { namespace image
{

struct ImageProperties
{
	SizeType		width;
	SizeType		height;
	TextureFormat	format;
	std::string		error;
};

	
ImageProperties	GetImageProps( const std::string & imageFilePath );


} // image
} // bv