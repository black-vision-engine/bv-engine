#include "stdafx.h"
#include "FFmpegAVDecoder.h"

#include "FFmpegUtils.h"


namespace bv {


// *********************************
//
FFmpegAVDecoder::FFmpegAVDecoder		( AVAssetConstPtr asset )
    : m_muted( false )
{
	auto path = asset->GetStreamPath();

	m_demuxer = std::unique_ptr< FFmpegDemuxer >( new FFmpegDemuxer( path ) );
    m_demuxerThread = std::unique_ptr< FFmpegDemuxerThread >( new FFmpegDemuxerThread( m_demuxer.get() ) );

    UInt64 videoDuration = 0, audioDuration = 0;

    FFmpegVideoStreamDecoder * videoStreamDecoder = nullptr;
    FFmpegAudioStreamDecoder * audioStreamDecoder = nullptr;

    if( asset->IsVideoEnabled() )
    {
        auto vstreamIdx = m_demuxer->GetStreamIndex( AVMEDIA_TYPE_VIDEO );
        if( vstreamIdx >= 0 )
        {
            videoStreamDecoder = new FFmpegVideoStreamDecoder( asset, m_demuxer->GetFormatContext(), vstreamIdx, 10 );
            m_streams[ AVMEDIA_TYPE_VIDEO ] = std::move( std::unique_ptr< FFmpegVideoStreamDecoder >( videoStreamDecoder ) );

            videoDuration = videoStreamDecoder->GetDuration();
        }
    }

    if( asset->IsAudioEnabled() )
    {
        auto astreamIdx = m_demuxer->GetStreamIndex( AVMEDIA_TYPE_AUDIO );
        if( astreamIdx >= 0 )
        {
            audioStreamDecoder = new FFmpegAudioStreamDecoder( asset, m_demuxer->GetFormatContext(), astreamIdx, 100 );
            m_streams[ AVMEDIA_TYPE_AUDIO ] = std::move( std::unique_ptr< FFmpegAudioStreamDecoder >( audioStreamDecoder ) );

            audioDuration = audioStreamDecoder->GetDuration();
        }
    }

    m_duration = ( std::max )( videoDuration, audioDuration );

	m_decoderThread = std::unique_ptr< AVDecoderThread >( new AVDecoderThread( this ) );
   
    m_streamsDecoderThread = std::move( std::unique_ptr< FFmpegStreamsDecoderThread >( new FFmpegStreamsDecoderThread( videoStreamDecoder, audioStreamDecoder, m_demuxer.get() ) ) );

    m_demuxerThread->Start();
    m_streamsDecoderThread->Start();

    m_decoderThread->Start();
}

// *********************************
//
FFmpegAVDecoder::~FFmpegAVDecoder		()
{
    assert( m_decoderThread );
	
	StopDecoding();

	m_demuxerThread->Kill();
	m_demuxerThread->Join();
	m_demuxerThread = nullptr;

	m_streamsDecoderThread->Kill();
	m_streamsDecoderThread->Join();
	m_streamsDecoderThread = nullptr;

	for( auto & s : m_streams )
	{
		s.second->FinishQueue();
	}

	FlushBuffers();

	m_decoderThread->Kill();
	m_decoderThread->Join();
	m_decoderThread = nullptr;

	m_streams.clear();

	//force 
    

	m_demuxer = nullptr;
}

// *********************************
//
void						FFmpegAVDecoder::Play				()
{
    m_decoderThread->Play();
    RestartDecoding();
}

// *********************************
//
void						FFmpegAVDecoder::Pause				()
{
    if( m_decoderThread->Pause() )
    {
        StopDecoding();
    }
    else
    {
        RestartDecoding();
    }
}

// *********************************
//
void						FFmpegAVDecoder::Stop				()
{
    m_decoderThread->Stop();
    while( !m_decoderThread->Stopped() );

    StopDecoding();
}

// *********************************
//
bool			            FFmpegAVDecoder::GetVideoMediaData	( AVMediaData & mediaData )
{
    if( HasVideo() )
    {
        return m_streams[ AVMEDIA_TYPE_VIDEO ]->PopData( mediaData );
    }

    return false;
}

// *********************************
//
bool			            FFmpegAVDecoder::GetAudioMediaData  ( AVMediaData & mediaData )
{
    if( HasAudio() )
    {
        return m_streams[ AVMEDIA_TYPE_AUDIO ]->PopData( mediaData );
    }

    return false;
}

// ***********************
// Jumps to frameTime. This function doesn't return to time before this function call.
AVMediaData		            FFmpegAVDecoder::GetSingleFrame  	( TimeType frameTime )
{
    AVMediaData data;

    if( HasVideo() )
    {
        Seek( frameTime );
        
        Play();

        while( !GetVideoMediaData( data ) );

        Stop();
    }

    return data;
}

// *********************************
//
bool				FFmpegAVDecoder::NextDataReady	            ( AVMediaType type, UInt64 time, bool block )
{
    if( m_streams.count( type ) )
    {
        return m_streams[ type ]->NextDataReady( time, block );
    }

    return false;
}


// *********************************
//
bool				FFmpegAVDecoder::NextDataReady              ( UInt64 time, bool block )
{
	bool result = true;
    for( auto & stream : m_streams )
    {
		result &= stream.second->NextDataReady( time, block );
    }
	return result;
}

// *********************************
//
SizeType				FFmpegAVDecoder::GetVideoFrameSize		    () const
{
    if( HasVideo() )
    {
        return static_cast< FFmpegVideoStreamDecoder * >( m_streams.at( AVMEDIA_TYPE_VIDEO ).get() )->GetFrameSize();
    }
    return 0;
}

// *********************************
//
UInt32					FFmpegAVDecoder::GetWidth				() const
{
    if( HasVideo() )
    {
        return static_cast< FFmpegVideoStreamDecoder * >( m_streams.at( AVMEDIA_TYPE_VIDEO ).get() )->GetWidth();
    }
    return 0;
}

// *********************************
//
UInt32					FFmpegAVDecoder::GetHeight				() const
{
    if( HasVideo() )
	{
        return static_cast< FFmpegVideoStreamDecoder * >( m_streams.at( AVMEDIA_TYPE_VIDEO ).get() )->GetHeight();
    }
    return 0;
}

// *********************************
//
Int32				    FFmpegAVDecoder::GetSampleRate			() const
{
    if( HasAudio() )
	{
        return static_cast< FFmpegAudioStreamDecoder * >( m_streams.at( AVMEDIA_TYPE_AUDIO ).get() )->GetSampleRate();
    }
    return 0;
}

// *********************************
//
AudioFormat				FFmpegAVDecoder::GetAudioFormat			() const
{
    if( HasAudio() )
	{
        return static_cast< FFmpegAudioStreamDecoder * >( m_streams.at( AVMEDIA_TYPE_AUDIO ).get() )->GetFormat();
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
    return ( m_streams.count( AVMEDIA_TYPE_VIDEO ) > 0 );
}

// *********************************
//
bool					FFmpegAVDecoder::HasAudio			    () const
{
    return ( m_streams.count( AVMEDIA_TYPE_AUDIO ) > 0 );
}

// *********************************
//
void					FFmpegAVDecoder::Seek					( Float64 time, bool flushBuffers ) 
{
	StopDecoding();
    //auto paused = m_decoderThread->Paused() || m_decoderThread->Stopped();
    //if( !paused )
    //{
    //    Pause();    // pause decoding threads
    //}

    if( flushBuffers )
    {
        FlushBuffers();
    }

    // seek all streams to the nearest keyframe
    for( auto & stream : m_streams )
    {
        auto decoder = stream.second.get();
	    m_demuxer->Seek( decoder->ConvertTime( time ), decoder->GetStreamIdx() );
    }

    // accurate seek all stream to the given frame
    for( auto & stream : m_streams )
    {
        auto decoder = stream.second.get();
        auto currPTS = Seek( decoder, FFmpegUtils::ConvertToMiliseconds( time ) );
        decoder->SetOffset( currPTS );
    }

	RestartDecoding();
    //if( !paused )
    //{
    //    Pause();    // unpause decoding threads
    //}
}

// *********************************
//
void					FFmpegAVDecoder::FlushBuffers			() 
{
    m_demuxer->ClearPacketQueue( false );
    for( auto & stream : m_streams )
    {
        stream.second->Reset();
    }
}

// *********************************
//
bool					FFmpegAVDecoder::IsEOF					() const
{
    return m_demuxer->IsEOF();
}

// *********************************
//
bool					FFmpegAVDecoder::IsFinished				() const
{
    bool finished = true;
    finished &= IsEOF();

    for( auto & stream : m_streams )
    {
        finished &= m_demuxer->IsPacketQueueEmpty( stream.second->GetStreamIdx() );
        finished &= stream.second->IsDataQueueEmpty();
        finished &= stream.second->IsOutQueueEmpty();
    }

    return finished;
}

// *********************************
//
void					FFmpegAVDecoder::Mute				        ( bool mute )
{
    if( HasAudio() )
    {
        Pause();
    
        if( mute )
        {
            m_streams[ AVMEDIA_TYPE_AUDIO ]->Reset();

            m_demuxer->ClearPacketQueue( m_streams[ AVMEDIA_TYPE_AUDIO ]->GetStreamIdx(), false );
            m_demuxer->DisableStream( AVMediaType::AVMEDIA_TYPE_AUDIO );
        }
        else
        {
            m_demuxer->GetStreamIndex( AVMediaType::AVMEDIA_TYPE_AUDIO );
        }

        m_muted = mute;

        Pause();
    }
}


// *********************************
//
void					FFmpegAVDecoder::ProcessFirstAVFrame    ()
{
    RestartDecoding();

    if( HasVideo() )
    {
        auto decoder = m_streams[ AVMEDIA_TYPE_VIDEO ].get();
        while( decoder->IsOutQueueEmpty() && !IsFinished() )
        {
            NextDataReady( AVMEDIA_TYPE_VIDEO, decoder->GetCurrentPTS(), false );
        }
    }
        
    if( HasAudio() )
    {
        auto i = 0;
        auto decoder = m_streams[ AVMEDIA_TYPE_AUDIO ].get();
        while( i < 5 && !IsFinished() )
        {
            if( NextDataReady( AVMEDIA_TYPE_AUDIO, decoder->GetDuration(), false ) )
            {
                i++;
            }
        }

    }
    
    StopDecoding();
}

// *********************************
//
void					FFmpegAVDecoder::RestartDecoding        ()
{
    m_demuxerThread->Restart();
    m_streamsDecoderThread->Restart();
}

// *********************************
//
void					FFmpegAVDecoder::StopDecoding           ()
{
	m_streamsDecoderThread->Stop();
	m_demuxerThread->Stop();

	while( !m_streamsDecoderThread->Stopped() );

    m_demuxerThread->Stop();
	m_demuxer->ClearPacketQueue( false );

    while( !m_demuxerThread->Stopped() );

	m_demuxer->ClearPacketQueue( false );
}

// *********************************
//
Int64					FFmpegAVDecoder::Seek				    ( FFmpegStreamDecoder * decoder, Int64 time )
{
    Int64 currTs = 0;
    
    // decode packets till reaching frame with given timestamp
    while( !m_demuxer->IsEOF() )
    {
        while( decoder->ProcessPacket( m_demuxer.get() ) )
        {
            currTs = decoder->GetCurrentPTS();

            decoder->UploadData();

            if( currTs < time )
            {
                AVMediaData data;
                decoder->PopData( data );
            }
            else
            {
                return currTs;
            }
        }

        m_demuxer->ProcessPacket();
    }

    return currTs;
}

} //bv
