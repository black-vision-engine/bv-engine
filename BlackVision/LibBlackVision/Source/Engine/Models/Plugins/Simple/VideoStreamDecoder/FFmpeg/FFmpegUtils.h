#pragma once

#include "FFmpegDef.h"
#include "Engine/Types/Enums.h"


namespace bv {


class FFmpegUtils
{

public:

	static AVPixelFormat		ToFFmpegPixelFormat		( TextureFormat format );
	static AVPixelFormat		ToFFmpegPixelFormat		( VideoPixelFormat format );

};

} //bv
