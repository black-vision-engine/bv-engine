#pragma once

#include <mutex>

#include "Assets/VideoStream/AVAsset.h"
#include "Assets/VideoStream/AVAssetDescriptor.h"

#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/Interfaces/IAVDecoder.h"
#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/AVDecoderThread.h"

#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegDemuxer.h"
#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegDemuxerThread.h"

#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegVideoStreamDecoder.h"
#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegVideoStreamDecoderThread.h"

#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegAudioStreamDecoder.h"
#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegAudioStreamDecoderThread.h"

#include "DataTypes/QueueConcurrent.h"


namespace bv {


class FFmpegAVDecoder : public IAVDecoder
{
private:

    bool                            m_hasVideo;
    bool                            m_hasAudio;

	FFmpegDemuxerUPtr				m_demuxer;

	FFmpegVideoStreamDecoderUPtr	m_videoDecoder;
	FFmpegAudioStreamDecoderUPtr    m_audioDecoder;

	AVDecoderThreadUPtr			    m_decoderThread;
	mutable std::mutex				m_mutex;

    FFmpegDemuxerThreadUPtr         m_demuxerThread;

	FFmpegVideoStreamDecoderThreadUPtr  m_videoDecoderThread;
	FFmpegAudioStreamDecoderThreadUPtr  m_audioDecoderThread;

	QueueConcurrent< AVMediaData >	m_outVideoQueue;
	QueueConcurrent< AVMediaData > 	m_outAudioQueue;

public:

								FFmpegAVDecoder		    ( AVAssetConstPtr asset );
	virtual						~FFmpegAVDecoder		();

	virtual void				Play					() override;
	virtual void				Pause					() override;
	virtual void				Stop					() override;

	virtual AVMediaData		    GetVideoMediaData		() override;
	virtual AVMediaData		    GetAudioMediaData		() override;
    virtual AVMediaData		    GetSingleFrame  		( TimeType frameTime) override;

	virtual SizeType			GetVideoFrameSize	    () const override;

	virtual UInt32				GetWidth				() const override;
	virtual UInt32				GetHeight				() const override;
	virtual Float64				GetFrameRate			() const override;

	virtual Int32				GetSampleRate			() const override;
    virtual AudioFormat			GetAudioFormat			() const override;

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
	virtual SizeType		    NextAudioDataReady		() override;


    friend class FFmpegAudioStreamDecoderThread;

};

} //bv
