#include "stdafx.h"

#include "FFmpegAVDecoder.h"

#include "FFmpegUtils.h"


namespace bv {


// *********************************
//
FFmpegAVDecoder::FFmpegAVDecoder		( AVAssetConstPtr asset )
    : m_videoDecoder( nullptr )
    , m_audioDecoder( nullptr )
    , m_videoDecoderThread( nullptr )
    , m_audioDecoderThread( nullptr )
    , m_hasVideo( false )
    , m_hasAudio( false )
    , m_prevVideoPTS( 0 )
    , m_prevAudioPTS( 0 )
{
	auto path = asset->GetStreamPath();

	m_demuxer = std::unique_ptr< FFmpegDemuxer >( new FFmpegDemuxer( path ) );
    m_demuxerThread = std::unique_ptr< FFmpegDemuxerThread >( new FFmpegDemuxerThread( m_demuxer.get() ) );

    UInt64 videoDuration = 0, audioDuration = 0;
    if( asset->IsVideoEnabled() )
    {
        auto vstreamIdx = m_demuxer->GetStreamIndex( AVMEDIA_TYPE_VIDEO );
        if( vstreamIdx >= 0 )
        {
	        m_videoDecoder = std::unique_ptr< FFmpegVideoStreamDecoder >( new FFmpegVideoStreamDecoder( asset, m_demuxer->GetFormatContext(), vstreamIdx ) );
            videoDuration = m_videoDecoder->GetDuration();

            m_hasVideo = true;
        }
    }

    if( asset->IsAudioEnabled() )
    {
        auto astreamIdx = m_demuxer->GetStreamIndex( AVMEDIA_TYPE_AUDIO );
        if( astreamIdx >= 0 )
        {
            m_audioDecoder = std::unique_ptr< FFmpegAudioStreamDecoder >( new FFmpegAudioStreamDecoder( asset, m_demuxer->GetFormatContext(), astreamIdx ) );
            audioDuration = m_audioDecoder->GetDuration();
            
            m_hasAudio = true;
        }
    }

    m_duration = std::max( videoDuration, audioDuration );

	m_decoderThread = std::unique_ptr< AVDecoderThread >( new AVDecoderThread( this ) );
   
    m_demuxerThread->Start();

    if( m_hasVideo )
    {
        m_videoDecoderThread = std::unique_ptr< FFmpegVideoStreamDecoderThread >( new FFmpegVideoStreamDecoderThread( this, m_videoDecoder.get(), m_demuxer.get() ) );
        m_videoDecoderThread->Start();
    }

    if( m_hasAudio )
    {
        m_audioDecoderThread = std::unique_ptr< FFmpegAudioStreamDecoderThread >( new FFmpegAudioStreamDecoderThread( this, m_audioDecoder.get(), m_demuxer.get() ) );
        m_audioDecoderThread->Start();
    }

    m_decoderThread->Start();
}

// *********************************
//
FFmpegAVDecoder::~FFmpegAVDecoder		()
{
    assert( m_decoderThread );
	
	m_decoderThread->Kill();
	m_decoderThread->Join();
	m_decoderThread = nullptr;

    if( m_hasVideo )
    {
        m_videoDecoderThread->Kill();
        m_videoDecoderThread->Join();
        m_videoDecoderThread = nullptr;
    }

    if( m_hasAudio )
    {
        m_audioDecoderThread->Kill();
        m_audioDecoderThread->Join();
        m_audioDecoderThread = nullptr;
    }

	m_demuxerThread->Kill();
	m_demuxerThread->Join();
	m_demuxerThread = nullptr;

	//force 
	m_videoDecoder = nullptr;
    m_audioDecoder = nullptr;
	m_demuxer = nullptr;

	std::lock_guard< std::mutex > lock( m_mutex );

    FlushBuffers();
}

// *********************************
//
void						FFmpegAVDecoder::Play				()
{
    assert( m_decoderThread );

    m_decoderThread->Play();
}

// *********************************
//
void						FFmpegAVDecoder::Pause				()
{
    assert( m_decoderThread );
	
    m_decoderThread->Stop();
}

// *********************************
//
void						FFmpegAVDecoder::Stop				()
{
    Pause();
	Reset();
}

// *********************************
//
bool			            FFmpegAVDecoder::GetVideoMediaData	( AVMediaData & mediaData )
{
    return m_outVideoQueue.TryPop( mediaData );
}

// *********************************
//
bool			            FFmpegAVDecoder::GetAudioMediaData  ( AVMediaData & mediaData )
{
    return m_outAudioQueue.TryPop( mediaData );
}

// ***********************
// Jumps to frameTime. This function doesn't return to time before this function call.
AVMediaData		            FFmpegAVDecoder::GetSingleFrame  		( TimeType frameTime )
{
    frameTime;
    //Seek( frameTime );
    //FlushBuffers();
    //if( DecodeNextFrame() )
    //{
    //    NextVideoDataReady();
        //return GetVideoMediaData();
    //}

	//VideoMediaData mediaData;
	//mediaData.frameIdx = 0;
	//mediaData.frameData = nullptr;
 //   return mediaData;
    return AVMediaData();
}

// *********************************
//
bool				FFmpegAVDecoder::NextVideoDataReady	        ( UInt64 time )
{
    auto success = false;

    if( m_hasVideo )
    {
        if( time <= m_duration )
        {
            if( m_demuxerThread->Stopped() )
            {
                m_demuxerThread->Restart();
            }

            if( m_videoDecoderThread->Stopped() )
            {
                m_videoDecoderThread->Restart();
            }

            auto currVideoPTS = m_videoDecoder->GetCurrentPTS();
            while( !m_videoDecoder->IsDataQueueEmpty() &&
                ( currVideoPTS <= time ) &&
                m_prevVideoPTS <= currVideoPTS )
            {
                AVMediaData data;
                if( m_videoDecoder->GetData( data ) )
                {
                    m_outVideoQueue.Push( data );
                    m_prevVideoPTS = data.framePTS;
                    success = true;
                    currVideoPTS = m_videoDecoder->GetCurrentPTS();
                }
            }
        }
        else
        {
            m_prevVideoPTS = 0;
        }
    }

    return success;
}

// *********************************
//
bool				FFmpegAVDecoder::NextAudioDataReady		    ( UInt64 time )
{
    auto success = false;

    if( m_hasAudio )
    {
        if( time <= m_duration )
        {
            if( m_demuxerThread->Stopped() )
            {
                m_demuxerThread->Restart();
            }

            if( m_audioDecoderThread->Stopped() )
            {
                m_audioDecoderThread->Restart();
            }

            auto currAudioPTS = m_audioDecoder->GetCurrentPTS();
            while( !m_audioDecoder->IsDataQueueEmpty() &&
                ( currAudioPTS <= time ) &&
                m_prevAudioPTS <= currAudioPTS )
            {
                AVMediaData data;
                if( m_audioDecoder->GetData( data ) )
                {
                    m_outAudioQueue.Push( data );
                    m_prevAudioPTS = data.framePTS;
                    success = true;
                    currAudioPTS = m_videoDecoder->GetCurrentPTS();
                }
            }
        }
        else
        {
            m_prevAudioPTS = 0;
        }
    }

    return success;
}

// *********************************
//
SizeType				FFmpegAVDecoder::GetVideoFrameSize		    () const
{
    if( m_hasVideo )
    {
        return m_videoDecoder->GetFrameSize();
    }
    return 0;
}

// *********************************
//
UInt32					FFmpegAVDecoder::GetWidth				() const
{
    if( m_hasVideo )
    {
        return m_videoDecoder->GetWidth();
    }
    return 0;
}

// *********************************
//
UInt32					FFmpegAVDecoder::GetHeight				() const
{
    if( m_hasVideo )
	{
        return m_videoDecoder->GetHeight();
    }
    return 0;
}

// *********************************
//
Int32				    FFmpegAVDecoder::GetSampleRate			() const
{
    if( m_hasAudio )
	{
        return m_audioDecoder->GetSampleRate();
    }

    return 0;
}

// *********************************
//
AudioFormat				FFmpegAVDecoder::GetAudioFormat			() const
{
    if( m_hasAudio )
	{
        return m_audioDecoder->GetFormat();
    }

    return AudioFormat::AF_TOTAL;
}

// *********************************
//
UInt64				    FFmpegAVDecoder::GetDuration		    () const
{
    return m_duration;
}

// *********************************
//
bool					FFmpegAVDecoder::HasVideo			    () const
{
    return m_hasVideo ;
}

// *********************************
//
bool					FFmpegAVDecoder::HasAudio			    () const
{
    return m_hasAudio;
}

// *********************************
//
void					FFmpegAVDecoder::Seek					( Float64 time ) 
{
	std::lock_guard< std::mutex > lock( m_mutex );
    
    auto stopped = m_demuxerThread->Stopped();
    if( !stopped )
    {
        m_demuxerThread->Stop();
        while( !m_demuxerThread->Stopped() );
    }

    if( m_hasVideo )
    {
        auto ts = m_videoDecoder->ConvertTime( time );
	    m_demuxer->Seek( ts, m_videoDecoder->GetStreamIdx() );

        Int64 currTs = 0;
        while( currTs < ts )
        {
            m_demuxer->ProcessPacket();
            auto packet = m_demuxer->GetPacket( m_videoDecoder->GetStreamIdx() );
            if( packet != nullptr )
            {
                currTs = packet->dts;
	            m_videoDecoder->DecodePacket( packet );
            }
            else if( m_demuxer->IsEOF() )
            {
                break;
            }
        }
    }

    if( m_hasAudio )
    {
	    m_demuxer->Seek( 0, m_audioDecoder->GetStreamIdx() );
    }

    if( !stopped )
    {
        m_demuxerThread->Restart();
    }
}

// *********************************
//
void					FFmpegAVDecoder::FlushBuffers			() 
{
	m_outVideoQueue.Clear();
	m_outAudioQueue.Clear();
}

// *********************************
//
void					FFmpegAVDecoder::Reset					() 
{
	{
        std::lock_guard< std::mutex > lock( m_mutex );
	    m_demuxer->Reset();

        if( m_hasVideo )
        {
	        m_videoDecoder->Reset();
        }

        if( m_hasAudio )
        {
	        m_audioDecoder->Reset();
        }

        FlushBuffers();
    }

    Seek( 0.f );
}

// *********************************
//
bool					FFmpegAVDecoder::IsEOF					() const
{
	std::lock_guard< std::mutex > lock( m_mutex );
    return m_demuxer->IsEOF();
}

// *********************************
//
bool					FFmpegAVDecoder::IsPacketQueueEmpty		( Int32 streamIdx ) const
{
	std::lock_guard< std::mutex > lock( m_mutex );
    return m_demuxer->IsPacketQueueEmpty( streamIdx ) && m_videoDecoder->IsDataQueueEmpty();
}

// *********************************
//
bool					FFmpegAVDecoder::IsFinished				() const
{
    bool finished = true;
    finished &= IsEOF();

    if( m_hasVideo )
    {
        finished &= m_demuxer->IsPacketQueueEmpty( m_videoDecoder->GetStreamIdx() );
        finished &= m_videoDecoder->IsDataQueueEmpty();
        finished &= m_outVideoQueue.Size() == 1;
    }

    if( m_hasAudio )
    {
        finished &= m_demuxer->IsPacketQueueEmpty( m_audioDecoder->GetStreamIdx() );
        //finished &= m_audioDecoder->IsDataQueueEmpty();
    }



    return finished;
}

} //bv