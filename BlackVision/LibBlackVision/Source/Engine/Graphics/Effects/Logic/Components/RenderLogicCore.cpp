#include "stdafx.h"

#include "RenderLogicCore.h"

#include "Engine/Graphics/Effects/Logic/RenderLogicImpl.h"

#include "Engine/Graphics/Effects/Logic/Components/RenderChannel.h"
#include "Engine/Graphics/Effects/Logic/Components/RenderContext.h"

#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeRenderLogic.h"

#include "Engine/Graphics/Effects/Logic/FullscreenRendering/FullscreenEffectFactory.h"

#include "Engine/Graphics/Effects/Logic/State/RenderedData.h"


namespace bv { 

// **************************
//
RenderLogicCore::RenderLogicCore()
    : m_allChannels( (unsigned int) RenderChannelType::RCT_TOTAL )
{
    m_allChannels[ (unsigned int) RenderChannelType::RCT_OUTPUT_1 ] = RenderChannelType::RCT_OUTPUT_1;
    m_allChannels[ (unsigned int) RenderChannelType::RCT_OUTPUT_2 ] = RenderChannelType::RCT_OUTPUT_2;
    m_allChannels[ (unsigned int) RenderChannelType::RCT_OUTPUT_3 ] = RenderChannelType::RCT_OUTPUT_3;
    m_allChannels[ (unsigned int) RenderChannelType::RCT_OUTPUT_4 ] = RenderChannelType::RCT_OUTPUT_4;

    assert( (unsigned int) RenderChannelType::RCT_OUTPUT_4 == (unsigned int) RenderChannelType::RCT_TOTAL - 1 );

    m_blitWithAlphaEffect = CreateFullscreenEffect( FullscreenEffectType::NFET_BLIT_WITH_ALPHA );
}

// **************************
//
void    RenderLogicCore::Render    ( const SceneVec & scenes, RenderedChannelsData * result, RenderContext * ctx )
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
void    RenderLogicCore::RenderScenes      ( const SceneVec & scenes, RenderedChannelsData * result, RenderContext * ctx )
{
    // Clear render targets before rendering. Note: Clearing can't be made by RenderScene functions, because we would
    // override previously rendered scene. We have to do it here.
    ClearActiveChannels( result, ctx );

    std::vector< RenderTarget * > rts;

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
void    RenderLogicCore::RenderScene       ( Scene * scene, const RenderTarget * outputRT, RenderContext * ctx )
{
    NodeRenderLogic::RenderQueued( scene, outputRT, ctx );
    NodeRenderLogic::RenderAudio( scene, ctx );
}

// **************************
//
void    RenderLogicCore::ClearActiveChannels   ( RenderedChannelsData * result, RenderContext * ctx )
{
    for( auto channelType : m_allChannels )
    {
        if( result->IsActive( channelType ) )
        {
            auto rt = result->GetActiveRenderTarget( channelType );

            NodeRenderLogic::Clear( rt, ctx );
        }
    }
}

// **************************
//
void    RenderLogicCore::PreRender         ( RenderedChannelsData * result )
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
void    RenderLogicCore::PostRender        ( RenderedChannelsData * result, RenderContext * ctx )
{
    for( auto channelType : m_allChannels )
    {
        if( result->IsActive( channelType ) )
        {
            if( !result->ContainsValidData( channelType ) )
            {
                auto rt = result->GetActiveRenderTarget( channelType );

                NodeRenderLogic::Clear( rt, ctx );

				result->SetContainsValidData( channelType, true );
            }

            result->InvalidateCachedTexture( channelType );
        }
    }
}


} //bv
