#include "EffectRenderLogic.h"

#include "Engine/Graphics/Resources/RenderTarget.h"
#include "Engine/Graphics/Renderers/Renderer.h"

#include "Engine/Graphics/SceneGraph/MainDisplayTarget.h"
#include "Engine/Graphics/SceneGraph/Camera.h"
#include "Engine/Graphics/SceneGraph/TriangleStrip.h"

#include "Engine/Graphics/Effects/Texture2DEffect.h"

namespace bv {

// ************************************************* RENDER TARGET DATA ***********************************************************

// **************************
//
RenderTargetData::RenderTargetData      ()
    : renderTarget( nullptr )
    , quad( nullptr )
{
}

// **************************
//
RenderTargetData::~RenderTargetData     ()
{
}


// **************************************************** TEXTURE DATA **************************************************************

// **************************
//
TextureData::TextureData( unsigned int width, unsigned int height, TextureFormat fmt )
{
    m_width     = width;
    m_height    = height;
    m_fmt       = fmt;

}


// *********************************************** EFFECT RENDER DATA **********************************************************

// **************************
//
EffectRenderData::EffectRenderData(  const RenderableEffectPtr & e  )
{
    auxQuad                 = MainDisplayTarget::CreateDisplayRect( nullptr );
	effect					= e;
	auxQuad->SetRenderableEffect( e );

    std::vector< bv::Transform > vec;
    vec.push_back( Transform( glm::mat4( 1.0f ), glm::mat4( 1.0f ) ) );

    auxQuad->SetWorldTransforms( vec );
}

// *********************************************** EFFECT RENDER LOGIC *********************************************************

// **************************
//
EffectRenderLogic::EffectRenderLogic   ( unsigned int width, unsigned int height, const RenderableEffectPtr & effect, Camera * camera, TextureFormat fmt )
    : m_textureData( width, height, fmt )
    , m_displayRTEnabled( false )
	, m_renderData( effect )
	, m_Camera( camera )
	, m_readbackTexture( nullptr )
{
    m_renderTargetData = CreateRenderTargetData();
	m_renderTarget = m_renderTargetData.renderTarget;
}

// **************************
//
EffectRenderLogic::~EffectRenderLogic  ()
{
	delete m_renderTargetData.quad;
	delete m_renderTargetData.renderTarget;

    delete m_Camera;
}

// **************************
//
void                EffectRenderLogic::Draw   ( Renderer * renderer )
{
    assert( m_displayRTEnabled == false );

	auto oldCamera = renderer->GetCamera();

	renderer->Enable( m_renderTarget );
	renderer->SetCamera( m_Camera );

	renderer->Draw( m_renderData.auxQuad );

    renderer->SetCamera( oldCamera );
	renderer->Disable( m_renderTarget );
}

// **************************
//
Texture2DConstPtr   EffectRenderLogic::ReadTarget         ( Renderer * renderer )
{
    renderer->ReadColorTexture( 0, m_renderTarget, m_readbackTexture );

    return m_readbackTexture;
}

// **************************
//
RenderTargetData    EffectRenderLogic::CreateRenderTargetData () const
{
    RenderTargetData ret;

    std::vector< bv::Transform > vec;
    vec.push_back( Transform( glm::mat4( 1.0f ), glm::mat4( 1.0f ) ) );

    auto rt   = MainDisplayTarget::CreateDisplayRenderTarget( m_textureData.m_width, m_textureData.m_height, m_textureData.m_fmt );
    auto quad = MainDisplayTarget::CreateDisplayRect( rt->ColorTexture( 0 ) );
    
    quad->SetWorldTransforms( vec );

    ret.renderTarget            = rt;
    ret.quad                    = quad;

    return ret;
}

} //bv
