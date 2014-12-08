#include "OffscreenRenderLogic.h"

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

// **************************
//
OffscreenRenderLogic::OffscreenRenderLogic   ( unsigned int width, unsigned int height, unsigned int numReadBuffers, Camera * camera, TextureFormat fmt )
    : m_width( width )
    , m_height( height )
    , m_fmt( fmt )
    , m_usedStackedRenderTargets( 0 )
    , m_topRenderTargetEnabled( false )
    , m_readbackTextures( numReadBuffers * GNumRenderTargets ) //two display targets that can be potentially used
    , m_displayCamera( nullptr )
    , m_rendererCamera( camera )
    , m_curDisplayTarget( 0 )
    , m_buffersPerTarget( numReadBuffers )
    , m_displayRTEnabled( false )
{
    m_displayCamera         = MainDisplayTarget::CreateDisplayCamera();

    m_displayRenderTargets[ 0 ] = MainDisplayTarget::CreateDisplayRenderTarget( width, height, fmt );
    m_displayRenderTargets[ 1 ] = MainDisplayTarget::CreateDisplayRenderTarget( width, height, fmt );

    m_displayQuads[ 0 ]         = MainDisplayTarget::CreateDisplayRect( m_displayRenderTargets[ 0 ]->ColorTexture( 0 ) );
    m_displayQuads[ 1 ]         = MainDisplayTarget::CreateDisplayRect( m_displayRenderTargets[ 1 ]->ColorTexture( 0 ) );

    std::vector< bv::Transform > vec;
    vec.push_back( Transform( glm::mat4( 1.0f ), glm::mat4( 1.0f ) ) );

    m_displayQuads[ 0 ]->SetWorldTransforms( vec );
    m_displayQuads[ 1 ]->SetWorldTransforms( vec );

    for( unsigned int i = 0; i < m_readbackTextures.size(); ++i )
    {
        m_readbackTextures[ i ] = nullptr;
    }
}

// **************************
//
OffscreenRenderLogic::~OffscreenRenderLogic  ()
{
    delete m_displayRenderTargets[ 0 ];
    delete m_displayRenderTargets[ 1 ];

    for( auto rt : m_auxRenderTargetVec )
    {
        delete rt;
    }

    for( auto aq : m_auxQuadVec )
    {
        delete aq;
    }

    delete m_displayQuads[ 0 ];
    delete m_displayQuads[ 1 ];

    delete m_displayCamera;
}

// **************************
//
void                OffscreenRenderLogic::AllocateNewRenderTarget     ( Renderer * renderer )
{
    if( m_topRenderTargetEnabled )
    {
        DisableTopRenderTarget( renderer );
    }
    
    m_usedStackedRenderTargets++;

    auto auxRenderTargets = m_usedStackedRenderTargets - 1;

    if( auxRenderTargets > m_auxRenderTargetVec.size() )
    {
        auto rt = CreateAuxRenderTarget( renderer );
        auto aq = CreateAuxQuad( rt );

        m_auxRenderTargetVec.push_back( rt );
        m_auxQuadVec.push_back( aq );
    }

    assert( auxRenderTargets <= m_auxRenderTargetVec.size() );
}

// **************************
//
void                OffscreenRenderLogic::EnableTopRenderTarget       ( Renderer * renderer )
{
    assert( m_usedStackedRenderTargets > 0 );

    if( !m_topRenderTargetEnabled )
    {
        renderer->Enable( GetTopRenderTarget() );

        m_topRenderTargetEnabled = true;
    }
}

// **************************
//
void                OffscreenRenderLogic::DiscardCurrentRenderTarget  ( Renderer * renderer )
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
void                OffscreenRenderLogic::DisableTopRenderTarget    ( Renderer * renderer )
{
    if( m_topRenderTargetEnabled )
    {
        renderer->Disable( GetTopRenderTarget() );

        m_topRenderTargetEnabled = false;
    }
}

// **************************
//
void                OffscreenRenderLogic::DrawTopAuxRenderTarget    ( Renderer * renderer, const IValue * alphaVal )
{
    DisableTopRenderTarget( renderer );

    auto rt = GetPrevRenderTarget();
    auto rq = GetTopRenderQuad();

    auto ef = static_cast< Texture2DEffect * >( rq->GetRenderableEffect() );
    ef->SetAlphaValModel( alphaVal );

    renderer->Enable( rt );
    renderer->SetCamera( m_displayCamera );
    renderer->Draw( rq );
    renderer->SetCamera( m_rendererCamera );
    renderer->Disable( rt );
}

