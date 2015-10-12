#pragma once

#include <mutex>

#include "Assets/VideoStream/VideoStreamAssetDescriptor.h"

#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/Interfaces/IVideoDecoder.h"
#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/VideoDecoderThread.h"

#include "FFmpegDemuxer.h"
#include "FFmpegVideoStreamDecoder.h"

namespace bv
{

class FFmpegVideoDecoder : public IVideoDecoder, public std::enable_shared_from_this< IVideoDecoder >
{
private:
	FFmpegDemuxerUPtr				m_demuxer;

	FFmpegVideoStreamDecoderUPtr	m_vstreamDecoder;
	//audio stream decoder

	AVFrame *						m_frame;
	AVFrame *						m_outFrame;

	VideoDecoderThreadUPtr			m_decoderThread;

	MemoryChunkPtr					m_frameData;
	mutable std::mutex				m_dataMutex;

public:
								FFmpegVideoDecoder		( VideoStreamAssetDescConstPtr desc );
								~FFmpegVideoDecoder		();

	virtual void				Start					() override;
	virtual void				Pause					() override;
	virtual void				Stop					() override;

	virtual MemoryChunkConstPtr	GetCurrentFrameData		( UInt32 & outFrameId ) const override;
	virtual bool				GetNextFrameData		() override;

	virtual UInt32				GetWidth				() const override;
	virtual UInt32				GetHeight				() const override;
	virtual UInt32				GetFrameRate			() const override;
	virtual UInt32				GetDuration				() const override;

};

} //bv
