#pragma once

#include "Engine/Models/Plugins/Channels/PixelShader/DefaultTextureDescriptor.h"

#include "Threading/Thread.h"
#include "CoreDEF.h"

extern "C"
{
	//#ifndef __STDC_CONSTANT_MACROS
	//#define __STDC_CONSTANT_MACROS
	//#endif

#pragma warning(push)
#pragma warning(disable : 4244)
	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
	#include <libswscale/swscale.h>
#pragma warning(pop)

}

namespace bv
{

class VideoStreamDecoderThread : public Thread
{

private:
	AVFormatContext *			m_formatCtx;
	AVCodecContext *			m_codecCtx;
	AVCodec *					m_codec;

	AVFrame *					m_frame;
	AVPicture *					m_outFrame;
	
	UInt32						m_width;
	UInt32						m_height;

	Int32						m_videoStreamIdx;					

	MemoryChunkPtr				m_frameData;
	UInt32						m_frameId;

	Int32						m_frameReady;

	SwsContext *				m_swsCtx;

public:
							VideoStreamDecoderThread	( const std::string & streamPath );
    virtual					~VideoStreamDecoderThread	();

	virtual void			Run							() override;

	UInt32					GetWidth					() const;	
	UInt32					GetHeight					() const;	

	MemoryChunkConstPtr		GetFrameData				( UInt32 & outFrameId ) const;	
	bool					NewVideoFrameArrived		( UInt32 lastFrameId ) const;	
};

} //bv
