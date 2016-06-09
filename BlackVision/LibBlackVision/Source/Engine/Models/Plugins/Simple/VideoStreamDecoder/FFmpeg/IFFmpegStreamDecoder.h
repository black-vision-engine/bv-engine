#pragma once

#include "FFmpegDef.h"

#include "Assets/VideoStream/VideoStreamAsset.h"


namespace bv {


class IFFmpegStreamDecoder
{
protected:

    AVStream *              m_stream;
    AVCodecContext *        m_codecCtx;
    AVCodec *               m_codec;

    Int32                   m_streamIdx;

    AVFrame *               m_frame;
    AVFrame *               m_outFrame;
    uint8_t *               m_outBuffer;

    SizeType                m_frameSize;

public:

    virtual bool            DecodePacket                ( AVPacket * packet ) = 0;

    virtual Int32           GetStreamIdx                () const = 0;
    
};

DEFINE_UPTR_TYPE( IFFmpegStreamDecoder )

} //bv
