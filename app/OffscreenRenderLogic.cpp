#include "OffscreenRenderLogic.h"

#include "Engine/Graphics/Resources/RenderTarget.h"
#include "Engine/Graphics/Renderers/Renderer.h"

#include "Engine/Graphics/SceneGraph/MainDisplayTarget.h"
#include "Engine/Graphics/SceneGraph/Camera.h"


namespace bv {

// **************************
//
OffscreenRenderLogic::OffscreenRenderLogic   ( unsigned int width, unsigned int height, TextureFormat fmt )
    : m_displayRenderTarget( nullptr )
    , m_auxRenderTarget( nullptr )
    , m_readbackTexture( nullptr )
    , m_displayQuad( nullptr )
    , m_auxQuad( nullptr )
    , m_displayRTEnabled( false )
    , m_auxRTEnabled( false )
{
    m_displayRenderTarget   = MainDisplayTarget::CreateDisplayRenderTarget( width, height, fmt );
    m_auxRenderTarget       = MainDisplayTarget::CreateAuxRenderTarget( width, height, fmt );

    m_displayQuad           = MainDisplayTarget::CreateDisplayRect( m_displayRenderTarget->ColorTexture( 0 ) );
    m_auxQuad               = MainDisplayTarget::CreateDisplayRect( m_auxRenderTarget->ColorTexture( 0 ) );

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
void                OffscreenRenderLogic::EnableDisplayRenderTarget   ( Renderer * renderer )
{
    if( !m_displayRTEnabled )
    {
        if( m_auxRTEnabled )
        {
            DisableAuxRenderTarget( renderer );
        }

        renderer->Enable( m_displayRenderTarget );
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
const Texture2D *   OffscreenRenderLogic::ReadDisplayTarget   ( Renderer * renderer )
{
    renderer->ReadColorTexture( 0, m_displayRenderTarget, m_readbackTexture );

    return m_readbackTexture;
}

} //bv
