#pragma once

#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegStreamDecoder.h"

#include "Assets/VideoStream/AVAsset.h"
#include "DataTypes/QueueConcurrent.h"


namespace bv {
    
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

public:

                            FFmpegVideoStreamDecoder    ( AVAssetConstPtr asset, AVFormatContext * formatCtx, Int32 streamIdx, UInt32 maxQueueSize = 5 );
                            ~FFmpegVideoStreamDecoder   ();

    SizeType                GetFrameSize                () const;   

    UInt32                  GetWidth                    () const;   
    UInt32                  GetHeight                   () const;   
    Float64                 GetFrameRate                () const;   

    virtual bool            ProcessPacket               ( FFmpegDemuxer * demuxer ) override;

    virtual bool            DecodePacket                ( AVPacket * packet ) override;
    virtual AVMediaData     ConvertFrame                () override;

};

} //bv
