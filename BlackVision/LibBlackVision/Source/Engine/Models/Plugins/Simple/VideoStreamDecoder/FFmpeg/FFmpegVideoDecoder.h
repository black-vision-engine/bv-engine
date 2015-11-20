#pragma once

#include <mutex>

#include "Assets/VideoStream/VideoStreamAssetDescriptor.h"

#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/Interfaces/IVideoDecoder.h"
#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/VideoDecoderThread.h"

#include "DataTypes/QueueConcurrent.h"

#include "FFmpegDemuxer.h"
#include "FFmpegVideoStreamDecoder.h"

namespace bv
{

class FFmpegVideoDecoder : public IVideoDecoder
{
private:
	FFmpegDemuxerUPtr				m_demuxer;

	FFmpegVideoStreamDecoderUPtr	m_vstreamDecoder;
	//FFmpegAudioStreamDecoderUPtr

	AVFrame *						m_frame;
	AVFrame *						m_outFrame;

	SizeType						m_frameSize;

	VideoDecoderThreadUPtr			m_decoderThread;
	mutable std::mutex				m_mutex;

	QueueConcurrent< VideoMediaData >	m_frameQueue;
	static const UInt32				MAX_QUEUE_SIZE;

public:
								FFmpegVideoDecoder		( VideoStreamAssetDescConstPtr desc );
	virtual						~FFmpegVideoDecoder		();

	virtual void				Start					() override;
	virtual void				Pause					() override;
	virtual void				Stop					() override;

	virtual VideoMediaData		GetVideoMediaData		() override;

	virtual SizeType			GetFrameSize			() const override;

	virtual UInt32				GetWidth				() const override;
	virtual UInt32				GetHeight				() const override;
	virtual Float64				GetFrameRate			() const override;

	/** @param[time] in seconds */
	virtual void				Seek					( Float64 time ) override;

	virtual void				Reset					() override;

	virtual bool				IsEOF					() const override;

protected:
	
	virtual bool				DecodeNextFrame			() override;
	virtual void				NextFrameDataReady		() override;

};

} //bv
