#pragma once

#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/Interfaces/IVideoStreamDecoder.h"

#include "FFmpegDef.h"

namespace bv
{

class FFmpegVideoStreamDecoder : public IVideoStreamDecoder
{

private:
	AVCodecContext *			m_codecCtx;
	AVCodec *					m_codec;

	SwsContext *				m_swsCtx;

	UInt32						m_width;
	UInt32						m_height;
	UInt32						m_frameRate;

	UInt32						m_currFrame;

	Int32						m_streamIdx;

public:
									FFmpegVideoStreamDecoder	( AVFormatContext * formatCtx, Int32 streamIdx );
    virtual							~FFmpegVideoStreamDecoder	();

	virtual UInt32					GetWidth					() const override;	
	virtual UInt32					GetHeight					() const override;	
	virtual UInt32					GetFrameRate				() const override;	

	//FIXME...
	bool							DecodePacket				( AVPacket * packet, AVFrame * frame );
	void							ConvertFrame				( AVFrame * inFrame, AVFrame * outFrame );

	//virtual MemoryChunkConstPtr		GetFrameData				( UInt32 & outFrameId ) const override;
	
	virtual Int32					GetStreamIdx				() const override;
	virtual UInt32					GetCurrentFrameId			() const override;	
};

} //bv
