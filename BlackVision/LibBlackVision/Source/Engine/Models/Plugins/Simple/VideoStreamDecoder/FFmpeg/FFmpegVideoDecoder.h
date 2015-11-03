#pragma once

#include <mutex>

#include "Assets/VideoStream/VideoStreamAssetDescriptor.h"

#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/Interfaces/IVideoDecoder.h"
#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/VideoDecoderThread.h"

#include "FFmpegDemuxer.h"
#include "FFmpegVideoStreamDecoder.h"

namespace bv
{

class FFmpegVideoDecoder : public IVideoDecoder
{
private:
	FFmpegDemuxer *				m_demuxer;
	FFmpegVideoStreamDecoder *	m_vstreamDecoder;

	AVFrame *					m_frame;
	AVFrame *					m_outFrame;

	VideoDecoderThreadUPtr		m_decoderThread;

	MemoryChunkPtr				m_frameData;

	mutable std::mutex			m_dataMutex;

public:
								FFmpegVideoDecoder		( VideoStreamAssetDescConstPtr desc );
								~FFmpegVideoDecoder		();

	virtual void				StartDecoding			() override;

	virtual MemoryChunkConstPtr	GetCurrentFrameData		( UInt32 & outFrameId ) const override;
	virtual bool				GetNextFrameData		() override;

	virtual UInt32				GetWidth				() const override;
	virtual UInt32				GetHeight				() const override;
	virtual UInt32				GetFrameRate			() const override;

};

} //bv
