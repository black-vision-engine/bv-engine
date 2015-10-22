#include "NodeMaskRenderLogic.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"

#include "Rendering/OffscreenRenderLogic.h"

#include "Rendering/Logic/RenderLogic.h"


namespace bv {

// *********************************
//
NodeMaskRenderLogic::NodeMaskRenderLogic         ( RenderLogic * renderLogic, OffscreenRenderLogic * offscreenRenderLogic )
    : NodeEffectRenderLogic( renderLogic, offscreenRenderLogic )
{
}

// *********************************
//
NodeMaskRenderLogic::~NodeMaskRenderLogic        ()
{
}

// *********************************
//
void    NodeMaskRenderLogic::RenderNode                  ( Renderer * renderer, SceneNode * node )
{
    if( node->NumChildNodes() < 2 )
    {
        GetRenderLogic()->DrawNode( renderer, node );
    }
    else
    {
        GetRenderLogic()->DrawNodeOnly( renderer, node );

        GetOffscreenRenderLogic()->AllocateNewRenderTarget( renderer );
        GetOffscreenRenderLogic()->EnableTopRenderTarget( renderer );

        renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );
        renderer->ClearBuffers();
        
        // MASK
        GetRenderLogic()->RenderNode( renderer, node->GetChild( 1 ) ); 

        GetOffscreenRenderLogic()->AllocateNewRenderTarget( renderer );
        GetOffscreenRenderLogic()->EnableTopRenderTarget( renderer );
        renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );
        renderer->ClearBuffers();

        // FOREGROUND
        GetRenderLogic()->RenderNode( renderer, node->GetChild( 0 ) ); 

        GetOffscreenRenderLogic()->DrawAMTopTwoRenderTargets( renderer, node->GetOverrideAlphaVal() );
    
        GetOffscreenRenderLogic()->DiscardCurrentRenderTarget( renderer );
        GetOffscreenRenderLogic()->DiscardCurrentRenderTarget( renderer );

        GetOffscreenRenderLogic()->EnableTopRenderTarget( renderer );

        GetRenderLogic()->DrawChildren( renderer, node, 2 );
    }
}

} //bv
