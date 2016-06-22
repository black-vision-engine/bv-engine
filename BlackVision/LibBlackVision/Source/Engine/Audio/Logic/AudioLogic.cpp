#include "stdafx.h"

#include "AudioLogic.h"

#include "BVAL.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Audio/AudioRenderer.h"

#include "UseLoggerLibBlackVision.h"


namespace bv { namespace audio {


// *********************************
//
	    AudioLogic::AudioLogic	    ()
{
}

// *********************************
//
	    AudioLogic::~AudioLogic	    ()
{
}

// *********************************
//
void	AudioLogic::Play	        ( AudioRenderer * renderer, const SceneVec & scenes )
{
    for( auto & scene : scenes )
    {
        Play( renderer, scene->GetRoot() );
    }
}

// *********************************
//
void	AudioLogic::Play	        ( AudioRenderer * renderer, SceneNode * node )
{
    if ( node->IsVisible() )
    {
        auto audio = node->GetAudio();
        if( audio )
        {
            renderer->Play( audio );
        }

        for( unsigned int i = 0; i < ( UInt32 )node->NumChildNodes(); ++i )
        {
            Play( renderer, node->GetChild( i ) );
        }
    }
}

} // audio
} //bv
