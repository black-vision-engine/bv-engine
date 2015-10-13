#pragma once

#include "FFmpegDef.h"

#include "Assets/VideoStream/VideoStreamAssetDescriptor.h"

namespace bv
{

class FFmpegVideoStreamDecoder
{

private:
	AVStream *				m_stream;
	AVCodecContext *		m_codecCtx;
	AVCodec *				m_codec;

	SwsContext *			m_swsCtx;

	UInt32					m_width;
	UInt32					m_height;
	Float64					m_frameRate;
	UInt64					m_duration;

	Int32					m_streamIdx;

public:
							FFmpegVideoStreamDecoder	( VideoStreamAssetDescConstPtr desc, AVFormatContext * formatCtx, Int32 streamIdx );
							~FFmpegVideoStreamDecoder	();

	UInt32					GetWidth					() const;	
	UInt32					GetHeight					() const;	
	Float64					GetFrameRate				() const;	
	UInt64					GetDuration					() const;	

	bool					DecodePacket				( AVPacket * packet, AVFrame * frame );
	void					ConvertFrame				( AVFrame * inFrame, AVFrame * outFrame );

	Int32					GetStreamIdx				() const;

	UInt32					GetCurrentFrameId			() const;
};

DEFINE_UPTR_TYPE( FFmpegVideoStreamDecoder )

} //bv
