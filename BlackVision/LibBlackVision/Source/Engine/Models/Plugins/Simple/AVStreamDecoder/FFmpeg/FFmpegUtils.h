#pragma once

#include "FFmpegDef.h"
#include "Engine/Types/Enums.h"


namespace bv {


class FFmpegUtils
{

public:

	static AVPixelFormat		ToFFmpegPixelFormat		( TextureFormat format );
	static AVPixelFormat		ToFFmpegPixelFormat		( VideoPixelFormat format );

    static Int64                ConvertToMiliseconds    ( Float64 seconds );

    static std::string          AVErrorToString         ( Int32 errCode );
};

} //bv
