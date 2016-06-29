#include "stdafx.h"

#include "AudioRenderer.h"

#include "BVAL.h"

#include "Engine/Audio/OALRenderer/PdrAudioBuffersQueue.h"
#include "Engine/Audio/OALRenderer/PdrSource.h"
#include "Engine/Audio/AudioEntity.h"

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

    PdrSource * source = GetPdrSource( audio ); 
    PdrAudioBuffersQueue * queue = GetPdrAudioBuffersQueue( source, audio );

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
PdrSource *             AudioRenderer::GetPdrSource                 ( const AudioEntity * audio )
{
    PdrSource * source = nullptr;

    if( !m_sources.count( audio ) )
    {
        source = new PdrSource();
        m_sources[ audio ] = source;
        m_audioEntityUpdateIDMap[ audio ] = 0;
    }
    else
    {
        source = m_sources[ audio ];
    }

    return source;
}

// *********************************
//
PdrAudioBuffersQueue *  AudioRenderer::GetPdrAudioBuffersQueue      ( PdrSource * source, const AudioEntity * audio )
{
    PdrAudioBuffersQueue * queue = nullptr;

    if( !m_bufferMap.count( source ) )
    {
        queue = new PdrAudioBuffersQueue( source->GetHandle(), audio->GetFrequency(), audio->GetFormat() );
        m_bufferMap[ source ] = queue;
    }
    else
    {
        queue = m_bufferMap[ source ];
    }

    return queue;
}

} // audio
} //bv
