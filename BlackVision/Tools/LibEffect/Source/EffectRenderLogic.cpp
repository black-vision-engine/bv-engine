#include "EffectRenderLogic.h"

#include "Engine/Graphics/Resources/RenderTarget.h"
#include "Engine/Graphics/Renderers/Renderer.h"

#include "Engine/Graphics/SceneGraph/MainDisplayTarget.h"
#include "Engine/Graphics/SceneGraph/Camera.h"
#include "Engine/Graphics/SceneGraph/TriangleStrip.h"

#include "Engine/Graphics/Effects/Texture2DEffect.h"

namespace bv {

namespace {

const unsigned int GNumRenderTargets = 1;

} //anonymous


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


// *********************************************** OFFSCREEN RENDER DATA **********************************************************

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

//// **************************
////
//void                EffectRenderLogic::AllocateNewRenderTarget     ( Renderer * renderer )
//{
//    if( m_topRenderTargetEnabled )
//    {
//        DisableTopRenderTarget( renderer );
//    }
//    
//    m_usedStackedRenderTargets++;
//
//    auto auxRenderTargets = m_usedStackedRenderTargets - 1;
//
//    if( auxRenderTargets > m_auxRenderTargets.size() )
//    {
//        m_auxRenderTargets.push_back( MainDisplayTarget::CreateAuxRenderTarget( m_textureData.m_width, m_textureData.m_height, m_textureData.m_fmt ) );
//    }
//
//    assert( auxRenderTargets <= m_auxRenderTargets.size() );
//}
//
//// **************************
////
//void                EffectRenderLogic::EnableTopRenderTarget       ( Renderer * renderer )
//{
//    assert( m_usedStackedRenderTargets > 0 );
//
//    if( !m_topRenderTargetEnabled )
//    {
//        renderer->Enable( GetRenderTargetAt( -1 ) );
//
//        m_topRenderTargetEnabled = true;
//    }
//}
//
//// **************************
////
//void                EffectRenderLogic::DiscardCurrentRenderTarget  ( Renderer * renderer )
//{
//    assert( m_usedStackedRenderTargets > 0 );
//
//    if( m_topRenderTargetEnabled )
//    {
//        DisableTopRenderTarget( renderer );
//    }
//
//    m_usedStackedRenderTargets--;
//}
//
//// **************************
////
//void                EffectRenderLogic::DisableTopRenderTarget    ( Renderer * renderer )
//{
//    if( m_topRenderTargetEnabled )
//    {
//        renderer->Disable( GetRenderTargetAt( -1 ) );
//
//        m_topRenderTargetEnabled = false;
//    }
//}

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

//// **************************
////
//void                EffectRenderLogic::SwapDisplayRenderTargets  ()
//{
//    m_curDisplayTarget = ( m_curDisplayTarget + 1 ) % GNumRenderTargets;
//}
//
//// **************************
////
//unsigned int    EffectRenderLogic::TotalNumReadBuffers           () const
//{
//    return (unsigned int) m_readbackTextures.size();
//}
//
//// **************************
////
//unsigned int    EffectRenderLogic::NumReadBuffersPerRT           () const
//{
//    return TotalNumReadBuffers() / GNumRenderTargets;
//}

// **************************
//
Texture2DConstPtr   EffectRenderLogic::ReadTarget         ( Renderer * renderer )
{
    renderer->ReadColorTexture( 0, m_renderTarget, m_readbackTexture );

    return m_readbackTexture;
}

//// **************************
//// Python-like logic, where negative numbers are used to index the array backwards
//RenderTarget *      EffectRenderLogic::GetRenderTargetAt         ( int i ) const
//{
//    int numUsedRT = (int) m_usedStackedRenderTargets;
//
//    if( i < 0 )
//    {
//        i = numUsedRT + i;
//    }
//
//    if( i < 0 || i >= numUsedRT )
//    {
//        assert( false );
//
//        return nullptr;
//    }
//
//    if( i == 0 )
//    {
//        return CurDisplayRenderTargetData().renderTarget;
//    }
//    else
//    {
//        return m_auxRenderTargets[ i - 1 ]; 
//    }
//}

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
