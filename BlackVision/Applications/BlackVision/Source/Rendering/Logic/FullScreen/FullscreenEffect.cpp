#include "FullscreenEffect.h"

#include "Engine/Graphics/SceneGraph/Camera.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Engine/Graphics/Renderers/Renderer.h"

#include "Rendering/Utils/FullscreenUtils.h"


namespace bv {

// **************************
//
FullscreenEffect::FullscreenEffect      ()
    : m_fullscreenQuad( nullptr )
    , m_fullscreenCamera( nullptr )
{
    m_fullscreenCamera = FullscreenUtils::CreateDisplayCamera();
}

// **************************
//
FullscreenEffect::~FullscreenEffect     ()
{
    delete m_fullscreenCamera;
    delete m_fullscreenQuad;
}

// **************************
//
void    FullscreenEffect::Render        ( Renderer * renderer )
{
    if( !m_fullscreenQuad )
    {
        m_fullscreenQuad = CreateFullscreenQuad();
    }
    
    ToggleFullscreenCamera( renderer );

    renderer->Draw( m_fullscreenQuad );

    ToggleRegularCamera( renderer );
}

// **************************
//
void    FullscreenEffect::ToggleFullscreenCamera( Renderer * renderer )
{
    m_lastRendererCamera = renderer->GetCamera();
    renderer->SetCamera( m_fullscreenCamera );
}

// **************************
//
void    FullscreenEffect::ToggleRegularCamera   ( Renderer * renderer )
{
    renderer->SetCamera( m_lastRendererCamera );
}

} //bv
