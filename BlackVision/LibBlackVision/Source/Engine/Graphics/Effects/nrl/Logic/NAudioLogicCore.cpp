#include "stdafx.h"

#include "NAudioLogicCore.h"

#include "Engine/Audio/AudioRenderer.h"

#include "Engine/Graphics/Effects/nrl/Logic/NRenderLogicImpl.h"
#include "Engine/Graphics/Effects/nrl/Logic/NRenderContext.h"


namespace bv { namespace nrl {

// **************************
//
void    NAudioLogicCore::RenderAudio        ( SceneNode * root, NRenderContext * ctx )
{
    if( root )
    {
        RenderAudioImpl( root, audio( ctx ) );
    }
}

// **************************
//
void    NAudioLogicCore::RenderAudioImpl    ( SceneNode * node, audio::AudioRenderer * renderer )
{
    // FIXME: and what about node effects???
    if ( node->IsVisible() )
    {
        auto audio = node->GetAudio();

        if( audio )
        {
            renderer->Proccess( audio );
        }

        for( unsigned int i = 0; i < ( UInt32 )node->NumChildNodes(); ++i )
        {
            RenderAudioImpl( node->GetChild( i ), renderer );
        }
    }
}

} //nrl
} //bv
