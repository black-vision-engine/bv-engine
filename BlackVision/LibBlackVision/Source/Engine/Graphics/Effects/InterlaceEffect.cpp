#include "InterlaceEffect.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParamFactory.h"
#include "Engine/Models/Builder/RendererStatesBuilder.h"

#include "Engine/Models/Plugins/EngineConstantsMapper.h"

#include "Engine/Graphics/Effects/InterlaceShaders.h"


namespace bv {

// ****************************
//
InterlaceEffect::InterlaceEffect (  Texture2DPtr tex0, Texture2DPtr tex1  )
{
    auto ps = CreatePS( tex0, tex1 );
    auto vs = CreateVS();

    RenderablePass * pass = new RenderablePass( ps, vs, nullptr );
    auto sinst = pass->GetStateInstance();

    RendererStatesBuilder::Create( sinst );

    auto as = RenderStateAccessor::AccessAlphaState( sinst );
    auto ds = RenderStateAccessor::AccessDepthState( sinst );
    auto cs = RenderStateAccessor::AccessCullState( sinst );

    as->blendEnabled = false;
    ds->enabled = false;
    cs->enabled = false;

    AddPass( pass );
}

// ****************************
//
InterlaceEffect::~InterlaceEffect()
{
    auto pass = GetPass( 0 );

    delete pass->GetPixelShader();
    delete pass->GetVertexShader();
}

// ****************************
//
PixelShader *       InterlaceEffect::CreatePS        ( Texture2DPtr tex0, Texture2DPtr tex1 )
{
    auto params = new ShaderParameters();
    params->AddTexture( tex0 );
    params->AddTexture( tex1 );

    m_firstTextureIdxParam = static_cast< ShaderParamInt * >( ShaderParamFactory::CreateGenericParameter( "first_texture_idx", ParamType::PT_INT ) );
    assert( m_firstTextureIdxParam != nullptr );

    auto shader = new PixelShader( GetInterlacePixelShaderCode(), params );

    auto sampler0 = CreateSampler( 0, "tex0" );
    auto sampler1 = CreateSampler( 1, "tex1" );

    shader->AddTextureSampler( sampler0 );
    shader->AddTextureSampler( sampler1 );

    return shader;
}

// ****************************
//
VertexShader *      InterlaceEffect::CreateVS        ()
{
    auto params     = new ShaderParameters();
    auto mvpParam   = ShaderParamFactory::CreateMVPParameter();

    params->AddParameter( mvpParam );

    auto shader = new VertexShader( GetInterlaceVertexShaderCode(), params );

    return shader;
}

// ****************************
//
TextureSampler *    InterlaceEffect::CreateSampler  ( int samplerId, const std::string & samplerName ) const
{
    auto wrapX          = EngineConstantsMapper::EngineConstant( TextureWrappingMode::TWM_CLAMP );
    auto wrapY          = EngineConstantsMapper::EngineConstant( TextureWrappingMode::TWM_CLAMP );            
    auto samplingMode   = SamplerSamplingMode::SSM_MODE_2D;
    auto sfm            = EngineConstantsMapper::EngineConstant( TextureFilteringMode::TFM_POINT );

    SamplerWrappingMode wrappingMode[] = { wrapX, wrapY, SamplerWrappingMode::SWM_REPEAT };

    auto sampler = new TextureSampler( samplerId, samplerName, samplingMode, sfm, wrappingMode, glm::vec4( 0.f, 0.f, 0.f, 0.f ) ); 

    return sampler;
}

} // bv
