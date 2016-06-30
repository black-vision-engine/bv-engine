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
    if( asset->IsVideoEnabled() )
    {
        auto vstreamIdx = m_demuxer->GetStreamIndex( AVMEDIA_TYPE_VIDEO );
        if( vstreamIdx >= 0 )
        {
            auto streamData = new StreamData();

	        streamData->decoder = std::move( std::unique_ptr< FFmpegVideoStreamDecoder >( new FFmpegVideoStreamDecoder( asset, m_demuxer->GetFormatContext(), vstreamIdx ) ) );
            streamData->decoderThread = std::move( std::unique_ptr< FFmpegStreamDecoderThread >( new FFmpegStreamDecoderThread( this, streamData->decoder.get(), m_demuxer.get() ) ) );
            streamData->prevPTS = 0;

            videoDuration = streamData->decoder->GetDuration();

            m_streams[ AVMEDIA_TYPE_VIDEO ] = streamData;
        }
    }

    if( asset->IsAudioEnabled() )
    {
        auto astreamIdx = m_demuxer->GetStreamIndex( AVMEDIA_TYPE_AUDIO );
        if( astreamIdx >= 0 )
        {
            auto streamData = new StreamData();

	        streamData->decoder = std::move( std::unique_ptr< FFmpegAudioStreamDecoder >( new FFmpegAudioStreamDecoder( asset, m_demuxer->GetFormatContext(), astreamIdx ) ) );
            streamData->decoderThread = std::move( std::unique_ptr< FFmpegStreamDecoderThread >( new FFmpegStreamDecoderThread( this, streamData->decoder.get(), m_demuxer.get() ) ) );
            streamData->prevPTS = 0;

            audioDuration = streamData->decoder->GetDuration();
            
            m_streams[ AVMEDIA_TYPE_AUDIO ] = streamData;
        }
    }

    m_duration = std::max( videoDuration, audioDuration );

	m_decoderThread = std::unique_ptr< AVDecoderThread >( new AVDecoderThread( this ) );
   
    m_demuxerThread->Start();

    for( auto & stream : m_streams )
    {
        stream.second->decoderThread->Start();
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


    for( auto & stream : m_streams )
    {
        stream.second->decoderThread->Kill();
        stream.second->decoderThread->Join();
        stream.second->decoderThread = nullptr;
    }

	m_demuxerThread->Kill();
	m_demuxerThread->Join();
	m_demuxerThread = nullptr;

    FlushBuffers();

	//force 
    for( auto & stream : m_streams )
    {
        stream.second->decoder = nullptr;
    }

	m_demuxer = nullptr;

    for( auto & stream : m_streams )
    {
        delete stream.second;
    }
}

// *********************************
//
void						FFmpegAVDecoder::Play				()
{
    m_decoderThread->Play();
}

// *********************************
//
void						FFmpegAVDecoder::Pause				()
{
    if( !m_decoderThread->Paused() )
    {
        StopDecoding();
    }

    m_decoderThread->Pause();
    while( !m_decoderThread->Paused() );
}

// *********************************
//
void						FFmpegAVDecoder::Stop				()
{
    StopDecoding();

    m_decoderThread->Stop();
    while( !m_decoderThread->Stopped() );

	Reset();
}

// *********************************
//
bool			            FFmpegAVDecoder::GetVideoMediaData	( AVMediaData & mediaData )
{
    if( HasVideo() )
    {
        return m_streams[ AVMEDIA_TYPE_VIDEO ]->outQueue.TryPop( mediaData );
    }

    return false;
}

// *********************************
//
bool			            FFmpegAVDecoder::GetAudioMediaData  ( AVMediaData & mediaData )
{
    if( HasAudio() )
    {
        return m_streams[ AVMEDIA_TYPE_AUDIO ]->outQueue.TryPop( mediaData );
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
        FlushBuffers();

        if( m_demuxerThread->Stopped() )
        {
            m_demuxerThread->Restart();
        }

        auto & videoStreamData = m_streams[ AVMEDIA_TYPE_VIDEO ];

        if( videoStreamData->decoderThread->Stopped() )
        {
            videoStreamData->decoderThread->Restart();
        }

        videoStreamData->decoder->GetData( data );
    }

    return data;
}

// *********************************
//
bool				FFmpegAVDecoder::NextVideoDataReady	        ( UInt64 time )
{
    return NextStreamDataReady( AVMEDIA_TYPE_VIDEO, time );
}

// *********************************
//
bool				FFmpegAVDecoder::NextAudioDataReady		    ( UInt64 time )
{
    if( !m_muted )
    {
        return NextStreamDataReady( AVMEDIA_TYPE_AUDIO, time );
    }
    return false;
}

// *********************************
//
SizeType				FFmpegAVDecoder::GetVideoFrameSize		    () const
{
    if( HasVideo() )
    {
        return static_cast< FFmpegVideoStreamDecoder * >( m_streams.at( AVMEDIA_TYPE_VIDEO )->decoder.get() )->GetFrameSize();
    }
    return 0;
}

// *********************************
//
UInt32					FFmpegAVDecoder::GetWidth				() const
{
    if( HasVideo() )
    {
        return static_cast< FFmpegVideoStreamDecoder * >( m_streams.at( AVMEDIA_TYPE_VIDEO )->decoder.get() )->GetWidth();
    }
    return 0;
}

