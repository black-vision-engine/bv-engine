#pragma once

#include "FFmpegDef.h"
#include "CoreEnums.h"


namespace bv {


class FFmpegUtils
{

public:

	static AVPixelFormat		ToFFmpegPixelFormat		( TextureFormat format );
	static AVPixelFormat		ToFFmpegPixelFormat		( VideoPixelFormat format );

    static Int64                ConvertToMiliseconds    ( Float64 seconds );

    static std::string          AVErrorToString         ( Int32 errCode );

    static void                 AVCustomLog             ( void *, int level, const char * fmt, va_list vl );
};

} //bv
