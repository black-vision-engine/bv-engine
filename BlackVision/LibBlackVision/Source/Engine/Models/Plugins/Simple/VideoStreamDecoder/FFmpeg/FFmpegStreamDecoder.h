#pragma once

#include "FFmpegDef.h"

#include "Assets/VideoStream/AVAsset.h"


namespace bv {


class FFmpegStreamDecoder
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

    virtual Int32           GetStreamIdx                () const = 0;
    
};

DEFINE_UPTR_TYPE( FFmpegStreamDecoder )

} //bv
