#pragma once

#include "FFmpegDef.h"

#include "Assets/VideoStream/VideoStreamAsset.h"

namespace bv
{

class FFmpegAudioStreamDecoder
{

private:

	AVStream *				m_stream;
	AVCodecContext *		m_codecCtx;
	AVCodec *				m_codec;

    Int32                   m_sampleRate;
    Int32                   m_channelsNum;
    UInt64                  m_channelLayout;

	Int32					m_streamIdx;

public:

							FFmpegAudioStreamDecoder	( VideoStreamAssetConstPtr asset, AVFormatContext * formatCtx, Int32 streamIdx );
							~FFmpegAudioStreamDecoder	();

	/** Converts time from seconds to the stream specific time base timestamp */
	Int64					ConvertTime					( Float64 time );

	Int32					DecodePacket				( AVPacket * packet, AVFrame * frame );
	void					ConvertFrame				( AVFrame * inFrame, AVFrame * outFrame );

	void					Reset						();

	Int32					GetStreamIdx				() const;

};

DEFINE_UPTR_TYPE( FFmpegAudioStreamDecoder )

} //bv
