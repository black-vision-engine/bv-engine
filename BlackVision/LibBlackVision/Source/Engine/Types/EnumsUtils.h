#pragma once

#include "Enums.h"
#include "LibImage.h"

#include "Serialization/SerializationHelper.h"

namespace bv { 

class EnumsUtils
{
public:
	static TextureFormat		Convert	( image::ImageFormat ifmt );
};


DECLARE_ENUM_SERIALIZATION( PrimitiveType );


} // bv
