#include "stdafx.h"

#include "NRenderLogicCore.h"

#include "Engine/Graphics/Effects/nrl/Logic/NRenderLogicImpl.h"
#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/RenderChannel.h"
#include "Engine/Graphics/Effects/nrl/Logic/NRenderContext.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NNodeRenderLogic.h"


namespace bv { namespace nrl {

// **************************
//
NRenderLogicCore::NRenderLogicCore()
    : m_allChannels( (unsigned int) RenderChannelType::RCT_TOTAL )
{
    m_allChannels[ (unsigned int) RenderChannelType::RCT_OUTPUT_1 ] = RenderChannelType::RCT_OUTPUT_1;
    m_allChannels[ (unsigned int) RenderChannelType::RCT_OUTPUT_2 ] = RenderChannelType::RCT_OUTPUT_2;
    m_allChannels[ (unsigned int) RenderChannelType::RCT_OUTPUT_3 ] = RenderChannelType::RCT_OUTPUT_3;
    m_allChannels[ (unsigned int) RenderChannelType::RCT_OUTPUT_4 ] = RenderChannelType::RCT_OUTPUT_4;

    assert( (unsigned int) RenderChannelType::RCT_OUTPUT_4 == (unsigned int) RenderChannelType::RCT_TOTAL - 1 );
}

// **************************
//
void    NRenderLogicCore::Render    ( const SceneVec & scenes, RenderResult * result, NRenderContext * ctx )
{
    // Invalidate all active output channels
    PreRender   ( result );

    // Render scenes to appropriate channels
    RenderScenes( scenes, result, ctx );

    // Clear all active channels that are still invalid, invalidate readback textures
    // FIXME: nrl - one minor optimization is to skip clearing if a channel is already cleared but that is not very important
    // FIXME: nrl - as all active channels are supposed to be cleared by RenderScenes() anyway
    PostRender  ( result, ctx );
}

// **************************
//
void    NRenderLogicCore::RenderScenes      ( const SceneVec & scenes, RenderResult * result, NRenderContext * ctx )
{
    // Clear render targets before rendering. Note: Clearing can't be made by RenderScene functions, because we would
    // override previously rendered scene. We have to do it here.
    for( auto channelType : m_allChannels )
    {
        if( result->IsActive( channelType ) )
        {
            auto rt = result->GetActiveRenderTarget( channelType );
            NNodeRenderLogic::Clear( rt, ctx );
        }
    }

    // FIXME: nrl - is this the correct logic (to switch output channel per scene and not per scene group which belongs to a channel)
    for( auto & scene : scenes )
    {
        auto outIdx = scene->GetOutputChannelIdx(); // FIXME: nrl - this mapping should be strictly typed
        assert( outIdx < (unsigned int) RenderChannelType::RCT_TOTAL );

        auto outputType = ( RenderChannelType ) outIdx;   
        auto outputRT = result->GetActiveRenderTarget( outputType );

        RenderScene( scene, outputRT, ctx );

        result->SetContainsValidData( outputType, true );
    }
}

// **************************
//
void    NRenderLogicCore::RenderScene       ( Scene * scene, const RenderTarget * outputRT, NRenderContext * ctx )
{
    NNodeRenderLogic::RenderQueued( scene, outputRT, ctx );
    NNodeRenderLogic::RenderAudio( scene, ctx );
}

// **************************
//
void    NRenderLogicCore::PreRender         ( RenderResult * result )
{
    for( auto channelType : m_allChannels )
    {
        if( result->IsActive( channelType ) )
        {
            result->SetContainsValidData( channelType, false );
        }
    }
}

// **************************
//
void    NRenderLogicCore::PostRender        ( RenderResult * result, NRenderContext * ctx )
{
    for( auto channelType : m_allChannels )
    {
        if( result->IsActive( channelType ) )
        {
            if( !result->ContainsValidData( channelType ) )
            {
                auto rt = result->GetActiveRenderTarget( channelType );

                NNodeRenderLogic::Clear( rt, ctx );

				result->SetContainsValidData( channelType, true );
            }

            result->InvalidateCachedTexture( channelType );
        }
    }
}

} //nrl
} //bv
