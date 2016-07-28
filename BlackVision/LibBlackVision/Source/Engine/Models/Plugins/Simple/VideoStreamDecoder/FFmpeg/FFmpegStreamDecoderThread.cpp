#include "stdafx.h"

#include "FFmpegStreamDecoderThread.h"

#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegAVDecoder.h"
#include "Tools/HRTimer.h"

#include <thread>
#include <chrono>

namespace bv
{

// *******************************
//
FFmpegStreamDecoderThread::FFmpegStreamDecoderThread				( FFmpegAVDecoder * decoder, FFmpegStreamDecoder * streamDecoder, FFmpegDemuxer * demuxer )
	: m_decoder( decoder )
    , m_streamDecoder( streamDecoder )
    , m_demuxer( demuxer )
	, m_stopped( false )
    , m_running( true )
{
}

// *******************************
//
FFmpegStreamDecoderThread::~FFmpegStreamDecoderThread				()
{
}

// *******************************
//
void				FFmpegStreamDecoderThread::Kill	    ()
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
void				FFmpegStreamDecoderThread::Restart	    ()
{
	std::unique_lock< std::mutex > lock( m_mutex );
    if( m_stopped )
	{
		m_stopped = false;
	    m_cond.notify_one();
	}
}

// *******************************
//
void				FFmpegStreamDecoderThread::Stop		()
{
	{
		std::unique_lock< std::mutex > lock( m_mutex );
		m_stopped = true;
	}
	m_cond.notify_one();
}

// *******************************
//
bool				FFmpegStreamDecoderThread::Stopped		() const
{
	std::unique_lock< std::mutex > lock( m_mutex );
	return m_stopped;
}

// *******************************
//
void				FFmpegStreamDecoderThread::Run			()
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

        if( m_stopped )
		{
			while( m_stopped )
			{
				m_cond.wait( lock );
			}
		}

        if( !m_streamDecoder->ProcessPacket( m_demuxer ) )
        {
            m_stopped = true;
        }
    }
}

} //bv