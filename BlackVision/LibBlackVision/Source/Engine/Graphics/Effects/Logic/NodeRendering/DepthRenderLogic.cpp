#include "stdafx.h"

#include "DepthRenderLogic.h"
#include "NodeRenderLogic.h"

#include "Engine/Graphics/SceneGraph/Scene.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/SceneNodeRepr.h"
#include "Engine/Graphics/SceneGraph/RenderableEntityWithBoundingBox.h"

#include "Engine/Graphics/Effects/Logic/Components/RenderContext.h"



namespace bv
{


// ***********************
//
void            DepthRenderLogic::RenderQueued      ( Scene * scene, const RenderTarget * output, RenderContext * ctx )
{
    auto renderer = ctx->GetRenderer();
    renderer->SetCamera( scene->GetCamera() );

    // FIXME: nrl - reimplement it (do not use/preferably remove EnableScene which contains logic <implemented in renderer> 
    // FIXME: nrl - which does not belong there)
    renderer->EnableScene( scene );

    ctx->EnableDepthOnly( output );
    DepthRenderLogic::RenderQueued( scene->GetRoot(), ctx );

    disableBoundRT( ctx );
}

// ***********************
//
void            DepthRenderLogic::RenderQueued      ( SceneNode * node, RenderContext * ctx )
{
    auto queue = queue_allocator( ctx )->Allocate();

    queue->QueueNodeSubtree( node, ctx );
    queue->RenderDepth( ctx );

    queue_allocator( ctx )->Free();
}

// ***********************
//
void            DepthRenderLogic::Render            ( SceneNode * node, RenderContext * ctx )
{
    if( node->IsVisible() )
    {
        RenderImpl( node, ctx );
    }
}

// ***********************
//
void            DepthRenderLogic::RenderImpl        ( SceneNode * node, RenderContext * ctx )
{
    // FIXME: nrl transition
    auto nEffect = node->IsNodeEffectEnabled() ? node->GetNodeEffect() : nullptr;
    auto repr = node->GetRepr();

    if( nEffect )
    {
        nEffect->RenderDepth( repr, ctx ); //FIXME: test and implement
    }
    else
    {
        NodeRenderLogic::RenderRoot( node->GetRepr(), ctx );
    }
}


}	// bv


