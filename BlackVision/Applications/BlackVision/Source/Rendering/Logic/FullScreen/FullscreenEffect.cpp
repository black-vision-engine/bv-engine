#include "FullscreenEffect.h"

#include "Engine/Graphics/SceneGraph/Camera.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParamFactory.h"

#include "Engine/Graphics/Renderers/Renderer.h"

#include "Rendering/Utils/FullscreenUtils.h"

#include "Rendering/Logic/FullScreen/FullscreenVSShader.h"


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
        m_fullscreenQuad = CreateFullscreenQuad( renderer );
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

// **************************
//
VertexShader *  FullscreenEffect::CreateVS      ( unsigned int numUVChannels ) const
{
    auto params     = new ShaderParameters();
    auto mvpParam   = ShaderParamFactory::CreateMVPParameter();

    params->AddParameter( mvpParam );

    auto shader = new VertexShader( GetVSShaderSource( numUVChannels ), params );

    return shader;
}

// **************************
//
std::string     FullscreenEffect::GetVSShaderSource ( unsigned int numUVChannels ) const
{
    return FullscreenVSShader::GenerateDefaultVS( numUVChannels );
}

} //bv