// *********************************
//
UInt32					FFmpegAVDecoder::GetHeight				() const
{
    if( HasVideo() )
	{
        return static_cast< FFmpegVideoStreamDecoder * >( m_streams.at( AVMEDIA_TYPE_VIDEO )->decoder.get() )->GetHeight();
    }
    return 0;
}

// *********************************
//
Int32				    FFmpegAVDecoder::GetSampleRate			() const
{
    if( HasAudio() )
	{
        return static_cast< FFmpegAudioStreamDecoder * >( m_streams.at( AVMEDIA_TYPE_AUDIO )->decoder.get() )->GetSampleRate();
    }
    return 0;
}

// *********************************
//
AudioFormat				FFmpegAVDecoder::GetAudioFormat			() const
{
    if( HasAudio() )
	{
        return static_cast< FFmpegAudioStreamDecoder * >( m_streams.at( AVMEDIA_TYPE_AUDIO )->decoder.get() )->GetFormat();
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
void					FFmpegAVDecoder::Seek					( Float64 time ) 
{
    StopDecoding();

    for( auto & stream : m_streams )
    {
        Seek( stream.second->decoder.get(), time );
    }
}

// *********************************
//
void					FFmpegAVDecoder::FlushBuffers			() 
{
    for( auto & stream : m_streams )
    {
        ClearStream( stream.second );
    }
    m_demuxer->ClearPacketQueue();
}

// *********************************
//
void					FFmpegAVDecoder::Reset					() 
{
	m_demuxer->Reset();
    FlushBuffers();
    Seek( 0.f );
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
        finished &= m_demuxer->IsPacketQueueEmpty( stream.second->decoder->GetStreamIdx() );
        finished &= stream.second->decoder->IsDataQueueEmpty();
        finished &= stream.second->outQueue.IsEmpty();
    }

    return finished;
}

// *********************************
//
void					FFmpegAVDecoder::Mute				        ( bool mute )
{
    if( HasAudio() )
    {
        auto audioStream = m_streams[ AVMEDIA_TYPE_AUDIO ];

        Pause();
    
        if( mute )
        {
            ClearStream( audioStream );
            m_demuxer->ClearPacketQueue( audioStream->decoder->GetStreamIdx() );
            m_demuxer->DisableStream( AVMediaType::AVMEDIA_TYPE_AUDIO );
        }
        else
        {
            m_demuxer->GetStreamIndex( AVMediaType::AVMEDIA_TYPE_AUDIO );
        }

        m_muted = mute;

        m_decoderThread->Pause();
    }
}

// *********************************
//
void					FFmpegAVDecoder::StopDecoding           ()
{
    m_demuxerThread->Stop();
    while( !m_demuxerThread->Stopped() );

    for( auto & stream : m_streams )
    {
        stream.second->decoderThread->Stop();
        while( !stream.second->decoderThread->Stopped() );
    }
}

// *********************************
//
void					FFmpegAVDecoder::ClearStream           ( StreamData * streamData )
{
    streamData->outQueue.Clear();
    streamData->decoder->Reset();
    streamData->prevPTS = 0;
}

// *********************************
//
void					FFmpegAVDecoder::Seek				    ( FFmpegStreamDecoder * decoder, Float64 time )
{
    auto ts = decoder->ConvertTime( time );
	m_demuxer->Seek( ts, decoder->GetStreamIdx() );

    Int64 currTs = 0;
    while( currTs < ts )
    {
        if( m_demuxerThread->Stopped() )
        {
            m_demuxerThread->Restart();
        }

        decoder->ProcessPacket( m_demuxer.get() );
        auto packet = m_demuxer->GetPacket( decoder->GetStreamIdx() );
        if( packet != nullptr )
        {
            currTs = packet->dts;
	        decoder->DecodePacket( packet );
        }
        else if( m_demuxer->IsEOF() )
        {
            break;
        }
    }
}

// *********************************
//
bool				FFmpegAVDecoder::NextStreamDataReady	        ( AVMediaType type, UInt64 time )
{
    auto success = false;

    if( m_streams.count( type ) )
    {
        auto & streamData = m_streams[ type ];

        if( time <= m_duration )
        {
            if( m_demuxerThread->Stopped() )
            {
                m_demuxerThread->Restart();
            }

            if( streamData->decoderThread->Stopped() )
            {
                streamData->decoderThread->Restart();
            }

            auto currVideoPTS = streamData->decoder->GetCurrentPTS();
            while( streamData->outQueue.IsEmpty() &&
                !streamData->decoder->IsDataQueueEmpty() &&
                ( currVideoPTS <= time ) &&
                ( streamData->prevPTS <= currVideoPTS ) )
            {
                AVMediaData data;
                if( streamData->decoder->GetData( data ) )
                {
                    streamData->outQueue.Push( data );
                    streamData->prevPTS = data.framePTS;
                    currVideoPTS = streamData->decoder->GetCurrentPTS();
                    success = true;
                }
            }
        }
        else
        {
            streamData->prevPTS = 0;
        }
    }

    return success;
}

} //bv