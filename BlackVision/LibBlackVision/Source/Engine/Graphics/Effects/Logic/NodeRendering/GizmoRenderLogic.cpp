#include "stdafx.h"

#include "NodeRenderLogic.h"

#include "Engine/Graphics/SceneGraph/Scene.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/SceneNodeRepr.h"
#include "Engine/Graphics/SceneGraph/RenderableEntityWithBoundingBox.h"

#include "Engine/Graphics/Effects/Logic/Components/RenderContext.h"
#include "GizmoRenderLogic.h"



namespace bv
{


// ***********************
//
void            GizmoRenderLogic::Render            ( Scene * scene, const RenderTarget * output, RenderContext * ctx )
{
    auto renderer = ctx->GetRenderer();
    renderer->SetCamera( scene->GetCamera() );

    // FIXME: nrl - reimplement it (do not use/preferably remove EnableScene which contains logic <implemented in renderer> 
    // FIXME: nrl - which does not belong there)
    renderer->EnableScene( scene );

    enable( ctx, output );
    GizmoRenderLogic::RenderTree( scene->GetRoot(), ctx );

    disableBoundRT( ctx );
}


// ***********************
//
void            GizmoRenderLogic::RenderTree        ( SceneNode * node, RenderContext * ctx )
{
    Render( node, ctx );

    for( UInt32 i = 0; i < node->NumChildNodes(); i++ )
    {
        RenderTree( node->GetChild( i ), ctx );
    }
}

// ***********************
//
void            GizmoRenderLogic::Render            ( SceneNode * node, RenderContext * ctx )
{
    for( UInt32 i = 0; i < node->GetNumGizmos(); i++ )
    {
        auto gizmoRoot = node->GetGizmo( i );
        assert( gizmoRoot );

        NodeRenderLogic::Render( gizmoRoot, ctx );
        NodeRenderLogic::RenderChildren( gizmoRoot->GetRepr(), ctx );
    }
}

}	// bv

