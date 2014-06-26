#include "RenderTargetLogic.h"

#include "Engine/Graphics/Resources/RenderTarget.h"
#include "Engine/Graphics/Renderers/Renderer.h"

#include "Engine/Graphics/SceneGraph/MainDisplayTarget.h"
#include "Engine/Graphics/SceneGraph/Camera.h"


namespace bv {

// **************************
//
RenderTargetLogic::RenderTargetLogic   ( unsigned int width, unsigned int height, TextureFormat fmt )
    : m_displayRenderTarget( nullptr )
    , m_auxRenderTarget( nullptr )
    , m_readbackTexture( nullptr )
{
    m_displayRenderTarget   = MainDisplayTarget::CreateDisplayRenderTarget( width, height, fmt );
    m_auxRenderTarget       = MainDisplayTarget::CreateAuxRenderTarget( width, height, fmt );

    m_displayCamera         = MainDisplayTarget::CreateDisplayCamera();
}

// **************************
//
RenderTargetLogic::~RenderTargetLogic  ()
{
    delete m_displayRenderTarget;
    delete m_auxRenderTarget;

    delete m_displayCamera;

    delete m_readbackTexture;
}

// **************************
//
const Texture2D *   RenderTargetLogic::ReadDisplayTarget   ( Renderer * renderer )
{
    renderer->ReadColorTexture( 0, m_displayRenderTarget, m_readbackTexture );

    return m_readbackTexture;
}

} //bv
