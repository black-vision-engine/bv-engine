#pragma once

#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegStreamDecoder.h"
#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/Interfaces/IAVDefs.h"

#include "Assets/VideoStream/AVAsset.h"
#include "DataTypes/QueueConcurrent.h"


namespace bv {
    
class FFmpegDemuxer;

class FFmpegVideoStreamDecoder : public FFmpegStreamDecoder
{
private:

    AVFrame *                           m_outFrame;
    uint8_t *                           m_outBuffer;

    SizeType                            m_frameSize;

    SwsContext *                        m_swsCtx;

    UInt32                              m_width;
    UInt32                              m_height;
    Float64                             m_frameRate;
    UInt64                              m_duration;

    QueueConcurrent< AVMediaData >      m_bufferQueue;

    UInt32                              m_maxQueueSize;

public:

                            FFmpegVideoStreamDecoder    ( AVAssetConstPtr asset, AVFormatContext * formatCtx, Int32 streamIdx, UInt32 maxQueueSize = 5 );
                            ~FFmpegVideoStreamDecoder   ();

    SizeType                GetFrameSize                () const;   

    UInt32                  GetWidth                    () const;   
    UInt32                  GetHeight                   () const;   
    Float64                 GetFrameRate                () const;   
    UInt64                  GetDuration                 () const;   
    
    UInt64                  GetCurrentPTS               ();

    bool                    GetData                     ( AVMediaData & data );

    bool                    ProcessPacket               ( FFmpegDemuxer * demuxer );

    /** Converts time from seconds to the stream specific time base timestamp */
    Int64                   ConvertTime                 ( Float64 time );

    bool                    DecodePacket                ( AVPacket * packet );
    virtual AVMediaData     ConvertFrame                ();

    void                    Reset                       ();
    void                    Clear                       ();

    virtual Int32           GetStreamIdx                () const override;

	bool					IsDataQueueEmpty		    () const;
    
};

DEFINE_UPTR_TYPE( FFmpegVideoStreamDecoder )

} //bv
