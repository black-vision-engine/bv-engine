#include "EnumsUtils.h"

namespace bv { 

namespace 
{

TextureFormat TextureFormatBV[] =
{
	TextureFormat::F_A8R8G8B8	, //IF_A8R8G8B8
	TextureFormat::F_R8G8B8		, //IF_R8G8B8
	TextureFormat::F_A8			, //IF_A8
};

} // anonymous

TextureFormat	EnumsUtils::Convert	( image::ImageFormat ifmt )
{
	return TextureFormatBV[ ( int )ifmt ];
}

} // bv