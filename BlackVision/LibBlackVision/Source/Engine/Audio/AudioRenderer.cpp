#include "stdafx.h"

#include "AudioRenderer.h"

#include "BVAL.h"

#include "Engine/Audio/OALRenderer/PdrAudioBuffersQueue.h"
#include "Engine/Audio/OALRenderer/PdrSource.h"
#include "Engine/Graphics/SceneGraph/AudioEntity.h"

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
    PdrSource * source = nullptr; 
    PdrAudioBuffersQueue * queue = nullptr;

    if( !m_sources.count( audio ) )
    {
        source = new PdrSource();
        m_sources[ audio ] = source;
        
        queue = new PdrAudioBuffersQueue();
        m_bufferMap[ source ] = queue;

    }
    else
    {
        source = m_sources[ audio ];
        queue = m_bufferMap[ source ];
    }

    if( !audio->IsEmpty() )
    {
        queue->PushData( audio->PopData() );
    }

    if( queue->BufferData( source ) )
    {
        source->Play();
    }
}

} // audio
} //bv
