#include "OffscreenRenderLogic.h"

#include "Engine/Graphics/Resources/RenderTarget.h"
#include "Engine/Graphics/Renderers/Renderer.h"

#include "Engine/Graphics/SceneGraph/MainDisplayTarget.h"
#include "Engine/Graphics/SceneGraph/Camera.h"

#include "Engine/Graphics/SceneGraph/TriangleStrip.h"


namespace bv {

// **************************
//
OffscreenRenderLogic::OffscreenRenderLogic   ( unsigned int width, unsigned int height, Camera * camera, TextureFormat fmt )
    : m_displayRenderTarget( nullptr )
    , m_auxRenderTarget( nullptr )
    , m_readbackTexture( nullptr )
    , m_displayCamera( nullptr )
    , m_rendererCamera( camera )
    , m_displayQuad( nullptr )
    , m_auxQuad( nullptr )
    , m_displayRTEnabled( false )
    , m_auxRTEnabled( false )
{
    m_displayRenderTarget   = MainDisplayTarget::CreateDisplayRenderTarget( width, height, fmt );
    m_auxRenderTarget       = MainDisplayTarget::CreateAuxRenderTarget( width, height, fmt );

    m_displayQuad           = MainDisplayTarget::CreateDisplayRect( m_displayRenderTarget->ColorTexture( 0 ) );
    m_auxQuad               = MainDisplayTarget::CreateDisplayRect( m_auxRenderTarget->ColorTexture( 0 ) );

    std::vector< bv::Transform > vec;
    vec.push_back( Transform( glm::mat4( 1.0f ), glm::mat4( 1.0f ) ) );

    m_displayQuad->SetWorldTransforms( vec );
    m_auxQuad->SetWorldTransforms( vec );

    m_displayCamera         = MainDisplayTarget::CreateDisplayCamera();
}

// **************************
//
OffscreenRenderLogic::~OffscreenRenderLogic  ()
{
    delete m_displayRenderTarget;
    delete m_auxRenderTarget;

    delete m_displayCamera;

    delete m_readbackTexture;
}

// **************************
//
void                OffscreenRenderLogic::SetRendererCamera           ( Camera * camera )
{
    m_rendererCamera = camera;
}

// **************************
//
void                OffscreenRenderLogic::EnableDisplayRenderTarget   ( Renderer * renderer )
{
    if( !m_displayRTEnabled )
    {
        if( m_auxRTEnabled )
        {
            DisableAuxRenderTarget( renderer );
        }

        renderer->Enable( m_displayRenderTarget );
        renderer->ClearBuffers(); //FIXME: set clear color if necessary

        m_displayRTEnabled = true;
    }
}

// **************************
//
void                OffscreenRenderLogic::EnableAuxRenderTarget       ( Renderer * renderer )
{
    if( !m_auxRTEnabled )
    {
        if( m_displayRTEnabled )
        {
            DisableDisplayRenderTarget( renderer );
        }

        renderer->Enable( m_auxRenderTarget );
        renderer->ClearBuffers(); //FIXME: set clear color if necessary

        m_auxRTEnabled = true;
    }
}

// **************************
//
void                OffscreenRenderLogic::DisableDisplayRenderTarget  ( Renderer * renderer )
{
    m_displayRTEnabled = false;

    renderer->Disable( m_displayRenderTarget );
}

// **************************
//
void                OffscreenRenderLogic::DisableAuxRenderTarget      ( Renderer * renderer )
{
    m_auxRTEnabled = false;

    renderer->Disable( m_auxRenderTarget );
}

// **************************
//
void                OffscreenRenderLogic::DrawDisplayRenderTarget     ( Renderer * renderer )
{
    assert( m_auxRTEnabled == false );
    assert( m_displayRTEnabled == false );

    renderer->SetCamera( m_displayCamera );
    renderer->Draw( m_displayQuad );
    renderer->SetCamera( m_rendererCamera );
}

// **************************
//
const Texture2D *   OffscreenRenderLogic::ReadDisplayTarget   ( Renderer * renderer )
{
    renderer->ReadColorTexture( 0, m_displayRenderTarget, m_readbackTexture );

    return m_readbackTexture;
}

} //bv
