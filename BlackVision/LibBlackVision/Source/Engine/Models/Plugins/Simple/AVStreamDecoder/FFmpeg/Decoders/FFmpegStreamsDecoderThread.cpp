#include "stdafx.h"

#include "FFmpegStreamsDecoderThread.h"

#include "UseLoggerLibBlackVision.h"


namespace bv {


// *******************************
//
FFmpegStreamsDecoderThread::FFmpegStreamsDecoderThread          ( FFmpegStreamDecoder * streamDecoder )
	: m_streamDecoder( streamDecoder )
	, m_stopped( false )
	, m_stopThread( false )
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

	LOG_MESSAGE( SeverityLevel::debug )
		<< "KILLING Decoder thread stream id: "
		<< m_streamDecoder->GetStreamIdx()
		<< " thread id: "
		<< std::this_thread::get_id();

	if( m_streamDecoder )
		m_streamDecoder->Reset();
	
	m_cond.notify_one();
}

// *******************************
//
void				FFmpegStreamsDecoderThread::Restart	    ()
{
	std::unique_lock< std::mutex > lock( m_mutex );
	if( m_stopped || m_stopThread )
	{
		LOG_MESSAGE( SeverityLevel::debug ) 
			<< "RESTARTING Decoder thread stream id: " 
			<< m_streamDecoder->GetStreamIdx() 
			<< " thread id: " 
			<< std::this_thread::get_id() << std::endl;

		m_stopThread = false;
		m_stopped = false;
		m_cond.notify_one();
	}
}

// *******************************
//
void				FFmpegStreamsDecoderThread::Stop		()
{
	std::unique_lock< std::mutex > lock( m_mutex );

	if( !m_stopThread )
	{
		LOG_MESSAGE( SeverityLevel::debug ) 
			<< "STOPPING Decoder thread strema id: " 
			<< m_streamDecoder->GetStreamIdx() 
			<< " thread id: " 
			<< std::this_thread::get_id();

		m_stopThread = true;
		m_cond.notify_one();
	}
}

// *******************************
//
bool				FFmpegStreamsDecoderThread::Stopped		() const
{
	std::unique_lock< std::mutex > lock( m_mutex );
	return m_stopped;
}

// *******************************
//
void				FFmpegStreamsDecoderThread::Run			()
{
	LOG_MESSAGE( SeverityLevel::debug ) 
		<< "STARTING Decoder thread " 
		<< std::this_thread::get_id();


	while( true )
	{
        {
            std::unique_lock< std::mutex > lock( m_mutex );

            if( m_stopThread )
            {
                LOG_MESSAGE( SeverityLevel::debug )
                    << "STOPPED Decoder thread strema id: "
                    << m_streamDecoder->GetStreamIdx()
                    << " thread id: "
                    << std::this_thread::get_id();

                m_stopped = true;
                m_cond.wait( lock, [ = ]
                {
                    return m_stopped == false;
                } );

                LOG_MESSAGE( SeverityLevel::debug )
                    << "STARTED Decoder thread strema id: "
                    << m_streamDecoder->GetStreamIdx()
                    << " thread id: "
                    << std::this_thread::get_id();
            }

            if( !m_running )
            {
                break;
            }
        }

        try
        {
            if( m_streamDecoder )
            {
                m_streamDecoder->ProcessPacket( true );
            }

        }
        catch( const std::runtime_error & exc )
        {
            LOG_MESSAGE( SeverityLevel::error )
                << "Streams decoder error. Stream id: "
                << m_streamDecoder->GetStreamIdx()
                << ". THREAD ID: " << std::this_thread::get_id() << std::endl
                << exc.what();
        }
    }


	LOG_MESSAGE( SeverityLevel::debug ) 
		<< "DYING Decoder thread " 
		<< std::this_thread::get_id();
}

} //bv