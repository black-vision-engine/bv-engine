#include "Texture2DEffect.h"

#include "Engine/Interfaces/IValue.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParamFactory.h"

#include "Engine/Graphics/Effects/Texture2DEffectShaders.h"

#include "Engine/Models/Plugins/EngineConstantsMapper.h"

#include "Engine/Models/Builder/RendererStatesBuilder.h"


namespace bv {

// ****************************
//
Texture2DEffect::Texture2DEffect    ( Texture2DPtr texture, bool hasAlpha, TextureFilteringMode filteringMode, TextureWrappingMode wrapModeX, TextureWrappingMode wrapModeY, const glm::vec4 & borderColor )
    : m_alphaParam( nullptr )
{
    auto ps = CreatePS( texture, filteringMode, wrapModeX, wrapModeY, borderColor, hasAlpha );
    auto vs = CreateVS();

    RenderablePass * pass = new RenderablePass( ps, vs, nullptr );
    auto sinst = pass->GetStateInstance();

    RendererStatesBuilder::Create( sinst );

    auto as = RenderStateAccessor::AccessAlphaState( sinst );
    auto ds = RenderStateAccessor::AccessDepthState( sinst );
    auto cs = RenderStateAccessor::AccessCullState( sinst );

    as->blendEnabled = true;
    ds->enabled = false;
    cs->enabled = false;

    AddPass( pass );
}

// ****************************
//
Texture2DEffect::~Texture2DEffect    ()
{
    auto pass = GetPass( 0 );

    delete pass->GetPixelShader();
    delete pass->GetVertexShader();
}

// ****************************
//
void    Texture2DEffect::SetAlphaValModel   ( const IValue * val )
{
    if( m_alphaParam )
    {
        auto value = QueryTypedValue< ValueFloat >( val );
        assert( value != nullptr );

        m_alphaParam->SetModelValue( value );
    }
}

// ****************************
//
PixelShader *   Texture2DEffect::CreatePS   ( Texture2DPtr texture, TextureFilteringMode filteringMode, TextureWrappingMode wrapModeX, TextureWrappingMode wrapModeY, const glm::vec4 & borderColor, bool hasAlpha )
{
    assert( texture != nullptr );

    auto params = new ShaderParameters();
    params->AddTexture( texture );

    if( hasAlpha )
    {
        m_alphaParam = static_cast< ShaderParamFloat * >( ShaderParamFactory::CreateGenericParameter( GetTexture2DEffectAlphaParamName(), ParamType::PT_FLOAT1 ) );
        assert( m_alphaParam != nullptr );

        params->AddParameter( m_alphaParam );
    }

    auto shader = new PixelShader( GetTexture2DEffectPixelShaderSource( hasAlpha ), params );
    auto sampler = CreateSampler( filteringMode, wrapModeX, wrapModeY, borderColor );

    shader->AddTextureSampler( sampler );

    return shader;
}

// ****************************
//
VertexShader *  Texture2DEffect::CreateVS   ()
{
    auto params     = new ShaderParameters();
    auto mvpParam   = ShaderParamFactory::CreateMVPParameter();

    params->AddParameter( mvpParam );

    auto shader = new VertexShader( GetTexture2DEffectVertexShaderSource(), params );

    return shader;
}

// ****************************
//
TextureSampler *    Texture2DEffect::CreateSampler   ( TextureFilteringMode filteringMode, TextureWrappingMode wrapModeX, TextureWrappingMode wrapModeY, const glm::vec4 & borderColor )
{
    auto wrapX          = EngineConstantsMapper::EngineConstant( wrapModeX );
    auto wrapY          = EngineConstantsMapper::EngineConstant( wrapModeY );            
    auto samplingMode   = SamplerSamplingMode::SSM_MODE_2D;
    auto sfm            = EngineConstantsMapper::EngineConstant( filteringMode );

    SamplerWrappingMode wrappingMode[] = { wrapX, wrapY, SamplerWrappingMode::SWM_REPEAT };

    auto sampler = new TextureSampler( 0, GetTexture2DEffectTextureSamplerName(), samplingMode, sfm, wrappingMode, borderColor ); 

    return sampler;
}

} //bv
