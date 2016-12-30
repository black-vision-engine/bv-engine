#pragma once

#include "FFmpegDef.h"

#include "DataTypes/QueueConcurrentLimited.h"
#include "DataTypes/QueueConcurrent.h"

#include "Assets/VideoStream/AVAsset.h"
#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/Interfaces/IAVDefs.h"


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
	QueueConcurrent< AVMediaData >	   m_outQueue;

    /** Starting frame timestamp (in stream time base), updated on seeking. */
    UInt64                              m_offset;

	std::atomic< bool >					m_interruptWait;

public:

    explicit                FFmpegStreamDecoder         ( AVFormatContext * formatCtx, Int32 streamIdx, UInt32 maxQueueSize, FFmpegDemuxer * demuxer );
    virtual                 ~FFmpegStreamDecoder        ();

    UInt64                  GetDuration                 () const;   

    virtual UInt64          GetCurrentPTS               ();

    void                    UploadData                  ();
    virtual bool            PopData                     ( AVMediaData & data );
    bool					IsDataQueueEmpty            () const;
	void					ClearDataQueue				();

    bool					IsOutQueueEmpty		        () const;
	void					ClearOutQueue				();

    virtual void			Reset                       ();

    virtual bool            ProcessPacket               ( bool block = false );

    virtual bool            DecodePacket                ( AVPacket * packet );
    virtual AVMediaData     ConvertFrame                () = 0;

    void                    SetOffset                   ( UInt64 offset );
    UInt64                  GetOffset                   () const;

    Int32                   GetStreamIdx                () const;

    /** Converts time from seconds to the stream specific time base timestamp */
    Int64                   ConvertTime                 ( Float64 time );

    bool                    NextDataReady               ( UInt64 time, bool block );

	void					FinishQueue					();

	bool					IsFinished					() const;

	void					SetWaitingInterrupt			();
	void					EnqueueDummyDataMessage		();
};

DEFINE_UPTR_TYPE( FFmpegStreamDecoder )

} //bv
