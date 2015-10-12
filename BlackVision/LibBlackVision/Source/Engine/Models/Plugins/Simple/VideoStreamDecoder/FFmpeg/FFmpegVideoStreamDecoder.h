#pragma once

#include "FFmpegDef.h"

namespace bv
{

class FFmpegVideoStreamDecoder
{

private:
	AVCodecContext *		m_codecCtx;
	AVCodec *				m_codec;

	SwsContext *			m_swsCtx;

	UInt32					m_width;
	UInt32					m_height;
	UInt32					m_frameRate;

	UInt32					m_currFrame;

	Int32					m_streamIdx;

public:
							FFmpegVideoStreamDecoder	( AVFormatContext * formatCtx, Int32 streamIdx );
							~FFmpegVideoStreamDecoder	();

	UInt32					GetWidth					() const;	
	UInt32					GetHeight					() const;	
	UInt32					GetFrameRate				() const;	

	bool					DecodePacket				( AVPacket * packet, AVFrame * frame );
	void					ConvertFrame				( AVFrame * inFrame, AVFrame * outFrame );

	Int32					GetStreamIdx				() const;

	UInt32					GetCurrentFrameId			() const;

	void					Reset						();
};

DEFINE_UPTR_TYPE( FFmpegVideoStreamDecoder )

} //bv
