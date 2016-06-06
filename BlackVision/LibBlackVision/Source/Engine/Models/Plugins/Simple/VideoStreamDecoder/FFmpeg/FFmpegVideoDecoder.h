#pragma once

#include <mutex>

#include "Assets/VideoStream/VideoStreamAsset.h"
#include "Assets/VideoStream/VideoStreamAssetDescriptor.h"

#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/Interfaces/IVideoDecoder.h"
#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/VideoDecoderThread.h"
#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegDemuxerThread.h"
#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegVideoStreamDecoderThread.h"
#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegAudioStreamDecoderThread.h"

#include "DataTypes/QueueConcurrent.h"

#include "FFmpegDemuxer.h"
#include "FFmpegVideoStreamDecoder.h"
#include "FFmpegAudioStreamDecoder.h"


namespace bv {


class FFmpegVideoDecoder : public IVideoDecoder
{
private:

    bool                            m_hasVideo;
    bool                            m_hasAudio;

	FFmpegDemuxerUPtr				m_demuxer;

	FFmpegVideoStreamDecoderUPtr	m_videoDecoder;
	FFmpegAudioStreamDecoderUPtr    m_audioDecoder;

	VideoDecoderThreadUPtr			m_decoderThread;
	mutable std::mutex				m_mutex;

    FFmpegDemuxerThreadUPtr         m_demuxerThread;

	FFmpegVideoStreamDecoderThreadUPtr  m_videoDecoderThread;
	FFmpegAudioStreamDecoderThreadUPtr  m_audioDecoderThread;

	QueueConcurrent< VideoMediaData >	m_outVideoQueue;
	QueueConcurrent< AudioMediaData > 	m_outAudioQueue;

public:

								FFmpegVideoDecoder		( VideoStreamAssetConstPtr asset );
	virtual						~FFmpegVideoDecoder		();

	virtual void				Play					() override;
	virtual void				Pause					() override;
	virtual void				Stop					() override;

	virtual VideoMediaData		GetVideoMediaData		() override;
	virtual AudioMediaData		GetAudioMediaData		() override;
    virtual VideoMediaData		GetSingleFrame  		( TimeType frameTime) override;

	virtual SizeType			GetFrameSize			() const override;

	virtual UInt32				GetWidth				() const override;
	virtual UInt32				GetHeight				() const override;
	virtual Float64				GetFrameRate			() const override;

    virtual bool                HasVideo                () const override;
    virtual bool                HasAudio                () const override;

	/** Accurate seeking.
	@param[time] in seconds 
    */
	virtual void				Seek					( Float64 time ) override;
	virtual void				FlushBuffers			() override;

	virtual void				Reset					() override;

	virtual bool				IsEOF					() const override;
	virtual bool				IsPacketQueueEmpty		( Int32 streamIdx ) const;
	virtual bool				IsFinished				() const override;

protected:
	
	virtual bool				NextVideoDataReady		() override;
	virtual bool				NextAudioDataReady		() override;


    friend class FFmpegAudioStreamDecoderThread;

};

} //bv