// **************************
//
RenderTarget *      OffscreenRenderLogic::GetTopRenderTarget        () const
{
    if( m_usedStackedRenderTargets == 1 )
    {
        return CurDisplayRenderTarget();
    }
    else if( m_usedStackedRenderTargets > 1 )
    {
        return m_auxRenderTargetVec[ m_usedStackedRenderTargets - 2 ]; 
    }

    return nullptr;
}

// **************************
//
RenderTarget *      OffscreenRenderLogic::GetPrevRenderTarget       () const
{
    assert( m_usedStackedRenderTargets > 1 );

    if( m_usedStackedRenderTargets == 2 )
    {
        return CurDisplayRenderTarget();
    }
    else if( m_usedStackedRenderTargets > 2 )
    {
        return m_auxRenderTargetVec[ m_usedStackedRenderTargets - 3 ]; 
    }

    return nullptr;   
}

// **************************
//
TriangleStrip *      OffscreenRenderLogic::GetTopRenderQuad         () const
{
    assert( m_usedStackedRenderTargets > 0 );

    if( m_usedStackedRenderTargets == 1 )
    {
        return CurDisplayQuad();
    }
    else if( m_usedStackedRenderTargets > 1 )
    {
        return m_auxQuadVec[ m_usedStackedRenderTargets - 2 ]; 
    }

    return nullptr;   
}

// **************************
//
RenderTarget *      OffscreenRenderLogic::CreateAuxRenderTarget     ( Renderer * renderer )
{
    return MainDisplayTarget::CreateAuxRenderTarget( m_width, m_height, m_fmt );
}

// **************************
//
TriangleStrip *     OffscreenRenderLogic::CreateAuxQuad             ( RenderTarget * rt )
{
    auto retQuad = MainDisplayTarget::CreateAuxRect( rt->ColorTexture( 0 ) );

    std::vector< bv::Transform > vec;
    vec.push_back( Transform( glm::mat4( 1.0f ), glm::mat4( 1.0f ) ) );

    retQuad->SetWorldTransforms( vec );

    return retQuad;
}

// **************************
//
void                OffscreenRenderLogic::SetRendererCamera         ( Camera * camera )
{
    m_rendererCamera = camera;
}

// **************************
//
void                OffscreenRenderLogic::SwapDisplayRenderTargets  ()
{
    m_curDisplayTarget = ( m_curDisplayTarget + 1 ) % GNumRenderTargets;
}

// **************************
//
void                OffscreenRenderLogic::DrawDisplayRenderTarget   ( Renderer * renderer )
{
    assert( m_displayRTEnabled == false );

    renderer->SetCamera( m_displayCamera );
    renderer->Draw( CurDisplayQuad() );
    renderer->SetCamera( m_rendererCamera );
}

// **************************
//
unsigned int        OffscreenRenderLogic::TotalNumReadBuffers       () const
{
    return (unsigned int) m_readbackTextures.size();
}

// **************************
//
unsigned int        OffscreenRenderLogic::NumReadBuffersPerRT       () const
{
    return TotalNumReadBuffers() / GNumRenderTargets;
}

// **************************
//
Texture2DConstPtr   OffscreenRenderLogic::ReadDisplayTarget         ( Renderer * renderer, unsigned int bufNum )
{
    unsigned int bufferIdx = GNumRenderTargets * bufNum + CurDisplayRenderTargetNum();

    assert( bufferIdx < m_readbackTextures.size() );

    renderer->ReadColorTexture( 0, CurDisplayRenderTarget(), m_readbackTextures[ bufferIdx ] );

    return m_readbackTextures[ bufferIdx ];
}

// **************************
//
unsigned int      OffscreenRenderLogic::CurDisplayRenderTargetNum   () const
{
    return m_curDisplayTarget;
}

// **************************
//
RenderTarget *    OffscreenRenderLogic::CurDisplayRenderTarget      () const
{
    return m_displayRenderTargets[ m_curDisplayTarget ];
}

// **************************
//
TriangleStrip *   OffscreenRenderLogic:: CurDisplayQuad              () const
{
    return m_displayQuads[ m_curDisplayTarget ];
}

} //bv
