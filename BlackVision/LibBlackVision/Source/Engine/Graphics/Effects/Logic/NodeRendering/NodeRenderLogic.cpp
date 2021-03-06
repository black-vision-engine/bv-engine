#include "stdafx.h"

#include "NodeRenderLogic.h"

#include "Engine/Graphics/SceneGraph/Scene.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/SceneNodeRepr.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Engine/Graphics/Effects/Logic/Components/RenderContext.h"
#include "Engine/Graphics/Effects/Utils/RenderableEffectFactory.h"

#include "Engine/Audio/AudioRenderer.h"


namespace bv { 

// *********************************
//
void    NodeRenderLogic::Clear             ( const RenderTarget * rt, RenderContext * ctx, const glm::vec4 & color )
{
    enable( ctx, rt );
    clearBoundRT( ctx, color );
    disableBoundRT( ctx );
}

// *********************************
//
void    NodeRenderLogic::RenderAudio       ( Scene * scene, RenderContext * ctx, audio::AudioRenderChannelData & arcd )
{
    auto rootNode = scene->GetRoot();

    if( rootNode )
        RenderAudio( rootNode, audio_renderer( ctx ), arcd );
}

// *********************************
//
void    NodeRenderLogic::RenderAudio       ( SceneNode * node, audio::AudioRenderer * audio, audio::AudioRenderChannelData & arcd )
{
    // FIXME: nrl - and what about node effects???
    if ( node->IsVisible() )
    {
        auto nodeAudio = node->GetAudio();

        if( nodeAudio )
            audio->Proccess( nodeAudio, arcd );

        for( unsigned int i = 0; i < ( UInt32 )node->NumChildNodes(); ++i )
            RenderAudio( node->GetChild( i ), audio, arcd );
    }
}

// *********************************
//
void    NodeRenderLogic::RenderQueued      ( Scene * scene, const RenderTarget * output, RenderContext * ctx )
{
    auto renderer = ctx->GetRenderer();

    renderer->Performance().AverageScenePerformanceData( scene );
    renderer->SetCamera( scene->GetCamera() );

    // FIXME: nrl - reimplement it (do not use/preferably remove EnableScene which contains logic <implemented in renderer> 
    // FIXME: nrl - which does not belong there)
    renderer->EnableScene( scene );

    enable( ctx, output );
    RenderQueued( scene->GetRoot(), ctx );

    disableBoundRT( ctx );
}

// *********************************
//
void    NodeRenderLogic::RenderQueued      ( SceneNode * node, RenderContext * ctx )
{
    // FIXME: nrl - remove this method and implement its logic in some other place (if necessary)
    auto queue = queue_allocator( ctx )->Allocate();
   
    queue->QueueNodeSubtree( node, ctx );
   
    queue->Render( ctx );

    queue_allocator( ctx )->Free();
}

// *********************************
//
void    NodeRenderLogic::Render            ( SceneNode * node, RenderContext * ctx )
{
    if( node->IsVisible() )
    {
        RenderImpl( node, ctx );
    }
}

// *********************************
//
void    NodeRenderLogic::Render            ( SceneNode * node, const RenderTarget * output, RenderContext * ctx )
{
    if ( node->IsVisible() )
    {
        enable( ctx, output );
        clearBoundRT( ctx, glm::vec4() ); // FIXME: default clear color used - posisibly customize it a bit

        RenderImpl( node, ctx );

        disableBoundRT( ctx );
    }
}

// *********************************
//
void     NodeRenderLogic::RenderImpl      ( SceneNode * node, RenderContext * ctx )
{
    // FIXME: nrl transition
    auto nEffect = node->IsNodeEffectEnabled() ? node->GetNodeEffect() : nullptr;
    auto repr   = node->GetRepr();

    if( nEffect )
    {
        // FIXME: nrl - remove whem all effects are implemented in nrl
        //RenderImpl( repr, ctx );
        assert( nEffect->GetType() == NodeEffectType::NET_DEFAULT ||
				nEffect->GetType() == NodeEffectType::NET_ALPHA_MASK ||
				nEffect->GetType() == NodeEffectType::NET_NODE_MASK ||
				nEffect->GetType() == NodeEffectType::NET_BLUR ||
				nEffect->GetType() == NodeEffectType::NET_LIGHT_SCATTERING ||
				nEffect->GetType() == NodeEffectType::NET_SHADOW ||
                nEffect->GetType() == NodeEffectType::NET_Z_SORT ||
				nEffect->GetType() == NodeEffectType::NET_GLOW || 
				nEffect->GetType() == NodeEffectType::NET_SOFT_MASK ||
                nEffect->GetType() == NodeEffectType::NET_COLOR_CORRECTION
				);

        nEffect->Render( repr, ctx ); //FIXME: test and implement
    }
    else 
    {
        NodeRenderLogic::RenderRoot( node->GetRepr(), ctx );
        // FIXME: nrl - transition implementations
        // assert( false );
    }
}

// *********************************
//
void    NodeRenderLogic::Render            ( SceneNodeRepr * nodeRepr, const RenderTarget * output, RenderContext * ctx )
{
    enable( ctx, output );
    clearBoundRT( ctx, glm::vec4() ); // FIXME: default clear color used - posisibly customize it a bit

    RenderImpl( nodeRepr, ctx );
	renderer( ctx )->GenerateMipmaps( output );

    disableBoundRT( ctx );    
}

// ***********************
//
void    NodeRenderLogic::RenderQueued      ( SceneNodeRepr * nodeRepr, const RenderTarget * output, RenderContext * ctx )
{
    enable( ctx, output );
    clearBoundRT( ctx, glm::vec4() ); // FIXME: default clear color used - posisibly customize it a bit

    RenderQueued( nodeRepr, ctx );

    disableBoundRT( ctx );
}

// ***********************
//
void    NodeRenderLogic::RenderQueued      ( SceneNodeRepr * nodeRepr, RenderContext * ctx )
{
    // FIXME: nrl - remove this method and implement its logic in some other place (if necessary)
    auto queue = queue_allocator( ctx )->Allocate();

    queue->QueueNodeSubtree( nodeRepr, ctx );

    queue->Render( ctx );

    queue_allocator( ctx )->Free();
}

// *********************************
//
void    NodeRenderLogic::Render            ( SceneNodeRepr * nodeRepr, RenderContext * ctx )
{
    RenderImpl( nodeRepr, ctx );
}

// *********************************
//
void    NodeRenderLogic::RenderImpl        ( SceneNodeRepr * nodeRepr, RenderContext * ctx )
{
    RenderRoot( nodeRepr, ctx );

    RenderChildren( nodeRepr, ctx );
}

// *********************************
//
void    NodeRenderLogic::RenderRoot        ( SceneNodeRepr * nodeRepr, RenderContext * ctx )
{
    renderer( ctx )->Draw( renderable( nodeRepr ) );
}

// *********************************
//
void    NodeRenderLogic::RenderChildren    ( SceneNodeRepr * nodeRepr, RenderContext * ctx, unsigned int firstChildIdx )
{
    for ( unsigned int i = firstChildIdx; i < (unsigned int) nodeRepr->NumChildNodes(); i++ )
    {
        Render( nodeRepr->GetChild( i ), ctx ); 
    }
}


} // bv
