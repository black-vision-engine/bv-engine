#pragma once

#include <mutex>

#include "Assets/VideoStream/AVAsset.h"
#include "Assets/VideoStream/AVAssetDescriptor.h"

#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/Interfaces/IAVDecoder.h"
#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/AVDecoderThread.h"

#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegDemuxer.h"
#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegDemuxerThread.h"

#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegVideoStreamDecoder.h"
#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegAudioStreamDecoder.h"
#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegStreamsDecoderThread.h"

#include "DataTypes/QueueConcurrent.h"


namespace bv {


class FFmpegAVDecoder : public IAVDecoder
{
private:

    std::map< AVMediaType, FFmpegStreamDecoderUPtr >   m_streams;

	FFmpegDemuxerUPtr				        m_demuxer;
    FFmpegDemuxerThreadUPtr                 m_demuxerThread;
    
	FFmpegStreamsDecoderThreadUPtr          m_videoStreamsDecoderThread;
    FFmpegStreamsDecoderThreadUPtr          m_audioStreamsDecoderThread;

	AVDecoderThreadUPtr			            m_videoDecoderThread;
	AVDecoderThreadUPtr			            m_audioDecoderThread;

    UInt64                                  m_duration;

    bool                                    m_muted;

	SimpleTimer								m_timer;
	bool									m_paused;

public:

								FFmpegAVDecoder		    ( AVAssetConstPtr asset );
	virtual						~FFmpegAVDecoder		();

	virtual void				Play					() override;
	virtual void				Pause					() override;
    virtual void				Stop                    () override;

	virtual bool		        GetVideoMediaData		( AVMediaData & data ) override;
	virtual bool		        GetAudioMediaData		( AVMediaData & data ) override;
    virtual AVMediaData		    GetSingleFrame  		( TimeType frameTime ) override;

	virtual SizeType			GetVideoFrameSize	    () const override;

	virtual UInt32				GetWidth				() const override;
	virtual UInt32				GetHeight				() const override;

	virtual Int32				GetSampleRate			() const override;
    virtual AudioFormat			GetAudioFormat			() const override;
    
	virtual UInt64				GetDuration			    () const override;

    virtual bool                HasVideo                () const override;
    virtual bool                HasAudio                () const override;

	/** Accurate seeking of all attached streams.
	@param[time] in seconds 
    */
	virtual void				Seek					( Float64 time ) override;
	virtual void				FlushBuffers			() override;

	virtual bool				IsEOF					() const override;
	virtual bool				IsFinished				() const override;

    virtual void                Mute                    ( bool mute ) override;

    //FIXME: decode first video frame
    void                        ProcessFirstAVFrame     ();

protected:
	
    bool			            NextDataReady           ( AVMediaType type, UInt64 t, bool block );
    virtual bool			    NextDataReady           ( UInt64 t, bool block ) override;

private:

    void                        RestartDecoding         ();
    void                        StopDecoding            ();

    /** Manually seek from current keyframe to frame with at given timestamp.
    @param[time] in miliseconds
    */
    Int64				        Seek					( FFmpegStreamDecoder * decoder, Int64 time );

};

} //bv
