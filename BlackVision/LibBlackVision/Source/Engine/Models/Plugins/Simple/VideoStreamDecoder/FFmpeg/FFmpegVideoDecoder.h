#pragma once

#include <mutex>

#include "Assets/VideoStream/VideoStreamAsset.h"
#include "Assets/VideoStream/VideoStreamAssetDescriptor.h"

#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/Interfaces/IVideoDecoder.h"
#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/VideoDecoderThread.h"

#include "DataTypes/QueueConcurrent.h"

#include "FFmpegDemuxer.h"
#include "FFmpegVideoStreamDecoder.h"
#include "FFmpegAudioStreamDecoder.h"


namespace bv
{

enum class FFmpegSyncType : int
{
    FST_AUDIO_MASTER = 0,
    FST_VIDEO_MASTER,
    FST_EXTERNAL_CLOCK
};

struct FFmpegClock
{
    Float64             pts;
    Float64             ptsDrift;
    Float64             lastUpdated;
    Float64             speed;
    Int32               serial;
    Int32               paused;
};

struct FFmpegFrame 
{
    AVFrame *           frame;
    Float64             pts;
    Float64             duration;
    Int32               serial;
};

class FFmpegVideoDecoder : public IVideoDecoder
{
private:

	FFmpegDemuxerUPtr				m_demuxer;

	FFmpegVideoStreamDecoderUPtr	m_vstreamDecoder;
	FFmpegAudioStreamDecoderUPtr    m_astreamDecoder;

    FFmpegFrame                     m_frame;
    FFmpegFrame                     m_outFrame;
    uint8_t *                       m_outBuffer;

    FFmpegClock                     m_videoClock;
    FFmpegClock                     m_audioClock;
    FFmpegClock                     m_externalClock;

	SizeType						m_frameSize;

	VideoDecoderThreadUPtr			m_decoderThread;
	mutable std::mutex				m_mutex;

    FFmpegSyncType                  m_syncType;

	QueueConcurrent< VideoMediaData >	m_outQueue;
	QueueConcurrent< VideoMediaData >	m_bufferQueue;

	static const UInt32				MAX_QUEUE_SIZE;

public:

								FFmpegVideoDecoder		( VideoStreamAssetConstPtr asset );
	virtual						~FFmpegVideoDecoder		();

	virtual void				Play					() override;
	virtual void				Pause					() override;
	virtual void				Stop					() override;

	virtual VideoMediaData		GetVideoMediaData		() override;
    virtual VideoMediaData		GetSingleFrame  		( TimeType frameTime) override;

	virtual SizeType			GetFrameSize			() const override;

	virtual UInt32				GetWidth				() const override;
	virtual UInt32				GetHeight				() const override;
	virtual Float64				GetFrameRate			() const override;

    virtual UInt32              GetMaxBufferSize        () const override;

	/** Accurate seeking.
	@param[time] in seconds 
    */
	virtual void				Seek					( Float64 time ) override;
	virtual void				FlushBuffers			() override;

	virtual void				Reset					() override;

	virtual bool				IsEOF					() const override;
	virtual bool				IsFinished				() const override;

    Float64                     GetMaterClock           () const;

protected:
	
	virtual bool				DecodeNextFrame			() override;
	virtual bool				NextFrameDataReady		() override;

private:

    void                        InitClock               ( FFmpegClock * clock );
    void                        SetClock                ( FFmpegClock * clock, Float64 pts, Int32 serial );
    Float64                     GetClock                ( const FFmpegClock * clock ) const;

};

} //bv
