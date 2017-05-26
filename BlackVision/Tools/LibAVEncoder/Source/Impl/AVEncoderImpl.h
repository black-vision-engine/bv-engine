#pragma once 
#include "CoreDEF.h"
#include "AVEncoder.h"
#include "AVEncoderThread.h"
#include "Memory/AVFrame.h" 

#include "FFmpegUtils.h"

#include <boost/circular_buffer.hpp>
#include <mutex>

namespace bv 
{

namespace avencoder
{

class AVEncoder;
class AVEncoder::Impl;

class AVEncoder::Impl
{
    std::unique_ptr< AVEncoderThread >          m_encoderThread;
    boost::circular_buffer< AVFramePtr >        m_avFramesBuffer;

    ::AVFrame *             m_AVFrame;
    ::AVFormatContext *     m_AVContext;
    FILE *                  m_file;
    OutputStream *          m_video_st;
    OutputStream *          m_audio_st;
    UInt32                  m_frameBufferSize;

    std::mutex              m_mutex;

private:
    Impl( const Impl & copy );
    const AVEncoder & operator=( const Impl & copy );

    AVFramePtr              GetFrameBuffer  ();
    void                    FrameWritten    ( const AVFrameConstPtr & frame );

public:
    Impl           ( UInt32 frameBufferSize );
    virtual ~Impl  ();

    bool            OpenOutputStream    (   const std::string & outputFilePath,
                                            VideoOptions vOps,
                                            AudioOptions aOps,
                                            bool enableVideo = true,
                                            bool enableAudio = true );
    void            CloseStream         ();

    bool            WriteFrame          ( const AVFrameConstPtr & frame );
};

} // videoencoder 

} // bv
