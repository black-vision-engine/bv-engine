#include "stdafx.h"

#include "FFmpegAudioStreamDecoderThread.h"

#include "Engine/Models/Plugins/Simple/VideoStreamDecoder/FFmpeg/FFmpegVideoDecoder.h"
#include "Tools/HRTimer.h"

#include <thread>
#include <chrono>

namespace bv
{

// *******************************
//
FFmpegAudioStreamDecoderThread::FFmpegAudioStreamDecoderThread				( FFmpegVideoDecoder * decoder, FFmpegAudioStreamDecoder * audioDecoder, FFmpegDemuxer * demuxer )
	: m_decoder( decoder )
    , m_audioDecoder( audioDecoder )
    , m_demuxer( demuxer )
	, m_stopped( false )
    , m_running( true )
{
}

// *******************************
//
FFmpegAudioStreamDecoderThread::~FFmpegAudioStreamDecoderThread				()
{
    m_bufferQueue.Clear();
}

// *******************************
//
void				FFmpegAudioStreamDecoderThread::Kill	    ()
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
void				FFmpegAudioStreamDecoderThread::Restart	    ()
{
	{
		std::unique_lock< std::mutex > lock( m_mutex );
        m_stopped = false;
	}
	m_cond.notify_one();
}

// *******************************
//
void				FFmpegAudioStreamDecoderThread::Stop		()
{
	{
		std::unique_lock< std::mutex > lock( m_mutex );
		m_stopped = true;
	}
	m_cond.notify_one();
}

// *******************************
//
bool				FFmpegAudioStreamDecoderThread::Stopped		() const
{
	std::unique_lock< std::mutex > lock( m_mutex );
	return m_stopped;
}

// *******************************
//
void				FFmpegAudioStreamDecoderThread::Run			()
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

        if( !m_audioDecoder->ProcessPacket( m_demuxer ) )
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