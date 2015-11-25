#include "AlphaMaskRenderLogic.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"

#include "Rendering/Utils/OffscreenRenderLogic.h"

#include "Rendering/Logic/FrameRendering/FrameRenderLogic.h"


namespace bv {

// *********************************
//
AlphaMaskRenderLogic::AlphaMaskRenderLogic        ( FrameRenderLogic * renderLogic, OffscreenRenderLogic * offscreenRenderLogic )
    : NodeEffectRenderLogic( renderLogic, offscreenRenderLogic )
{ 
}

// *********************************
//
AlphaMaskRenderLogic::~AlphaMaskRenderLogic       ()
{
}

// *********************************
//
void    AlphaMaskRenderLogic::RenderNode                  ( Renderer * renderer, SceneNode * node )
{
    auto alphaVal = node->GetNodeEffect()->GetValue( "alpha" );
    auto alphaValue = QueryTypedValue< ValueFloatPtr >( alphaVal )->GetValue();

    if( alphaValue < 0.99f )
    {
        GetOffscreenRenderLogic()->AllocateNewRenderTarget( renderer );
        GetOffscreenRenderLogic()->EnableTopRenderTarget( renderer );

        renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );
        renderer->ClearBuffers();

        GetRenderLogic()->DrawNode( renderer, node );

        auto alphaVal = node->GetNodeEffect()->GetValue( "alpha" );
        GetOffscreenRenderLogic()->DrawTopAuxRenderTarget( renderer, alphaVal.get() );
        GetOffscreenRenderLogic()->DiscardCurrentRenderTarget( renderer );
        GetOffscreenRenderLogic()->EnableTopRenderTarget( renderer );
    }
    else
    {
        GetRenderLogic()->DrawNode( renderer, node );
    }
}

} //bv
