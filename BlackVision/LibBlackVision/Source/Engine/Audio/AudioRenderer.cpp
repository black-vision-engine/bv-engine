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
AudioBufferConstPtr     AudioRenderer::GetBufferedData              ()
{
    AudioBufferPtr mixedAudioBuffer = nullptr;
    for( auto & obj : m_bufferMap )
    {
        AudioBufferConstPtr bufferedData = nullptr;
        if( obj.second->GetBufferedData( bufferedData ) )
        {
            if( !mixedAudioBuffer )
            {
                mixedAudioBuffer = audio::AudioBuffer::Create( bufferedData->GetData(), bufferedData->GetFrequency(), bufferedData->GetFormat() );
            }
            else
            {
                //mix audio
            }
        }
    }

    return mixedAudioBuffer;
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
