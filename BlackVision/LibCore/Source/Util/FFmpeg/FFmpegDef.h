#pragma once

#include "CoreDEF.h"

extern "C"
{
	#ifndef __STDC_CONSTANT_MACROS
	#define __STDC_CONSTANT_MACROS
	#endif

#pragma warning(push)
#pragma warning(disable : 4244)
	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
	#include <libswscale/swscale.h>
    #include <libavutil/imgutils.h>
    #include <libavutil/time.h>
    #include <libswresample/swresample.h>
	#include <libavutil/opt.h>
	#include <libavutil/error.h>
	//#include <libavutil/timestamp.h>
#pragma warning(pop)

}