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
#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegStreamDecoderThread.h"

#include "DataTypes/QueueConcurrent.h"


namespace bv {


class FFmpegAVDecoder : public IAVDecoder
{
private:

    struct StreamData {
    
        FFmpegStreamDecoderUPtr         decoder;
        FFmpegStreamDecoderThreadUPtr   decoderThread;
        QueueConcurrent< AVMediaData >	outQueue;
        UInt64                          prevPTS;

    };

private:

	FFmpegDemuxerUPtr				    m_demuxer;

    std::map< AVMediaType, StreamData * > m_streams;

	//FFmpegVideoStreamDecoderUPtr	    m_videoDecoder;
	//FFmpegAudioStreamDecoderUPtr        m_audioDecoder;
	
    UInt64                              m_duration;

	AVDecoderThreadUPtr			        m_decoderThread;

    FFmpegDemuxerThreadUPtr             m_demuxerThread;

	//FFmpegStreamDecoderThreadUPtr       m_videoDecoderThread;
	//FFmpegStreamDecoderThreadUPtr       m_audioDecoderThread;

	//QueueConcurrent< AVMediaData >	    m_outVideoQueue;
	//QueueConcurrent< AVMediaData > 	    m_outAudioQueue;

 //   UInt64                              m_prevVideoPTS;
 //   UInt64                              m_prevAudioPTS;

public:

								FFmpegAVDecoder		    ( AVAssetConstPtr asset );
	virtual						~FFmpegAVDecoder		();

	virtual void				Play					() override;
	virtual void				Pause					() override;
	virtual void				Stop					() override;

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

	/** Accurate seeking.
	@param[time] in seconds 
    */
	virtual void				Seek					( Float64 time ) override;
	virtual void				FlushBuffers			() override;

	virtual void				Reset					() override;

	virtual bool				IsEOF					() const override;
	virtual bool				IsFinished				() const override;

protected:
	
	virtual bool				NextVideoDataReady		( UInt64 t ) override;
	virtual bool		        NextAudioDataReady		( UInt64 t ) override;

private:

	void				        Seek					( FFmpegStreamDecoder * decoder, Float64 time );
	bool				        NextStreamDataReady		( AVMediaType type, UInt64 t );

};

} //bv
