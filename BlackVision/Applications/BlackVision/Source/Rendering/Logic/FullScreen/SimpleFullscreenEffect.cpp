#include "SimpleFullscreenEffect.h"

#include "Engine/Graphics/SceneGraph/Camera.h"
#include "Engine/Graphics/SceneGraph/TriangleStrip.h"

#include "Engine/Graphics/Renderers/Renderer.h"

#include "Rendering/Utils/FullscreenUtils.h"


namespace  bv {

// **************************
//
SimpleFullscreenEffect::SimpleFullscreenEffect  ( const FullscreenEffectData & inputData )
    : m_data( inputData )
    , m_fullscreenCamera( nullptr )
    , m_fullscreenQuad( nullptr )
{
    m_fullscreenCamera = FullscreenUtils::CreateDisplayCamera();
}

// **************************
//
SimpleFullscreenEffect::~SimpleFullscreenEffect ()
{
    delete m_fullscreenCamera;
}

// **************************
//
void    SimpleFullscreenEffect::Render          ( FullscreenEffectContext * ctx )
{
    //if( !m_fullscreenQuad )
    //{
    //    m_fullscreenQuad = CreateFullscreenQuad();
    //}
    auto renderer = ctx->GetRenderer();

    auto rendererCamera = renderer->GetCamera();
    renderer->SetCamera( m_fullscreenCamera );

    renderer->Draw( m_fullscreenQuad );

    renderer->SetCamera( rendererCamera );
}

} //bv
