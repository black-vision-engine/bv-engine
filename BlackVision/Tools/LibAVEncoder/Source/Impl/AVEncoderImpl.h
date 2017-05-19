#pragma once 
#include "CoreDEF.h"
#include "AVEncoder.h"
#include "AVEncoderThread.h"
#include "Memory/AVFrame.h" 

#include "FFmpegUtils.h"

namespace bv 
{

namespace avencoder
{

class AVEncoder;
class AVEncoder::Impl;

// a wrapper around a single output AVStream
struct OutputStream {
	::AVStream *st;
	AVCodecContext *enc;
	/* pts of the next frame that will be generated */
	int64_t next_pts;
	int samples_count;
	::AVFrame *frame;
	::AVFrame *tmp_frame;
	float t, tincr, tincr2;
	struct SwsContext *sws_ctx;
	struct SwrContext *swr_ctx;
};

class AVEncoder::Impl
{
    std::unique_ptr< AVEncoderThread > m_encoderThread;

    ::AVFrame *				m_AVFrame;
    ::AVFormatContext *		m_AVContext;
    FILE *					m_file;
	OutputStream			m_video_st;
	OutputStream			m_audio_st;


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
