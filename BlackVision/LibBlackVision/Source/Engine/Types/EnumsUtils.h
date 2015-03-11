#pragma once

#include "Enums.h"
#include "LibImage.h"

namespace bv { 

class EnumsUtils
{
public:
	static TextureFormat		Convert	( image::ImageFormat ifmt );
};

} // bv
