#include "stdafx.h"

#include "LightScatteringEffect.h"

#include "Engine/Models/Builder/RendererStatesBuilder.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParamFactory.h"

#include "Engine/Graphics/Effects/LightScatteringShaders.h"

#include "Engine/Models/Plugins/EngineConstantsMapper.h"

namespace bv {

// ****************************
//
LightScatteringEffect::LightScatteringEffect ()
{
    auto ps = CreatePS();
    auto vs = CreateVS();
    //auto gs = CreateGS();

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
LightScatteringEffect::~LightScatteringEffect()
{
    auto pass = GetPass( 0 );

    delete pass->GetPixelShader();
    delete pass->GetVertexShader();
    delete pass->GetGeometryShader();
}

// ****************************
//
PixelShader *       LightScatteringEffect::CreatePS        ()
{
    auto params = new ShaderParameters();

    m_paramExposure = static_cast< ShaderParamFloat * >( ShaderParamFactory::CreateGenericParameter( "exposure", ParamType::PT_FLOAT1 ) );
    m_paramWeight = static_cast< ShaderParamFloat * >( ShaderParamFactory::CreateGenericParameter( "weight", ParamType::PT_FLOAT1 ) );
    m_paramDecay = static_cast< ShaderParamFloat * >( ShaderParamFactory::CreateGenericParameter( "decay", ParamType::PT_FLOAT1 ) );
    m_paramDensity = static_cast< ShaderParamFloat * >( ShaderParamFactory::CreateGenericParameter( "density", ParamType::PT_FLOAT1 ) );
    m_paramLightPositionOnScreen = static_cast< ShaderParamVec2 * >( ShaderParamFactory::CreateGenericParameter( "lightPositionOnScreen", ParamType::PT_FLOAT2 ) );
    m_paramNumSamples = static_cast< ShaderParamFloat * >( ShaderParamFactory::CreateGenericParameter( "numSamples", ParamType::PT_FLOAT1 ) );

    params->AddParameter( m_paramExposure );
    params->AddParameter( m_paramWeight );
    params->AddParameter( m_paramDecay );
    params->AddParameter( m_paramDensity );
    params->AddParameter( m_paramLightPositionOnScreen );
    params->AddParameter( m_paramNumSamples );

    auto shader = new PixelShader( GetLightScatteringPixelShaderCode(), params );
    
    auto sampler = CreateSampler();

    shader->AddTextureSampler( sampler );

    return shader;
}

// ****************************
//
VertexShader *      LightScatteringEffect::CreateVS        ()
{
    auto params = new ShaderParameters();

    params->AddParameter( ShaderParamFactory::CreateMVPParameter() );
    params->AddParameter( ShaderParamFactory::CreateMVParameter() );

    auto shader = new VertexShader( GetLightScatteringVertexShaderCode(), params );

    return shader;
}

//// ****************************
////
//GeometryShader *    LightScatteringEffect::CreateGS        ()
//{
//    auto params = new ShaderParameters();
//
//    params->AddParameter( ShaderParamFactory::CreateViewportMatrixParameter() );
//
//    auto shader = new GeometryShader( GetLightScatteringGeometryShaderCode(), params );
//
//    return shader;
//}

// ****************************
//
void                LightScatteringEffect::SetExposureVal( const IValue * exposureVal )
{
    if( !m_paramExposure )
    {
        
    }

    if( m_paramExposure )
    {
        auto value = QueryTypedValue< ValueFloat >( exposureVal );
        assert( value != nullptr );

        m_paramExposure->SetValue( value->GetValue() );
    }
}

// ****************************
//
void                LightScatteringEffect::SetWeightVal( const IValue * weightVal )
{
    if( !m_paramWeight )
    {
        
    }

    if( m_paramWeight )
    {
        auto value = QueryTypedValue< ValueFloat >( weightVal );
        assert( value != nullptr );

        m_paramWeight->SetValue( value->GetValue() );
    }
}

// ****************************
//
void                LightScatteringEffect::SetDecayVal( const IValue * decayVal )
{
    if( !m_paramDecay )
    {
        
    }

    if( m_paramDecay )
    {
        auto value = QueryTypedValue< ValueFloat >( decayVal );
        assert( value != nullptr );

        m_paramDecay->SetValue( value->GetValue() );
    }
}

// ****************************
//
void                LightScatteringEffect::SetDensityVal( const IValue * densityVal )
{
    if( !m_paramDensity )
    {
        
    }

    if( m_paramDensity )
    {
        auto value = QueryTypedValue< ValueFloat >( densityVal );
        assert( value != nullptr );

        m_paramDensity->SetValue( value->GetValue() );
    }
}

// ****************************
//
void                LightScatteringEffect::SetLightPositionOnScreenVal( const IValue * lightPositionOnScreenVal )
{
    if( !m_paramLightPositionOnScreen )
    {
        m_paramLightPositionOnScreen = static_cast< ShaderParamVec2 * >( ShaderParamFactory::CreateGenericParameter( lightPositionOnScreenVal->GetName(), ParamType::PT_FLOAT2 ) );
    }

    if( m_paramLightPositionOnScreen )
    {
        auto value = QueryTypedValue< ValueVec2 >( lightPositionOnScreenVal );
        assert( value != nullptr );

		m_paramLightPositionOnScreen->SetValue( value->GetValue() );
    }
}

// ****************************
//
void                LightScatteringEffect::SetNumSamplesVal( const IValue * numSamplesVal )
{
    if( !m_paramNumSamples )
    {
        m_paramNumSamples = static_cast< ShaderParamFloat * >( ShaderParamFactory::CreateGenericParameter( numSamplesVal->GetName(), ParamType::PT_FLOAT1 ) );
    }

    if( m_paramNumSamples )
    {
        auto value = QueryTypedValue< ValueFloat >( numSamplesVal );
        assert( value != nullptr );

        m_paramNumSamples->SetValue( value->GetValue() );
    }
}

// ****************************
//
void                LightScatteringEffect::AddTexture                  ( Texture2DPtr texture )
{
    auto params = GetPass( 0 )->GetPixelShader()->GetParameters();
    if( params->NumTextures() > 0 )
    {
        GetPass( 0 )->GetPixelShader()->GetParameters()->SetTexture( 0, texture );
    }
    else
    {
        params->AddTexture( texture );
    }
}

// ****************************
//
TextureSampler *    LightScatteringEffect::CreateSampler   ()
{
    auto wrapX          = EngineConstantsMapper::EngineConstant( TextureWrappingMode::TWM_CLAMP_EDGE );
    auto wrapY          = EngineConstantsMapper::EngineConstant( TextureWrappingMode::TWM_CLAMP_EDGE );            
    auto samplingMode   = SamplerSamplingMode::SSM_MODE_2D;
    auto sfm            = EngineConstantsMapper::EngineConstant( TextureFilteringMode::TFM_LINEAR );

    SamplerWrappingMode wrappingMode[] = { wrapX, wrapY, SamplerWrappingMode::SWM_REPEAT };

    auto sampler = new TextureSampler( 0, "Tex0", samplingMode, sfm, wrappingMode, glm::vec4( 0.f, 0.f, 0.f, 0.f ) );

    return sampler;
}

} //bv
