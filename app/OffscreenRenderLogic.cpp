#include "OffscreenRenderLogic.h"

#include "Engine/Graphics/Resources/RenderTarget.h"
#include "Engine/Graphics/Renderers/Renderer.h"

#include "Engine/Graphics/SceneGraph/MainDisplayTarget.h"
#include "Engine/Graphics/SceneGraph/Camera.h"
#include "Engine/Graphics/SceneGraph/TriangleStrip.h"

#include "Engine/Graphics/Effects/Texture2DEffect.h"


namespace bv {

// **************************
//
OffscreenRenderLogic::OffscreenRenderLogic   ( unsigned int width, unsigned int height, unsigned int numReadBuffers, Camera * camera, TextureFormat fmt )
    : m_displayRenderTarget( nullptr )
    , m_auxRenderTarget( nullptr )
    , m_readbackTextures( numReadBuffers )
    , m_displayCamera( nullptr )
    , m_rendererCamera( camera )
    , m_displayQuad( nullptr )
    , m_auxQuad( nullptr )
    , m_auxTexture2DEffect( nullptr )
    , m_displayRTEnabled( false )
    , m_auxRTEnabled( false )
{
    m_displayRenderTarget   = MainDisplayTarget::CreateDisplayRenderTarget( width, height, fmt );
    m_auxRenderTarget       = MainDisplayTarget::CreateAuxRenderTarget( width, height, fmt );

    m_displayQuad           = MainDisplayTarget::CreateDisplayRect( m_displayRenderTarget->ColorTexture( 0 ) );
    m_auxQuad               = MainDisplayTarget::CreateAuxRect( m_auxRenderTarget->ColorTexture( 0 ) );

    m_auxTexture2DEffect    = static_cast< Texture2DEffect * >( m_auxQuad->GetRenderableEffect() );

    std::vector< bv::Transform > vec;
    vec.push_back( Transform( glm::mat4( 1.0f ), glm::mat4( 1.0f ) ) );

    m_displayQuad->SetWorldTransforms( vec );
    m_auxQuad->SetWorldTransforms( vec );
  
    m_displayCamera         = MainDisplayTarget::CreateDisplayCamera();

    for( unsigned int i = 0; i < numReadBuffers; ++i )
    {
        m_readbackTextures[ i ] = nullptr;
    }
}

// **************************
//
OffscreenRenderLogic::~OffscreenRenderLogic  ()
{
    delete m_displayRenderTarget;
    delete m_auxRenderTarget;

    delete m_displayQuad;
    delete m_auxQuad;

    delete m_displayCamera;

    for( auto tx : m_readbackTextures )
    {
        delete tx;
    }
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

        renderer->Enable( m_displayRenderTarget );

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

    renderer->Disable( m_displayRenderTarget );
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
    renderer->Draw( m_displayQuad );
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
unsigned int        OffscreenRenderLogic::NumReadBuffers            () const
{
    return m_readbackTextures.size();
}

// **************************
//
const Texture2D *   OffscreenRenderLogic::ReadDisplayTarget         ( Renderer * renderer, unsigned int bufNum )
{
    assert( bufNum < m_readbackTextures.size() );

    renderer->ReadColorTexture( 0, m_displayRenderTarget, m_readbackTextures[ bufNum ] );

    return m_readbackTextures[ bufNum ];
}

} //bv
