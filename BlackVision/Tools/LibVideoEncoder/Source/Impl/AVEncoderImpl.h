#pragma once 
#include "CoreDEF.h"
#include "AVEncoder.h"
#include "AVEncoderThread.h"
#include "Memory/AVFrame.h" 

#pragma warning(push)
#pragma warning(disable : 4244) // warning C4244: 'return' : conversion from 'int' to 'uint8_t', possible loss of data

#include "libavformat/avformat.h"
#include "libswscale/swscale.h"

#pragma warning(pop)

#include "FFmpegUtils.h"

namespace bv 
{

namespace avencoder
{

class AVEncoder;
class AVEncoder::Impl;


class AVEncoder::Impl
{
    std::unique_ptr< AVEncoderThread > m_encoderThread;

    ::AVFrame *         m_AVFrame;
    ::AVCodecContext *  m_AVContext;
    FILE *              m_file;


private:
    Impl( const Impl & copy );
    const AVEncoder & operator=( const Impl & copy );

public:
    Impl           ();
    virtual ~Impl  ();

    bool            OpenOutputStream    ( const std::string & outputFilePath, bool enableVideo = true, bool enableAudio = true );
    void            CloseStream         ();

    bool            WriteFrame          ( const AVFramePtr & frame );
};

} // videoencoder 

} // bv
