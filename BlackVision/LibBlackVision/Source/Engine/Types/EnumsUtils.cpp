#include "EnumsUtils.h"

namespace bv { 

namespace 
{

TextureFormat TextureFormatBV[] =
{
	TextureFormat::F_A32FR32FG32FB32F	,	// IF_A32FR32FG32FB32F
	TextureFormat::F_R32FG32FB32F		,	// IF_R32FG32FB32F
	TextureFormat::F_A32F				,	// IF_A32F
	TextureFormat::F_A16R16G16B16		,	// IF_A16R16G16B16
	TextureFormat::F_R16G16B16			,	// IF_R16G16B16
	TextureFormat::F_A16				,	// IF_A16
	TextureFormat::F_A8R8G8B8			,	// IF_A8R8G8B8
	TextureFormat::F_R8G8B8				,	// IF_R8G8B8
	TextureFormat::F_A8					,	// IF_A8
	TextureFormat::F_L8					,	// IF_L8
};

} // anonymous

TextureFormat	EnumsUtils::Convert	( image::ImageFormat ifmt )
{
	return TextureFormatBV[ ( int )ifmt ];
}

} // bv