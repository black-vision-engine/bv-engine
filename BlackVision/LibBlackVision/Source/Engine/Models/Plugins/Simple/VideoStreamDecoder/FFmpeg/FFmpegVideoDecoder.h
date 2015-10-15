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
	//FFmpegAudioStreamDecoderUPtr

	AVFrame *						m_frame;
	AVFrame *						m_outFrame;

	VideoDecoderThreadUPtr			m_decoderThread;

	MemoryChunkPtr					m_frameData;
	SizeType						m_frameSize;

	mutable std::mutex				m_dataMutex;

public:
								FFmpegVideoDecoder		( VideoStreamAssetDescConstPtr desc );
								~FFmpegVideoDecoder		();

	virtual void				Start					() override;
	virtual void				Pause					() override;
	virtual void				Stop					() override;

	virtual MemoryChunkConstPtr	GetCurrentFrameData		( UInt64 & outFrameId ) const override;

	virtual bool				NextFrameDataReady		() override;

	virtual SizeType			GetFrameSize			() const override;

	virtual UInt32				GetWidth				() const override;
	virtual UInt32				GetHeight				() const override;
	virtual Float64				GetFrameRate			() const override;

	/** @param[time] in seconds */
	virtual void				Seek					( Float64 time ) override;

	virtual void				Reset					() override;

	virtual bool				IsEOF					() const override;

private:
	void						ClearFrameData			();

};

} //bv
