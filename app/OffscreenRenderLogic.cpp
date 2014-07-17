#include "OffscreenRenderLogic.h"

#include "Engine/Graphics/Resources/RenderTarget.h"
#include "Engine/Graphics/Renderers/Renderer.h"

#include "Engine/Graphics/SceneGraph/MainDisplayTarget.h"
#include "Engine/Graphics/SceneGraph/Camera.h"
#include "Engine/Graphics/SceneGraph/TriangleStrip.h"

#include "Engine/Graphics/Effects/Texture2DEffect.h"


namespace bv {

namespace {

const unsigned int GNumRednerTargets = 2;

} //anonymous

// **************************
//
OffscreenRenderLogic::OffscreenRenderLogic   ( unsigned int width, unsigned int height, unsigned int numReadBuffers, Camera * camera, TextureFormat fmt )
    : m_auxRenderTarget( nullptr )
    , m_readbackTextures( numReadBuffers * GNumRednerTargets ) //two display targets that can be potentially used
    , m_displayCamera( nullptr )
    , m_rendererCamera( camera )
    , m_auxQuad( nullptr )
    , m_curDisplayTarget( 0 )
    , m_buffersPerTarget( numReadBuffers )
    , m_auxTexture2DEffect( nullptr )
    , m_displayRTEnabled( false )
    , m_auxRTEnabled( false )
{
    m_displayRenderTargets[ 0 ] = MainDisplayTarget::CreateDisplayRenderTarget( width, height, fmt );
    m_displayRenderTargets[ 1 ] = MainDisplayTarget::CreateDisplayRenderTarget( width, height, fmt );
    m_auxRenderTarget           = MainDisplayTarget::CreateAuxRenderTarget( width, height, fmt );

    m_displayQuads[ 0 ]         = MainDisplayTarget::CreateDisplayRect( m_displayRenderTargets[ 0 ]->ColorTexture( 0 ) );
    m_displayQuads[ 1 ]         = MainDisplayTarget::CreateDisplayRect( m_displayRenderTargets[ 1 ]->ColorTexture( 0 ) );
    m_auxQuad                   = MainDisplayTarget::CreateAuxRect( m_auxRenderTarget->ColorTexture( 0 ) );

    m_auxTexture2DEffect        = static_cast< Texture2DEffect * >( m_auxQuad->GetRenderableEffect() );

    std::vector< bv::Transform > vec;
    vec.push_back( Transform( glm::mat4( 1.0f ), glm::mat4( 1.0f ) ) );

    m_displayQuads[ 0 ]->SetWorldTransforms( vec );
    m_displayQuads[ 1 ]->SetWorldTransforms( vec );
    m_auxQuad->SetWorldTransforms( vec );

    m_displayCamera         = MainDisplayTarget::CreateDisplayCamera();

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
    delete m_auxRenderTarget;

    delete m_displayQuads[ 0 ];
    delete m_displayQuads[ 1 ];
    delete m_auxQuad;

    delete m_displayCamera;
}

// **************************
//
void                OffscreenRenderLogic::SetRendererCamera         ( Camera * camera )
{
    m_rendererCamera = camera;
}

// **************************
//
void                OffscreenRenderLogic::EnableDisplayRenderTarget ( Renderer * renderer )
{
    if( !m_displayRTEnabled )
    {
        if( m_auxRTEnabled )
        {
            DisableAuxRenderTarget( renderer );
        }

        renderer->Enable( CurDisplayRenderTarget() );

        m_displayRTEnabled = true;
    }
}

// **************************
//
void                OffscreenRenderLogic::EnableAuxRenderTarget     ( Renderer * renderer )
{
    if( !m_auxRTEnabled )
    {
        if( m_displayRTEnabled )
        {
            DisableDisplayRenderTarget( renderer );
        }

        renderer->Enable( m_auxRenderTarget );

        m_auxRTEnabled = true;
    }
}

// **************************
//
void                OffscreenRenderLogic::DisableDisplayRenderTarget( Renderer * renderer )
{
    m_displayRTEnabled = false;

    renderer->Disable( CurDisplayRenderTarget() );
}

// **************************
//
void                OffscreenRenderLogic::DisableAuxRenderTarget    ( Renderer * renderer )
{
    m_auxRTEnabled = false;

    renderer->Disable( m_auxRenderTarget );
}

// **************************
//
void                OffscreenRenderLogic::SetAuxAlphaModelValue     ( const IValue * val )
{
    m_auxTexture2DEffect->SetAlphaValModel( val );
}

// **************************
//
void                OffscreenRenderLogic::SwapDisplayRenderTargets  ()
{
    m_curDisplayTarget = ( m_curDisplayTarget + 1 ) % GNumRednerTargets;
}

// **************************
//
bool                OffscreenRenderLogic::DisplayRenderTargetEnabled() const
{
    return m_displayRTEnabled;
}

// **************************
//
bool                OffscreenRenderLogic::AuxRenderTargetEnabled    () const
{
    return m_auxRTEnabled;
}

// **************************
//
void                OffscreenRenderLogic::DrawDisplayRenderTarget   ( Renderer * renderer )
{
    assert( m_auxRTEnabled == false );
    assert( m_displayRTEnabled == false );

    renderer->SetCamera( m_displayCamera );
    renderer->Draw( CurDisplayQuad() );
    renderer->SetCamera( m_rendererCamera );
}

// **************************
//
void                OffscreenRenderLogic::DrawAuxRenderTarget       ( Renderer * renderer )
{
    assert( m_auxRTEnabled == false );

    renderer->SetCamera( m_displayCamera );
    renderer->Draw( m_auxQuad );
    renderer->SetCamera( m_rendererCamera );
}

// **************************
//
unsigned int        OffscreenRenderLogic::TotalNumReadBuffers       () const
{
    return m_readbackTextures.size();
}

// **************************
//
unsigned int        OffscreenRenderLogic::NumReadBuffersPerRT       () const
{
    return TotalNumReadBuffers() / GNumRednerTargets;
}

// **************************
//
Texture2DConstPtr   OffscreenRenderLogic::ReadDisplayTarget         ( Renderer * renderer, unsigned int bufNum )
{
    unsigned int bufferIdx = GNumRednerTargets * bufNum + CurDisplayRenderTargetNum();

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
