#pragma once
#pragma warning(push)
#pragma warning(disable : 4244) // warning C4244: 'return' : conversion from 'int' to 'uint8_t', possible loss of data

#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavutil/channel_layout.h>
#include <libavutil/common.h>
#include <libavutil/imgutils.h>
#include <libavutil/mathematics.h>
#include <libavutil/samplefmt.h>

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