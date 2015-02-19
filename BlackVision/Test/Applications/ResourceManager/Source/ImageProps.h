#pragma once

#include "System/BasicTypes.h"
#include "Engine/Types/Enums.h"

#include <string>

struct ImageProps
{
	bv::SizeType		width;
	bv::SizeType		height;
	bv::TextureFormat	format;
};

	
ImageProps		GetImageProps( const std::string & imagePath );

