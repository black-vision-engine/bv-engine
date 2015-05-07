#include "EffectRenderLogic.h"

#include "Engine/Graphics/Resources/RenderTarget.h"
#include "Engine/Graphics/Renderers/Renderer.h"

#include "Engine/Graphics/SceneGraph/MainDisplayTarget.h"
#include "Engine/Graphics/SceneGraph/Camera.h"
#include "Engine/Graphics/SceneGraph/TriangleStrip.h"

#include "Engine/Graphics/Effects/Texture2DEffect.h"

namespace bv {

namespace {

const unsigned int GNumRenderTargets = 2;

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
    //effectTexture2D         = std::static_pointer_cast<Texture2DEffect>( auxQuad->GetRenderableEffect() );
    //effectTexture2DWithMask = std::make_shared<Texture2DEffectWithMask>( nullptr, nullptr, true );

    std::vector< bv::Transform > vec;
    vec.push_back( Transform( glm::mat4( 1.0f ), glm::mat4( 1.0f ) ) );

    auxQuad->SetWorldTransforms( vec );
}

//// **************************
////
//void    EffectRenderData::UseTexture2DEffect ( const IValue * val, Texture2DPtr tex )
//{
//    effect->SetAlphaValModel( val );
//    effect->SetTexture( tex );
//
//    auxQuad->SetRenderableEffect( effectTexture2D );
//}

//// **************************
////
//void    EffectRenderData::UseTexture2DEffect ( const IValue * val, Texture2DPtr tex, Texture2DPtr mask )
//{
//    effectTexture2DWithMask->SetAlphaValModel( val );
//    effectTexture2DWithMask->SetTexture( tex );
//    effectTexture2DWithMask->SetMask( mask );
//
//    auxQuad->SetRenderableEffect( effectTexture2DWithMask );
//}


// *********************************************** OFFSCREEN RENDER LOGIC *********************************************************

// **************************
//
EffectRenderLogic::EffectRenderLogic   ( unsigned int width, unsigned int height, unsigned int numReadBuffers, const RenderableEffectPtr & effect, Camera * camera, TextureFormat fmt )
    : m_textureData( width, height, fmt )
    , m_usedStackedRenderTargets( 0 )
    , m_topRenderTargetEnabled( false )
    , m_readbackTextures( numReadBuffers * GNumRenderTargets ) //two display targets that can be potentially used
    , m_displayCamera( nullptr )
    , m_rendererCamera( camera )
    , m_curDisplayTarget( 0 )
    , m_buffersPerTarget( numReadBuffers )
    , m_displayRTEnabled( false )
	, m_renderData( effect )
{
    m_displayCamera         = MainDisplayTarget::CreateDisplayCamera();

    m_displayRenderTargetData[ 0 ] = CreateDisplayRenderTargetData();
    m_displayRenderTargetData[ 1 ] = CreateDisplayRenderTargetData();

    for( unsigned int i = 0; i < m_readbackTextures.size(); ++i )
    {
        m_readbackTextures[ i ] = nullptr;
    }
}

// **************************
//
EffectRenderLogic::~EffectRenderLogic  ()
{
    for( auto rtd : m_auxRenderTargets )
    {
        delete rtd;
    }

    for( unsigned int i = 0; i < 2; ++i )
    {
        delete m_displayRenderTargetData[ i ].renderTarget;
        delete m_displayRenderTargetData[ i ].quad;
    }

    delete m_displayCamera;
}

// **************************
//
void                EffectRenderLogic::SetRendererCamera         ( Camera * camera )
{
    m_rendererCamera = camera;
}

// **************************
//
void                EffectRenderLogic::AllocateNewRenderTarget     ( Renderer * renderer )
{
    if( m_topRenderTargetEnabled )
    {
        DisableTopRenderTarget( renderer );
    }
    
    m_usedStackedRenderTargets++;

    auto auxRenderTargets = m_usedStackedRenderTargets - 1;

    if( auxRenderTargets > m_auxRenderTargets.size() )
    {
        m_auxRenderTargets.push_back( MainDisplayTarget::CreateAuxRenderTarget( m_textureData.m_width, m_textureData.m_height, m_textureData.m_fmt ) );
    }

    assert( auxRenderTargets <= m_auxRenderTargets.size() );
}

// **************************
//
void                EffectRenderLogic::EnableTopRenderTarget       ( Renderer * renderer )
{
    assert( m_usedStackedRenderTargets > 0 );

    if( !m_topRenderTargetEnabled )
    {
        renderer->Enable( GetRenderTargetAt( -1 ) );

        m_topRenderTargetEnabled = true;
    }
}

// **************************
//
void                EffectRenderLogic::DiscardCurrentRenderTarget  ( Renderer * renderer )
{
    assert( m_usedStackedRenderTargets > 0 );

    if( m_topRenderTargetEnabled )
    {
        DisableTopRenderTarget( renderer );
    }

    m_usedStackedRenderTargets--;
}

// **************************
//
void                EffectRenderLogic::DisableTopRenderTarget    ( Renderer * renderer )
{
    if( m_topRenderTargetEnabled )
    {
        renderer->Disable( GetRenderTargetAt( -1 ) );

        m_topRenderTargetEnabled = false;
    }
}

//// **************************
////
//void                EffectRenderLogic::DrawTopAuxRenderTarget    ( Renderer * renderer, const IValue * alphaVal )
//{
//    DisableTopRenderTarget( renderer );
//
//    auto topRTD = GetRenderTargetAt( -1 );
//    auto prvRTD = GetRenderTargetAt( -2 );
//
//    //m_renderData.UseTexture2DEffect ( alphaVal, topRTD->ColorTexture( 0 ) );
//
//    renderer->Enable( prvRTD );
//    renderer->SetCamera( m_displayCamera );
//    renderer->Draw( m_renderData.auxQuad );
//    renderer->SetCamera( m_rendererCamera );
//    renderer->Disable( prvRTD );
//}
//
//// **************************
////
//void                EffectRenderLogic::DrawAMTopTwoRenderTargets ( Renderer * renderer, const IValue * alphaVal )
//{
//    DisableTopRenderTarget( renderer );
//
//    auto maskRT     = GetRenderTargetAt( -1 );
//    auto textureRT  = GetRenderTargetAt( -2 );
//    auto mainRT     = GetRenderTargetAt( -3 );
//
//    //m_renderData.UseTexture2DEffect( alphaVal, textureRT->ColorTexture( 0 ), maskRT->ColorTexture( 0 ) );
//
//    renderer->Enable( mainRT );
//    renderer->SetCamera( m_displayCamera );
//    renderer->Draw( m_renderData.auxQuad );
//    renderer->SetCamera( m_rendererCamera );
//    renderer->Disable( mainRT );
//}

// **************************
//
void                EffectRenderLogic::DrawDisplayRenderTarget   ( Renderer * renderer )
{
    assert( m_displayRTEnabled == false );

    renderer->SetCamera( m_displayCamera );
	renderer->Draw( m_renderData.auxQuad );
    renderer->SetCamera( m_rendererCamera );
}

// **************************
//
void                EffectRenderLogic::SwapDisplayRenderTargets  ()
{
    m_curDisplayTarget = ( m_curDisplayTarget + 1 ) % GNumRenderTargets;
}

// **************************
//
unsigned int    EffectRenderLogic::TotalNumReadBuffers           () const
{
    return (unsigned int) m_readbackTextures.size();
}

// **************************
//
unsigned int    EffectRenderLogic::NumReadBuffersPerRT           () const
{
    return TotalNumReadBuffers() / GNumRenderTargets;
}

// **************************
//
Texture2DConstPtr   EffectRenderLogic::ReadDisplayTarget         ( Renderer * renderer, unsigned int bufNum )
{
    unsigned int bufferIdx = GNumRenderTargets * bufNum + CurDisplayRenderTargetNum();

    assert( bufferIdx < m_readbackTextures.size() );

    renderer->ReadColorTexture( 0, CurDisplayRenderTargetData().renderTarget, m_readbackTextures[ bufferIdx ] );

    return m_readbackTextures[ bufferIdx ];
}

// **************************
// Python-like logic, where negative numbers are used to index the array backwards
RenderTarget *      EffectRenderLogic::GetRenderTargetAt         ( int i ) const
{
    int numUsedRT = (int) m_usedStackedRenderTargets;

    if( i < 0 )
    {
        i = numUsedRT + i;
    }

    if( i < 0 || i >= numUsedRT )
    {
        assert( false );

        return nullptr;
    }

    if( i == 0 )
    {
        return CurDisplayRenderTargetData().renderTarget;
    }
    else
    {
        return m_auxRenderTargets[ i - 1 ]; 
    }
}

// **************************
//
RenderTargetData    EffectRenderLogic::CreateDisplayRenderTargetData () const
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

// **************************
//
unsigned int      EffectRenderLogic::CurDisplayRenderTargetNum   () const
{
    return m_curDisplayTarget;
}

// **************************
//
RenderTargetData  EffectRenderLogic::CurDisplayRenderTargetData  () const
{
    return m_displayRenderTargetData[ m_curDisplayTarget ];
}

} //bv
