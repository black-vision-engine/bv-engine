#include "stdafx.h"
#include "FFmpegAVDecoder.h"

#include "FFmpegUtils.h"




#include "Memory/MemoryLeaks.h"



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

    m_duration = ( std::max )( videoDuration, audioDuration );

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
    auto paused = m_decoderThread->Paused();
    if( !paused )
    {
        StopDecoding();
    }

    m_decoderThread->Pause();

    if( !paused )
    {
        while( !m_decoderThread->Paused() );
    }
}

// *********************************
//
void						FFmpegAVDecoder::Stop				()
{
    StopDecoding();

    m_decoderThread->Stop();
    while( !m_decoderThread->Stopped() );

	Seek( 0.f );

    for( auto & stream : m_streams )
    {
        stream.second->prevPTS = 0;
    }
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

        m_demuxerThread->Restart();

        auto & videoStreamData = m_streams[ AVMEDIA_TYPE_VIDEO ];

        videoStreamData->decoderThread->Restart();

        videoStreamData->decoder->PopData( data );
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
void					FFmpegAVDecoder::Seek					( Float64 time, bool flushBuffers ) 
{
    auto paused = m_decoderThread->Paused();
    if( !paused )
    {
        Pause();    // pause decoding threads
    }

    if( flushBuffers )
    {
        FlushBuffers();
    }

    // seek all streams to the nearest keyframe
    for( auto & stream : m_streams )
    {
        auto & decoder = stream.second->decoder;
        auto timestamp = decoder->ConvertTime( time );

	    m_demuxer->Seek( timestamp, decoder->GetStreamIdx() );
    }

    // accurate seek all stream to the given frame
    for( auto & stream : m_streams )
    {
        auto & decoder = stream.second->decoder;
        auto timestamp = decoder->ConvertTime( time );

        Seek( decoder.get(), timestamp );

        // add first frame to the buffer
        if( decoder->ProcessPacket( m_demuxer.get() ) )
        {
            auto currPTS = decoder->GetCurrentPTS();
            decoder->SetOffset( currPTS );
            stream.second->prevPTS = currPTS;
        }
    }

    if( !paused )
    {
        Pause();    // unpause decoding threads
    }
}

// *********************************
//
void					FFmpegAVDecoder::FlushBuffers			() 
{
    m_demuxer->ClearPacketQueue();
    for( auto & stream : m_streams )
    {
        ClearStream( stream.second );
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

        Pause();
    }
}


// *********************************
//
void					FFmpegAVDecoder::ProcessFirstVideoFrame ()
{
    if( HasVideo() )
    {
        auto & videoStreamData = m_streams[ AVMEDIA_TYPE_VIDEO ];
        while( !NextVideoDataReady( videoStreamData->decoder->GetCurrentPTS() ) )
        {
            if( !videoStreamData->outQueue.IsEmpty()
                || IsFinished() )
            {
                break;
            }
        }
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
}

// *********************************
//
void					FFmpegAVDecoder::Seek				    ( FFmpegStreamDecoder * decoder, Int64 timestamp )
{
    Int64 currTs = 0;
    // decode packets till reaching frame with given timestamp
    while( currTs < timestamp )
    {
        m_demuxerThread->Restart();

        auto ffmpegPacket = m_demuxer->GetPacket( decoder->GetStreamIdx() );
        if( ffmpegPacket )
        {
            auto packet = ffmpegPacket->GetAVPacket();
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
        auto streamData = m_streams[ type ];

        if( time <= m_duration )
        {
            m_demuxerThread->Restart();
            streamData->decoderThread->Restart();

            AVMediaData data;
            if( streamData->outQueue.IsEmpty() )
            {
                auto currentPTS = streamData->decoder->GetCurrentPTS();

                // find the closest frame to given time
                while( !streamData->decoder->IsDataQueueEmpty()
                    && ( streamData->prevPTS <= currentPTS )
                    && ( currentPTS <= time + streamData->decoder->GetOffset() ) )
                {
                    success = streamData->decoder->PopData( data );
                    currentPTS = streamData->decoder->GetCurrentPTS();
                }

                if( success )
                {
                    streamData->outQueue.Push( data );
                    streamData->prevPTS = data.framePTS;
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