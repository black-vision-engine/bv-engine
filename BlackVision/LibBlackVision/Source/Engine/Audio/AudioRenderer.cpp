#include "stdafx.h"

#include "AudioRenderer.h"

#include "BVAL.h"

#include "Engine/Audio/OALRenderer/PdrAudioBuffersQueue.h"
#include "Engine/Audio/OALRenderer/PdrSource.h"
#include "Engine/Audio/AudioEntity.h"

#include "Engine/Events/Events.h"
#include "Engine/Events/Interfaces/IEventManager.h"

#include "UseLoggerLibBlackVision.h"


namespace bv { namespace audio {


// *********************************
//
	    AudioRenderer::AudioRenderer	()
    : m_channels( 0 )
    , m_frequency( 0 )
    , m_channelDepth( 0 )
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
void    AudioRenderer::Play             ( AudioEntity * audio )
{
    //FIXME: no mechanism to free pdrsource & pdraudiobuffersqueue

    auto source = GetPdrSource( audio ); 
    auto queue = GetPdrAudioBuffersQueue( source, audio );

    if( m_audioEntityUpdateIDMap[ audio ] < audio->GetUpdateID() )
    {
        queue->Reinitialize( audio->GetFrequency(), audio->GetFormat() );
        m_audioEntityUpdateIDMap[ audio ] = audio->GetUpdateID();
    }

    if( !audio->IsEmpty() )
    {
        queue->PushData( audio->PopData() );
    }

    if( queue->BufferData() )
    {
        source->Play();
    }
}

// *********************************
//
void    AudioRenderer::Pause            ( AudioEntity * audio )
{
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
        if( autoCreate )
        {
            queue = new PdrAudioBuffersQueue( source->GetHandle(), audio->GetFrequency(), audio->GetFormat() );
            m_bufferMap[ source ] = queue;

            m_channels = audio->GetChannels();
            m_frequency = audio->GetFrequency();
            m_channelDepth = audio->GetChannelDepth();
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
    AudioBufferPtr mixedAudioBuffer = audio::AudioBuffer::Create( data, 48000, AudioFormat::STEREO16 ); //FIXME

    for( auto & obj : m_bufferMap )
    {
        if( obj.second->GetBufferedData( data ) )
        {
            //FIXME mix audio - for now only take one audio channel
            return mixedAudioBuffer;
        }
    }

    return mixedAudioBuffer;
}

// *********************************
//
UInt32                  AudioRenderer::GetChannels                  () const
{
    return m_channels;
}

// *********************************
//
UInt32                  AudioRenderer::GetFrequency                 () const
{
    return m_frequency;
}


// *********************************
//
UInt32                  AudioRenderer::GetChannelDepth                 () const
{
    return m_channelDepth;
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

} // audio
} //bv
