#include "stdafx.h"

#include "NNodeRenderLogic.h"

#include "Engine/Graphics/SceneGraph/Scene.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/SceneNodeRepr.h"

#include "Engine/Graphics/Effects/nrl/Logic/NRenderContext.h"

#include "Engine/Audio/AudioRenderer.h"


namespace bv { namespace nrl {

// *********************************
//
void    NNodeRenderLogic::RenderAudio       ( Scene * scene, NRenderContext * ctx )
{
    auto rootNode = scene->GetRoot();

    if( rootNode )
    {
        RenderAudio( rootNode, audio( ctx ) );
    }
}

// *********************************
//
void    NNodeRenderLogic::RenderAudio       ( SceneNode * node, audio::AudioRenderer * audio )
{
    // FIXME: nrl - and what about node effects???
    if ( node->IsVisible() )
    {
        auto nodeAudio = node->GetAudio();

        if( nodeAudio )
        {
            audio->Proccess( nodeAudio );
        }

        for( unsigned int i = 0; i < ( UInt32 )node->NumChildNodes(); ++i )
        {
            RenderAudio( node->GetChild( i ), audio );
        }
    }
}

// *********************************
//
void    NNodeRenderLogic::RenderQueued      ( Scene * scene, const RenderTarget * output, NRenderContext * ctx )
{
    auto renderer = ctx->GetRenderer();

    renderer->Performance().AverageScenePerformanceData( scene );
    renderer->SetCamera( scene->GetCamera() );

    // FIXME: nrl - reimplement it somehow
    renderer->EnableScene( scene );

    RenderQueued( scene->GetRoot(), output, ctx );
}

// *********************************
//
void    NNodeRenderLogic::RenderQueued      ( SceneNode * node, const RenderTarget * output, NRenderContext * ctx )
{
    enable( ctx, output );

    // FIXME: default clear color used - posisibly customize it a bit;
    clearBoundRT( ctx, glm::vec4() ); 

    // FIXME: nrl - remove this method and implement its logic in some other place
 
    auto queue = queue_allocator( ctx )->Allocate();
    
    queue->QueueNodeSubtree( node, ctx );
    
    queue->Render( ctx );

    queue_allocator( ctx )->Free();

    // FIXME: nrl - implement it back
//    RenderGridLines( scene, ctx );          // FIXME: Use some generic solution when other editor helper object apear in engine.

    disableBoundRT( ctx );
}

// *********************************
//
void    NNodeRenderLogic::Render            ( SceneNode * node, NRenderContext * ctx )
{
    if( node->IsVisible() )
    {
        RenderImpl( node, ctx );
    }
}

// *********************************
//
void    NNodeRenderLogic::Render            ( SceneNode * node, const RenderTarget * output, NRenderContext * ctx )
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
void     NNodeRenderLogic::RenderImpl      ( SceneNode * node, NRenderContext * ctx )
{
    // FIXME: nrl transition
    auto nEffect = node->GetNNodeEffect();
    auto repr   = node->GetRepr();

    if( nEffect )
    {
        //RenderImpl( repr, ctx );
        assert( nEffect->GetType() == nrl::NNodeEffectType::NNET_DEFAULT || nEffect->GetType() == nrl::NNodeEffectType::NNET_ALPHA_MASK );

        nEffect->Render( repr, ctx ); //FIXME: test and implement
    }
    else 
    {   //Transition implementations
        auto effect = node->GetNodeEffect();

        if( !effect || effect->GetType() ==  NodeEffectType::NET_DEFAULT )
        {
            RenderImpl( repr, ctx );
        }
        else
        {
            effect->Render( repr, ctx );
        }
    }
}

// *********************************
//
void    NNodeRenderLogic::Render            ( SceneNodeRepr * nodeRepr, const RenderTarget * output, NRenderContext * ctx )
{
    enable( ctx, output );
    clearBoundRT( ctx, glm::vec4() ); // FIXME: default clear color used - posisibly customize it a bit

    RenderImpl( nodeRepr, ctx );

    disableBoundRT( ctx );    
}

// *********************************
//
void    NNodeRenderLogic::Render            ( SceneNodeRepr * nodeRepr, NRenderContext * ctx )
{
    RenderImpl( nodeRepr, ctx );
}

// *********************************
//
void    NNodeRenderLogic::RenderImpl        ( SceneNodeRepr * nodeRepr, NRenderContext * ctx )
{
    RenderRoot( nodeRepr, ctx );

    RenderChildren( nodeRepr, ctx );
}

// *********************************
//
void    NNodeRenderLogic::RenderRoot        ( SceneNodeRepr * nodeRepr, NRenderContext * ctx )
{
    renderer( ctx )->Draw( renderable( nodeRepr ) );
}

// *********************************
//
void    NNodeRenderLogic::RenderChildren    ( SceneNodeRepr * nodeRepr, NRenderContext * ctx, unsigned int firstChildIdx )
{
    for ( unsigned int i = firstChildIdx; i < (unsigned int) nodeRepr->NumChildNodes(); i++ )
    {
        Render( nodeRepr->GetChild( i ), ctx ); 
    }
}

} // nrl
} // bv
