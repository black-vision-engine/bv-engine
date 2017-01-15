#include "stdafx.h"

#include "NRenderLogicCore.h"

#include "Engine/Graphics/Effects/nrl/Logic/NRenderLogicImpl.h"
#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/RenderOutputChannel.h"
#include "Engine/Graphics/Effects/nrl/Logic/NRenderContext.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NNodeRenderLogic.h"


namespace bv { namespace nrl {

// **************************
//
NRenderLogicCore::NRenderLogicCore()
    : m_allChannels( (unsigned int) RenderOutputChannelType::ROCT_TOTAL )
{
    m_allChannels[ (unsigned int) RenderOutputChannelType::ROCT_OUTPUT_1 ] = RenderOutputChannelType::ROCT_OUTPUT_1;
    m_allChannels[ (unsigned int) RenderOutputChannelType::ROCT_OUTPUT_2 ] = RenderOutputChannelType::ROCT_OUTPUT_2;
    m_allChannels[ (unsigned int) RenderOutputChannelType::ROCT_OUTPUT_3 ] = RenderOutputChannelType::ROCT_OUTPUT_3;
    m_allChannels[ (unsigned int) RenderOutputChannelType::ROCT_OUTPUT_4 ] = RenderOutputChannelType::ROCT_OUTPUT_4;

    assert( (unsigned int) RenderOutputChannelType::ROCT_OUTPUT_4 == (unsigned int) RenderOutputChannelType::ROCT_TOTAL - 1 );
}

// **************************
//
void    NRenderLogicCore::Render    ( const SceneVec & scenes, RenderResult * result, NRenderContext * ctx )
{
    PreRender   ( result );

    RenderScenes( scenes, result, ctx );

    PostRender  ( result, ctx );
}

// **************************
//
void    NRenderLogicCore::RenderScenes      ( const SceneVec & scenes, RenderResult * result, NRenderContext * ctx )
{
    for( auto & scene : scenes )
    {
        // FIXME: nrl - retrieve rt for each scene here (for each scene marked as a subgroup)
        auto rt = result->GetActiveRenderTarget( RenderOutputChannelType::ROCT_OUTPUT_1 );

        NNodeRenderLogic::RenderQueued( scene, rt, ctx );
        NNodeRenderLogic::RenderAudio( scene, ctx );
    }
}

// **************************
//
void    NRenderLogicCore::PreRender         ( RenderResult * result )
{
    // FIXME: nrl - this is a bit of an overkill (not too expensive though) to update this state each frame instead of tracking it and updating only when a scene is (or scenes are) changed
    // FIXME: nrl - only then this state should be updated

    // Clear render result state

    for( auto channelType : m_allChannels )
    {
        ClearChannelState( result, channelType );
    }
}

// **************************
//
void    NRenderLogicCore::PostRender        ( RenderResult * result, NRenderContext * ctx )
{
    // FIXME: nrl - this is a bit of an overkill (not too expensive though) to update this state each frame instead of tracking it and updating only when a scene is (or scenes are) changed
    // FIXME: nrl - only then this state should be updated

    // Clear output render targets which were not used durint the RenderScenes phase
}

// **************************
//
void    NRenderLogicCore::ClearChannelState ( RenderResult * result, RenderOutputChannelType roct )
{
}

} //nrl
} //bv
