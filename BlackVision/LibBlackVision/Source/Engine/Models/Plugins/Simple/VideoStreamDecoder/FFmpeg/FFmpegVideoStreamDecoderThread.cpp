#include "stdafx.h"

#include "FFmpegVideoStreamDecoderThread.h"

#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegVideoDecoder.h"
#include "Tools/HRTimer.h"

#include <thread>
#include <chrono>

namespace bv
{

// *******************************
//
FFmpegVideoStreamDecoderThread::FFmpegVideoStreamDecoderThread				( FFmpegVideoDecoder * decoder, FFmpegVideoStreamDecoder * videoDecoder, FFmpegDemuxer * demuxer )
	: m_decoder( decoder )
    , m_videoDecoder( videoDecoder )
    , m_demuxer( demuxer )
	, m_stopped( false )
    , m_running( true )
{
}

// *******************************
//
FFmpegVideoStreamDecoderThread::~FFmpegVideoStreamDecoderThread				()
{
}

// *******************************
//
void				FFmpegVideoStreamDecoderThread::Kill	    ()
{
    {
		std::unique_lock< std::mutex > lock( m_mutex );
		m_running = false;
        m_stopped = false;
	}
	m_cond.notify_one();
}

// *******************************
//
void				FFmpegVideoStreamDecoderThread::Restart	    ()
{
	{
		std::unique_lock< std::mutex > lock( m_mutex );
        m_stopped = false;
	}
	m_cond.notify_one();
}

// *******************************
//
void				FFmpegVideoStreamDecoderThread::Stop		()
{
	{
		std::unique_lock< std::mutex > lock( m_mutex );
		m_stopped = true;
	}
	m_cond.notify_one();
}

// *******************************
//
bool				FFmpegVideoStreamDecoderThread::Stopped		() const
{
	std::unique_lock< std::mutex > lock( m_mutex );
	return m_stopped;
}

// *******************************
//
void				FFmpegVideoStreamDecoderThread::Run			()
{
	{
		std::unique_lock< std::mutex > lock( m_mutex );
        m_stopped = true;
		m_running = true;
	}

    while( true )
    {
		std::unique_lock< std::mutex > lock( m_mutex );

        if( !m_running )
        {
            break;
        }

        if( !m_videoDecoder->ProcessPacket( m_demuxer ) )
        {
            m_stopped = true;
        }

        if( m_stopped )
		{
			while( m_stopped )
			{
				m_cond.wait( lock );
			}
		}
    }
}

} //bv