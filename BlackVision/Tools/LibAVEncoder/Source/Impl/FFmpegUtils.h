#pragma once

#pragma warning(push)
#pragma warning(disable : 4244) // warning C4244: 'return' : conversion from 'int' to 'uint8_t', possible loss of data

#include "Util/FFmpeg/FFmpegDef.h"
extern "C"
{

//#include <libavutil/avassert.h>
//#include <libavutil/channel_layout.h>
//#include <libavutil/mathematics.h>

//#include <libavformat/avformat.h>
//#include <libswscale/swscale.h>
//#include <libswresample/swresample.h>
//#include <libavutil/error.h>
}
#pragma warning(pop)

namespace bv 
{

namespace avencoder
{

struct FFmpegUtils
{

};

} //avencoder
} // bv