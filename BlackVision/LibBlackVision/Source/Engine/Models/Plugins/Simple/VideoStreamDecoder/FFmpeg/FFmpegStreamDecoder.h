#pragma once

#include "FFmpegDef.h"

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

    Int32                               m_streamIdx;

    AVFrame *                           m_frame;

	QueueConcurrent< AVMediaData >      m_bufferQueue;

    UInt32                              m_maxQueueSize;

    UInt64                              m_duration;

    /** Starting frame timestamp (in stream time base), updated on seeking. */
    UInt64                              m_offset;

public:

    explicit                FFmpegStreamDecoder         ( AVFormatContext * formatCtx, Int32 streamIdx, UInt32 maxQueueSize );
    virtual                 ~FFmpegStreamDecoder        ();

    UInt64                  GetDuration                 () const;   

    virtual UInt64          GetCurrentPTS               ();
    virtual bool            PopData                     ( AVMediaData & data );
	bool					IsDataQueueEmpty		    () const;

	virtual void			Reset						();

    virtual bool            ProcessPacket               ( FFmpegDemuxer * demuxer ) = 0;

    virtual bool            DecodePacket                ( AVPacket * packet ) = 0;
    virtual AVMediaData     ConvertFrame                () = 0;

    void                    SetOffset                   ( UInt64 offset );
    UInt64                  GetOffset                   () const;

    Int32                   GetStreamIdx                () const;

    /** Converts time from seconds to the stream specific time base timestamp */
    Int64                   ConvertTime                 ( Float64 time );

};

DEFINE_UPTR_TYPE( FFmpegStreamDecoder )

} //bv
