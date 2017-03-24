#pragma once

#include "Engine/Models/Plugins/Simple/AVStreamDecoder/FFmpeg/FFmpegDef.h"

#include "DataTypes/QueueConcurrentLimited.h"
#include "DataTypes/QueueConcurrent.h"

#include "Assets/AVStream/AVAsset.h"
#include "Engine/Models/Plugins/Simple/AVStreamDecoder/Interfaces/IAVDefs.h"


namespace bv {

class FFmpegDemuxer;

class FFmpegStreamDecoder
{
protected:

    AVStream *                          m_stream;
    AVCodecContext *                    m_codecCtx;
    AVCodec *                           m_codec;

	FFmpegDemuxer *						m_demuxer;

    Int32                               m_streamIdx;

    AVFrame *                           m_frame;

    UInt64                              m_duration;
    UInt64                              m_prevPTS;

    QueueConcurrentLimited< AVMediaData >      m_bufferQueue;
	QueueConcurrentLimited< AVMediaData >	   m_outQueue;

    /** Starting frame timestamp (in stream time base), updated on seeking. */
    UInt64                              m_offset;

	std::atomic< bool >					m_interruptWait;

public:
    explicit                FFmpegStreamDecoder         ( AVFormatContext * formatCtx, Int32 streamIdx, UInt32 maxQueueSize, FFmpegDemuxer * demuxer );
    virtual                 ~FFmpegStreamDecoder        ();

	Int32                   GetStreamIdx                () const;
	UInt64                  GetDuration                 () const;
	void					FinishQueue					();
	bool                    NextDataReady               ( UInt64 time, bool block );
	void                    UploadData                  ();
	bool					IsFinished					() const;

	/** Converts time from seconds to the stream specific time base timestamp */
	Int64                   ConvertTime                 ( Float64 time );

	void                    SetOffset                   ( UInt64 offset );
	UInt64                  GetOffset                   () const;

	bool					IsDataQueueEmpty            () const;
	bool					IsOutQueueEmpty		        () const;
	void					ClearDataQueue				();
	void					ClearOutQueue				();

	void					EnqueueDummyDataMessage		();
	void					SetWaitingInterrupt			();

	virtual void			Reset                       ();
	virtual bool            ProcessPacket               ( bool block = false );
	virtual bool            PopData                     ( AVMediaData & data );
	virtual UInt64          GetCurrentPTS               ();
    virtual UInt64          GetLastPlayedFramePTS       () const;
    virtual bool            IsAudio                     () const = 0;

private:

    virtual bool            DecodePacket                ( AVPacket * packet );
    virtual AVMediaData     ConvertFrame                () = 0;
};

DEFINE_UPTR_TYPE( FFmpegStreamDecoder )

} //bv
