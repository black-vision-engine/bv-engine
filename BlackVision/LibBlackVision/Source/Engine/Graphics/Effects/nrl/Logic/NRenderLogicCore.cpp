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
    // Invalidate all active output channels
    PreRender   ( result );

    // Render scenes to appropriate channels
    RenderScenes( scenes, result, ctx );

    // Clear all active channels that are still invalid
    // FIXME: nrl - one minor optimization is to skip clearing if a channel is already cleared but that is not very important
    // FIXME: nrl - as all active channels are supposed to be cleared by RenderScenes() anyway
    PostRender  ( result, ctx );
}

// **************************
//
void    NRenderLogicCore::RenderScenes      ( const SceneVec & scenes, RenderResult * result, NRenderContext * ctx )
{
    // FIXME: nrl - is this the correct logic (to switch output channel per scene and not per scene group which belongs to a channel)
    for( auto & scene : scenes )
    {
        auto outIdx = scene->GetOutputChannelIdx();

        assert( outIdx < (unsigned int) RenderOutputChannelType::ROCT_TOTAL );

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
        if( result->IsActive( channelType ) )
        {
            result->SetContainsValidData( false );
        }
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

} //nrl
} //bv
