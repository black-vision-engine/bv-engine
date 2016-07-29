#include "FullscreenEffectOld.h"

#include <sstream>

#include "Engine/Graphics/SceneGraph/Camera.h"
#include "Engine/Graphics/SceneGraph/TriangleStrip.h"

#include "Engine/Graphics/Renderers/Renderer.h"

#include "Engine/Models/Plugins/EngineConstantsMapper.h"

#include "Engine/Models/Builder/RendererStatesBuilder.h"

#include "Engine/Graphics/Effects/FullScreen/Old/Impl/FullscreenRenderableEffectOld.h"

#include "Engine/Graphics/Effects/Utils/FullscreenUtils.h"

#include "IO/FileIO.h"




#include "Memory/MemoryLeaks.h"



namespace bv {

// **************************
//
FullscreenEffectOld::FullscreenEffectOld      ()
    : m_fullscreenQuad( nullptr )
    , m_fullscreenCamera( nullptr )
{
    m_fullscreenCamera = FullscreenUtils::CreateDisplayCamera();
}

// **************************
//
FullscreenEffectOld::~FullscreenEffectOld     ()
{
    delete m_fullscreenCamera;
    delete m_fullscreenQuad;
}

// **************************
//
void    FullscreenEffectOld::Render        ( Renderer * renderer )
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
void    FullscreenEffectOld::SetFullscreenQuad         ( RenderableEntity * quad )
{
    assert( !m_fullscreenQuad );

    m_fullscreenQuad = quad;
}

// **************************
//
void    FullscreenEffectOld::ToggleFullscreenCamera    ( Renderer * renderer )
{
    m_lastRendererCamera = renderer->GetCamera();
    renderer->SetCamera( m_fullscreenCamera );
}

// **************************
//
void    FullscreenEffectOld::ToggleRegularCamera   ( Renderer * renderer )
{
    renderer->SetCamera( m_lastRendererCamera );
}

// **************************
//
RenderableEffectPtr FullscreenEffectOld::CreateDefaultEffect   ( PixelShader * ps )
{
    auto vs = FullscreenRenderableEffectOld::CreateVS( 1 );

    RenderablePass * pass = new RenderablePass( ps, vs, nullptr );

    auto sinst = pass->GetStateInstance();
    
    RendererStatesBuilder::Create( sinst );

    auto as = RenderStateAccessor::AccessAlphaState( sinst );
    auto ds = RenderStateAccessor::AccessDepthState( sinst );
    auto cs = RenderStateAccessor::AccessCullState( sinst );

    as->blendEnabled = false;
    ds->enabled = false;
    cs->enabled = false;

    auto effect = std::make_shared< FullscreenRenderableEffectOld >( pass );

    return effect;
}

// **************************
//
RenderableEntity *  FullscreenEffectOld::CreateDefaultFullscrQuad  ( PixelShader * ps )
{
    auto effect = CreateDefaultEffect( ps );

    RenderableEntity * renderable = FullscreenUtils::CreateFullscreenQuad( effect, 1 );

    return renderable;
}

// **************************
//
TextureSampler *    FullscreenEffectOld::CreateDefaultSampler      ( const std::string & samplerName )
{
    auto wrapX          = EngineConstantsMapper::EngineConstant( TextureWrappingMode::TWM_CLAMP );
    auto wrapY          = EngineConstantsMapper::EngineConstant( TextureWrappingMode::TWM_CLAMP );            
    auto samplingMode   = SamplerSamplingMode::SSM_MODE_2D;
    auto sfm            = EngineConstantsMapper::EngineConstant( TextureFilteringMode::TFM_POINT );

    SamplerWrappingMode wrappingMode[] = { wrapX, wrapY, SamplerWrappingMode::SWM_REPEAT };

    auto sampler = new TextureSampler( 0, samplerName, samplingMode, sfm, wrappingMode, glm::vec4( 0.f, 0.f, 0.f, 0.f ) ); 

    return sampler;
}

// **************************
//
std::string     FullscreenEffectOld::GetEffectShadersDir           ()
{
    return "Assets/Shaders/FullscreenEffects/";
}

// *********************************
//
std::string     FullscreenEffectOld::ReadShaderFromFile            ( const std::string & fileName )
{
    std::stringstream shaderSource;

    File::Open( fileName ) >> shaderSource;

    return shaderSource.str();
}

// *********************************
//
std::string     FullscreenEffectOld::ReadFullscreenShader          ( const std::string & fn )
{
    return ReadShaderFromFile( GetEffectShadersDir() + fn );
}

} //bv
