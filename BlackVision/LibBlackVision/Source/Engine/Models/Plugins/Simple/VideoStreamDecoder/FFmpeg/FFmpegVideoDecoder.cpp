#include "stdafx.h"

#include "FFmpegVideoDecoder.h"

#include "FFmpegUtils.h"


namespace bv {


// *********************************
//
FFmpegVideoDecoder::FFmpegVideoDecoder		( VideoStreamAssetConstPtr asset )
    : m_videoDecoder( nullptr )
    , m_audioDecoder( nullptr )
    , m_videoDecoderThread( nullptr )
    , m_audioDecoderThread( nullptr )
    , m_hasVideo( false )
    , m_hasAudio( false )
{
	auto path = asset->GetStreamPath();

	m_demuxer = std::unique_ptr< FFmpegDemuxer >( new FFmpegDemuxer( path ) );
    m_demuxerThread = std::unique_ptr< FFmpegDemuxerThread >( new FFmpegDemuxerThread( m_demuxer.get() ) );

    auto vstreamIdx = m_demuxer->GetStreamIndex( AVMEDIA_TYPE_VIDEO );
    if( vstreamIdx >= 0 )
    {
	    m_videoDecoder = std::unique_ptr< FFmpegVideoStreamDecoder >( new FFmpegVideoStreamDecoder( asset, m_demuxer->GetFormatContext(), vstreamIdx ) );
        m_hasVideo = true;
    }

    auto astreamIdx = m_demuxer->GetStreamIndex( AVMEDIA_TYPE_AUDIO );
    if( astreamIdx >= 0 )
    {
        m_audioDecoder = std::unique_ptr< FFmpegAudioStreamDecoder >( new FFmpegAudioStreamDecoder( asset, m_demuxer->GetFormatContext(), astreamIdx ) );
        m_hasAudio = true;
    }

	m_decoderThread = std::unique_ptr< VideoDecoderThread >( new VideoDecoderThread( this ) );
   
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
    //m_decoderThread->Stop();
}

// *********************************
//
FFmpegVideoDecoder::~FFmpegVideoDecoder		()
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
void						FFmpegVideoDecoder::Play				()
{
    assert( m_decoderThread );

    m_decoderThread->Play();
}

// *********************************
//
void						FFmpegVideoDecoder::Pause				()
{
    assert( m_decoderThread );
	
    m_decoderThread->Stop();
}

// *********************************
//
void						FFmpegVideoDecoder::Stop				()
{
    Pause();
	Reset();
}

// *********************************
//
VideoMediaData			FFmpegVideoDecoder::GetVideoMediaData		()
{
	VideoMediaData mediaData;
    m_outVideoQueue.Front( mediaData );
    return mediaData;
}

// *********************************
//
AudioMediaData			FFmpegVideoDecoder::GetAudioMediaData		()
{
	AudioMediaData mediaData;
    m_outAudioQueue.Front( mediaData );
    return mediaData;
}

// ***********************
// Jumps to frameTime. This function doesn't return to time before this function call.
VideoMediaData		    FFmpegVideoDecoder::GetSingleFrame  		( TimeType frameTime )
{
    frameTime;
    //Seek( frameTime );
    //FlushBuffers();
    //if( DecodeNextFrame() )
    //{
    //    NextVideoDataReady();
        return GetVideoMediaData();
    //}

	//VideoMediaData mediaData;
	//mediaData.frameIdx = 0;
	//mediaData.frameData = nullptr;
 //   return mediaData;
}

// *********************************
//
bool					FFmpegVideoDecoder::NextVideoDataReady		()
{
    if( m_hasVideo )
    {
        if( m_demuxerThread->Stopped() )
        {
            m_demuxerThread->Restart();
        }

        if( m_videoDecoderThread->Stopped() )
        {
            m_videoDecoderThread->Restart();
        }

        VideoMediaData videoMediaData;
        if( m_videoDecoder->GetData( videoMediaData ) )
        {
            m_outVideoQueue.Push( videoMediaData );

            if( m_outVideoQueue.Size() > 1 )
            {
                VideoMediaData del;
                m_outVideoQueue.TryPop( del );
            }

            return true;
        }
    }

    return false;
}

// *********************************
//
bool					FFmpegVideoDecoder::NextAudioDataReady		()
{
    if( m_hasAudio )
    {
        if( m_demuxerThread->Stopped() )
        {
            m_demuxerThread->Restart();
        }

        if( m_audioDecoderThread->Stopped() )
        {
            m_audioDecoderThread->Restart();
        }

        AudioMediaData audioMediaData;
        if( m_audioDecoder->GetData( audioMediaData ) )
        {
            m_outAudioQueue.Push( audioMediaData );

            if( m_outAudioQueue.Size() > 1 )
            {
                AudioMediaData del;
                m_outAudioQueue.TryPop( del );
            }

            return true;
        }
    }

    return false;
}

// *********************************
//
SizeType				FFmpegVideoDecoder::GetFrameSize			() const
{
    if( m_hasVideo )
    {
        return m_videoDecoder->GetFrameSize();
    }
    return 0;
}

// *********************************
//
UInt32					FFmpegVideoDecoder::GetWidth				() const
{
    if( m_hasVideo )
    {
        return m_videoDecoder->GetWidth();
    }
    return 0;
}

// *********************************
//
UInt32					FFmpegVideoDecoder::GetHeight				() const
{
    if( m_hasVideo )
	{
        return m_videoDecoder->GetHeight();
    }
    return 0;
}

// *********************************
//
Float64					FFmpegVideoDecoder::GetFrameRate			() const
{
    if( m_hasVideo )
	{
	    return m_videoDecoder->GetFrameRate();
    }
    return 0;
}

// *********************************
//
bool					FFmpegVideoDecoder::HasVideo			    () const
{
    return m_hasVideo ;
}


// *********************************
//
bool					FFmpegVideoDecoder::HasAudio			    () const
{
    return m_hasAudio;
}

// *********************************
//
void					FFmpegVideoDecoder::Seek					( Float64 time ) 
{
	std::lock_guard< std::mutex > lock( m_mutex );
    
    auto stopped = m_demuxerThread->Stopped();
    if( !stopped )
    {
        m_demuxerThread->Stop();
        while( !m_demuxerThread->Stopped() );
    }

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

    if( !stopped )
    {
        m_demuxerThread->Restart();
    }
}

// *********************************
//
void					FFmpegVideoDecoder::FlushBuffers			() 
{
	m_outVideoQueue.Clear();
	m_outAudioQueue.Clear();
}

// *********************************
//
void					FFmpegVideoDecoder::Reset					() 
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

// *********************************
//
bool					FFmpegVideoDecoder::IsEOF					() const
{
	std::lock_guard< std::mutex > lock( m_mutex );
    return m_demuxer->IsEOF();
}

// *********************************
//
bool					FFmpegVideoDecoder::IsPacketQueueEmpty		( Int32 streamIdx ) const
{
	std::lock_guard< std::mutex > lock( m_mutex );
    return m_demuxer->IsPacketQueueEmpty( streamIdx ) && m_videoDecoder->IsDataQueueEmpty();
}

// *********************************
//
bool					FFmpegVideoDecoder::IsFinished				() const
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