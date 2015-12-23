#include "AlphaChannelRenderLogic.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"
#include "Engine/Graphics/Resources/RenderTarget.h"

#include "Rendering/Utils/OffscreenRenderLogic.h"

#include "Rendering/Logic/RenderLogic.h"



namespace bv {

// *********************************
//
AlphaChannelRenderLogic::AlphaChannelRenderLogic        ()
	: m_effect( nullptr )
{
	m_effect = std::make_shared< AlphaChannelEffect >();
}

// *********************************
//
AlphaChannelRenderLogic::~AlphaChannelRenderLogic       ()
{
}

// *********************************
//
void    AlphaChannelRenderLogic::RenderNode             ( SceneNode * , RenderLogicContext *   )
// void    AlphaChannelRenderLogic::RenderNode             ( Renderer * renderer, SceneNode * node )
{
	//GetOffscreenRenderLogic()->AllocateNewRenderTarget( renderer );
	//GetOffscreenRenderLogic()->EnableTopRenderTarget( renderer );
	//
	//renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.f ) );
	//renderer->ClearBuffers();

 //   DrawNodeOnly( renderer, node );
	//DrawChildren( renderer, node );

 //   auto texture = GetOffscreenRenderLogic()->GetColorTextureAt( -1 );
	//m_effect->SetTexture( std::const_pointer_cast< Texture2D >( texture ) );
	//
	//GetOffscreenRenderLogic()->DrawTopAuxRenderTarget( renderer, m_effect );
	// 
	//GetOffscreenRenderLogic()->DiscardCurrentRenderTarget( renderer );
	//GetOffscreenRenderLogic()->EnableTopRenderTarget( renderer );

}

// *********************************
//
void    AlphaChannelRenderLogic::DrawNodeOnly      ( Renderer * renderer, SceneNode * node )
{
    auto renderable = static_cast< bv::RenderableEntity * >( node->GetTransformable() );

    renderer->Draw( renderable );
}

// *********************************
//
void    AlphaChannelRenderLogic::DrawChildren       ( Renderer * renderer, SceneNode * node )
{
    for ( unsigned int i = 0; i < ( unsigned int )node->NumChildNodes(); ++i )
    {
        DrawNodeOnly( renderer, node->GetChild( i ) ); 
        DrawChildren( renderer, node->GetChild( i ) );
    }
}

} //bv
