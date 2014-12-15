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
RenderTargetData::RenderTargetData      ()
    : renderTarget( nullptr )
    , quad( nullptr )
    , effectTexture2D( nullptr )
    , effectTexture2DWithMask( nullptr )
{
}

// **************************
//
RenderTargetData::~RenderTargetData     ()
{
}

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

    m_displayRenderTargetData[ 0 ] = CreateDisplayRenderTargetData();
    m_displayRenderTargetData[ 1 ] = CreateDisplayRenderTargetData();

    for( unsigned int i = 0; i < m_readbackTextures.size(); ++i )
    {
        m_readbackTextures[ i ] = nullptr;
    }
}

// **************************
//
OffscreenRenderLogic::~OffscreenRenderLogic  ()
{
    for( auto rt : m_auxRenderTargetVec )
    {
        delete rt;
    }

    for( auto aq : m_auxQuadVec )
    {
        delete aq;
    }

    for( auto rtd : m_auxRenderTargetsData )
    {
        delete rtd.renderTarget;
        delete rtd.quad;
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
void                OffscreenRenderLogic::AllocateNewRenderTarget     ( Renderer * renderer )
{
    if( m_topRenderTargetEnabled )
    {
        DisableTopRenderTarget( renderer );
    }
    
    m_usedStackedRenderTargets++;

    auto auxRenderTargets = m_usedStackedRenderTargets - 1;

    if( auxRenderTargets > m_auxRenderTargetsData.size() )
    {
        auto rtd = CreateAuxRenderTargetData();
        auto rt = CreateAuxRenderTarget();
        auto aq = CreateAuxQuad( rt );

        m_auxRenderTargetVec.push_back( rt );
        m_auxQuadVec.push_back( aq );

        m_auxRenderTargetsData.push_back( rtd );
    }

    assert( auxRenderTargets <= m_auxRenderTargetsData.size() );
}

// **************************
//
void                OffscreenRenderLogic::EnableTopRenderTarget       ( Renderer * renderer )
{
    assert( m_usedStackedRenderTargets > 0 );

    if( !m_topRenderTargetEnabled )
    {
        //renderer->Enable( GetTopRenderTarget() );
        renderer->Enable( GetTopRenderTargetData().renderTarget );

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
        // renderer->Disable( GetTopRenderTarget() );
        renderer->Disable( GetTopRenderTargetData().renderTarget );

        m_topRenderTargetEnabled = false;
    }
}

// **************************
//
void                OffscreenRenderLogic::DrawTopAuxRenderTarget    ( Renderer * renderer, const IValue * alphaVal )
{
    DisableTopRenderTarget( renderer );

    auto topRTD = GetTopRenderTargetData();
    auto prvRTD = GetRenderTargetDataAt( GetNumAllocatedRenderTargets() - 2 );

    auto eff = topRTD.effectTexture2D;
    eff->SetAlphaValModel( alphaVal );

    renderer->Enable( prvRTD.renderTarget );
    renderer->SetCamera( m_displayCamera );
    renderer->Draw( topRTD.quad );
    renderer->SetCamera( m_rendererCamera );
    renderer->Disable( prvRTD.renderTarget );

    /*
    auto rt = GetPrevRenderTarget();
    auto rq = GetTopRenderQuad();

    auto ef = std::static_pointer_cast< Texture2DEffect >( rq->GetRenderableEffect() );
    ef->SetAlphaValModel( alphaVal );

    renderer->Enable( rt );
    renderer->SetCamera( m_displayCamera );
    renderer->Draw( rq );
    renderer->SetCamera( m_rendererCamera );
    renderer->Disable( rt );
    */
}

// **************************
//FIXME: side effect - removes two topmost render targets
void                OffscreenRenderLogic::DrawAMTopTwoRenderTargets ( Renderer * renderer, const IValue * alphaVal )
{
    DisableTopRenderTarget( renderer );

    auto rtAlpha    = GetTopRenderTarget();
    auto quadData   = GetPrevRenderTarget();
    
    DiscardCurrentRenderTarget( renderer );
    DiscardCurrentRenderTarget( renderer );

    auto rtMain = GetTopRenderTarget();

    //FIXME: either effects should be replacable within Renderable or a few different versions of quads should be accessible here
    /*
    auto ef = static_cast< Texture2DEffect * >( rq->GetRenderableEffect() );
    ef->SetAlphaValModel( alphaVal );

    renderer->Enable( rt );
    renderer->SetCamera( m_displayCamera );
    renderer->Draw( rq );
    renderer->SetCamera( m_rendererCamera );
    renderer->Disable( rt );
    */
}

// **************************
//
RenderTargetData    OffscreenRenderLogic::GetTopRenderTargetData          () const
{
    if( m_usedStackedRenderTargets == 1 )
    {
        return CurDisplayRenderTargetData();
    }
    else if( m_usedStackedRenderTargets > 1 )
    {
        return m_auxRenderTargetsData[ m_usedStackedRenderTargets - 2 ]; 
    }

    assert( false );

    return RenderTargetData();
}

// **************************
//
RenderTargetData    OffscreenRenderLogic::GetRenderTargetDataAt           ( unsigned int i ) const
{
    if( i >= m_usedStackedRenderTargets )
    {
        assert( false );

        return RenderTargetData();
    }

    if( i == 0 )
    {
        return CurDisplayRenderTargetData();
    }
    else
    {
        return m_auxRenderTargetsData[ i - 1 ]; 
    }

    assert( false );

    return RenderTargetData();
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
RenderTargetData    OffscreenRenderLogic::CreateDisplayRenderTargetData () const
{
    auto rt   = MainDisplayTarget::CreateDisplayRenderTarget( m_width, m_height, m_fmt );
    auto quad = MainDisplayTarget::CreateDisplayRect( rt->ColorTexture( 0 ) );

    return CreateRenderTargetData( rt, quad, nullptr, nullptr );
}

// **************************
//
RenderTargetData    OffscreenRenderLogic::CreateAuxRenderTargetData     () const
{
    auto rt     = MainDisplayTarget::CreateAuxRenderTarget( m_width, m_height, m_fmt );
    auto quad   = MainDisplayTarget::CreateAuxRect( rt->ColorTexture( 0 ) );

    auto txEff = std::static_pointer_cast< Texture2DEffect >( quad->GetRenderableEffect() );

    return CreateRenderTargetData( rt, quad, txEff, nullptr );
}

// **************************
//
RenderTargetData    OffscreenRenderLogic::CreateRenderTargetData    ( RenderTarget * rt, TriangleStrip * ts, Texture2DEffectPtr effTx, Texture2DEffectWithMaskPtr effTxMask ) const
{
    SetDefaultTransform( ts );

    RenderTargetData ret;

    ret.renderTarget            = rt;
    ret.quad                    = ts;
    ret.effectTexture2D         = effTx;
    ret.effectTexture2DWithMask = effTxMask;

    return ret;
}

// **************************
//
void                OffscreenRenderLogic::SetDefaultTransform       ( TriangleStrip * ts ) const
{
    std::vector< bv::Transform > vec;
    vec.push_back( Transform( glm::mat4( 1.0f ), glm::mat4( 1.0f ) ) );

    ts->SetWorldTransforms( vec );
}

// **************************
//
RenderTarget *      OffscreenRenderLogic::CreateAuxRenderTarget     () const
{
    return MainDisplayTarget::CreateAuxRenderTarget( m_width, m_height, m_fmt );
}

// **************************
//
TriangleStrip *     OffscreenRenderLogic::CreateAuxQuad             ( RenderTarget * rt ) const
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
unsigned int    OffscreenRenderLogic::GetNumAllocatedRenderTargets  () const
{
    return m_usedStackedRenderTargets;
}

// **************************
//
unsigned int    OffscreenRenderLogic::TotalNumReadBuffers           () const
{
    return (unsigned int) m_readbackTextures.size();
}

// **************************
//
unsigned int    OffscreenRenderLogic::NumReadBuffersPerRT           () const
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
RenderTargetData  OffscreenRenderLogic::CurDisplayRenderTargetData  () const
{
    return m_displayRenderTargetData[ m_curDisplayTarget ];
}

// **************************
//
RenderTarget *    OffscreenRenderLogic::CurDisplayRenderTarget      () const
{
    return m_displayRenderTargetData[ m_curDisplayTarget ].renderTarget;
}

// **************************
//
TriangleStrip *   OffscreenRenderLogic:: CurDisplayQuad             () const
{
    return m_displayRenderTargetData[ m_curDisplayTarget ].quad;
}

} //bv
