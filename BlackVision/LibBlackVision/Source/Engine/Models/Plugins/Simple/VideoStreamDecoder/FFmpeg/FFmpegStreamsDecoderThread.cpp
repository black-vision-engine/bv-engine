#include "stdafx.h"

#include "FFmpegStreamsDecoderThread.h"

#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegAVDecoder.h"
#include "Tools/HRTimer.h"

#include <thread>
#include <chrono>


namespace bv {


// *******************************
//
FFmpegStreamsDecoderThread::FFmpegStreamsDecoderThread          ( FFmpegStreamDecoder * videoStreamDecoder, FFmpegStreamDecoder * audioStreamDecoder, FFmpegDemuxer * demuxer )
	: m_videoStreamDecoder( videoStreamDecoder )
    , m_audioStreamDecoder( audioStreamDecoder )
    , m_demuxer( demuxer )
	, m_stopped( false )
    , m_running( true )
{
}

// *******************************
//
FFmpegStreamsDecoderThread::~FFmpegStreamsDecoderThread         ()
{
}

// *******************************
//
void				FFmpegStreamsDecoderThread::Kill	        ()
{
	std::unique_lock< std::mutex > lock( m_mutex );
	m_running = false;
    m_stopped = false;
	m_cond.notify_one();
}

// *******************************
//
void				FFmpegStreamsDecoderThread::Restart	    ()
{
	std::unique_lock< std::mutex > lock( m_mutex );
	m_stopped = false;
    m_cond.notify_one();
}

// *******************************
//
void				FFmpegStreamsDecoderThread::Stop		()
{
	std::unique_lock< std::mutex > lock( m_mutex );
	m_stopped = true;
	m_cond.notify_one();
}

// *******************************
//
bool				FFmpegStreamsDecoderThread::Stopped		() const
{
	return m_stopped;
}

// *******************************
//
void				FFmpegStreamsDecoderThread::Run			()
{
    while( m_running )
    {
        {
            std::unique_lock< std::mutex > lock( m_mutex );
            while( m_stopped )
            {
                m_cond.wait( lock );
            }
        }

        if( m_videoStreamDecoder )
        {
            m_videoStreamDecoder->ProcessPacket( m_demuxer );
        }

        if( m_audioStreamDecoder )
        {
            m_audioStreamDecoder->ProcessPacket( m_demuxer );
        }
    }
}

} //bv