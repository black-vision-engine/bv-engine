#include "stdafx.h"

#include "AudioRenderer.h"

#include "BVAL.h"

#include "Engine/Audio/OALRenderer/PdrAudioBuffersQueue.h"
#include "Engine/Audio/OALRenderer/PdrSource.h"
#include "Engine/Audio/AudioEntity.h"
#include "Engine/Audio/Resources/AudioUtils.h"

#include "Engine/Events/Events.h"
#include "Engine/Events/Interfaces/IEventManager.h"

#include "UseLoggerLibBlackVision.h"


namespace bv { namespace audio {


// *********************************
//
AudioRenderer::AudioRenderer	()
    : m_format( AudioUtils::DEFAULT_SAMPLE_FORMAT )
    , m_frequency( AudioUtils::DEFAULT_SAMPLE_RATE )
	, m_gain( 1.f )
{
    Initialize();
}

// *********************************
//
	    AudioRenderer::~AudioRenderer	()
{
}

// *********************************
//
void	AudioRenderer::Initialize	    ()
{
    if( !BVAL::InitializeBVAL() )
    {
        assert( false ); //FIXME
    }
}

// *********************************
//
void	AudioRenderer::Terminate        ()
{
}

// *********************************
//
void    AudioRenderer::Proccess         ( AudioEntity * audio )
{
    //FIXME: no mechanism to free pdrsource & pdraudiobuffersqueue

    auto source = GetPdrSource( audio ); 
    auto queue = GetPdrAudioBuffersQueue( source, audio );

	source->SetGain( m_gain );

    if( queue && source )
    {
        if( m_audioEntityUpdateIDMap[ audio ] < audio->GetUpdateID() )
        {
            queue->Reinitialize( audio->GetFrequency(), audio->GetFormat() );
            m_audioEntityUpdateIDMap[ audio ] = audio->GetUpdateID();
        }

        if( audio->IsEOF() && ( queue->GetBufferedDataSize() == 0 ) && queue->BufferingDone() )
        {
            audio->Stop();
        }

        if( !audio->IsEmpty() )
        {
            queue->PushData( audio->PopData() );
        }

        if( audio->IsPlaying() && queue->BufferData() )
        {
            source->Play();
        }
    }
}

// *********************************
//
void    AudioRenderer::Play             ( AudioEntity * audio )
{
    audio->Play();
}

// *********************************
//
void    AudioRenderer::Pause            ( AudioEntity * audio )
{
    audio->Stop();

    auto source = GetPdrSource( audio, false ); 
    if( source )
    {
        source->Pause();
    }
}

// *********************************
//
void    AudioRenderer::Stop             ( AudioEntity * audio )
{
    audio->Stop();
    audio->Clear();

    auto source = GetPdrSource( audio, false );
    if( source )
    {
        source->Stop();
        auto queue = GetPdrAudioBuffersQueue( source, audio, false );
        if( queue )
        {
            queue->Reinitialize( audio->GetFrequency(), audio->GetFormat() );
        }
    }
}

// *********************************
//
void    AudioRenderer::EndOfFile        ( AudioEntity * audio )
{
    audio->PushData( nullptr );
}

// *********************************
//
PdrSource *             AudioRenderer::GetPdrSource                 ( const AudioEntity * audio, bool autoCreate )
{
    PdrSource * source = nullptr;

    if( !m_sources.count( audio ) )
    {
        if( autoCreate )
        {
            source = new PdrSource();
            m_sources[ audio ] = source;
            m_audioEntityUpdateIDMap[ audio ] = 0;
        }
    }
    else
    {
        source = m_sources[ audio ];
    }

    return source;
}

// *********************************
//
PdrAudioBuffersQueue *  AudioRenderer::GetPdrAudioBuffersQueue      ( PdrSource * source, const AudioEntity * audio, bool autoCreate )
{
    PdrAudioBuffersQueue * queue = nullptr;

    if( !m_bufferMap.count( source ) )
    {
        if( ( m_format != audio->GetFormat() ) || ( m_frequency != audio->GetFrequency() ) )
        {
            /** @brief all audio channels must be converted to the same format to work with videocards */
            LOG_MESSAGE( SeverityLevel::error ) << "Audio was not converted to default format required by videocards";
            assert( false );
        }
        else if( autoCreate )
        {
            queue = new PdrAudioBuffersQueue( source->GetHandle(), audio->GetFrequency(), audio->GetFormat() );
            m_bufferMap[ source ] = queue;
        }
    }
    else
    {
        queue = m_bufferMap[ source ];
    }

    return queue;
}

// *********************************
//
void                    AudioRenderer::DeletePDR                    ( const AudioEntity * audio )
{
    if( m_sources.count( audio ) )
    {
        auto source = m_sources[ audio ];
        DeleteSinglePDR( m_sources, audio );
        DeleteSinglePDR( m_bufferMap, source );
        m_audioEntityUpdateIDMap.erase( audio );
    }
}

// *********************************
//
AudioBufferConstPtr     AudioRenderer::GetBufferedData              ( MemoryChunkPtr data )
{
    // check whether active audio buffer exists
    if( IsAnySourcePlaying() )
    {
        // check whether any data needs uploading
        if( IsAnyBufferReady( data->Size() ) )
        {
            data->Clear();
        }

        for( auto & obj : m_sources )
        {        
            auto queue = m_bufferMap.at( obj.second );
            queue->MixBufferedData( data, obj.first->IsEOF() );
        }
    }
    else
    {
        data->Clear();
    }

	auto size = data->Size();
	auto outData = MemoryChunk::Create( size );

	audio::AudioUtils::ApplyGain( outData->GetWritable(), data->Get(), size, m_gain );

    return audio::AudioBuffer::Create( data, m_frequency, m_format, false );
}

// *********************************
//
UInt32                  AudioRenderer::GetChannels                  () const
{
    return AudioUtils::ChannelsCount( m_format );
}

// *********************************
//
Int32                   AudioRenderer::GetFrequency                 () const
{
    return m_frequency;
}


// *********************************
//
UInt32                  AudioRenderer::GetChannelDepth                 () const
{
    return AudioUtils::ChannelDepth( m_format );
}

// *********************************
//
template< typename MapType >
void                    AudioRenderer::DeleteSinglePDR              ( MapType & resMap, typename MapType::key_type & key )
{
    auto it = resMap.find( key );

    if( it != resMap.end() )
    {
        auto res = it->second;

        delete res;

        resMap.erase( it );
    }
}

// *********************************
//
bool                    AudioRenderer::IsAnySourcePlaying           () const
{
    for( auto & obj : m_sources )
    {
        if( obj.first->IsPlaying() )
        {
            return true;
        }
    }
    return false;
}

// *********************************
//
bool                    AudioRenderer::IsAnyBufferReady             ( SizeType requestedBufferSize ) const
{
    for( auto & obj : m_sources )
    {        
        auto queue = m_bufferMap.at( obj.second );
        if( !obj.first->IsEOF() )
        {
            if( requestedBufferSize <= queue->GetBufferedDataSize() )
            {
                return true;
            }
        }
        else
        {
            if( queue->GetBufferedDataSize() > 0 )
            {
                return true;
            }
        }
    }
    return false;
}

// *********************************
//
void					AudioRenderer::SetGain						( Float32 gain )
{
	m_gain = gain;
}

} // audio
} //bv
