#include "LightScatteringEffect.h"

#include "Engine/Models/Builder/RendererStatesBuilder.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParamFactory.h"

#include "Engine/Graphics/Effects/LightScatteringShaders.h"


namespace bv {

// ****************************
//
LightScatteringEffect::LightScatteringEffect ()
{
    auto ps = CreatePS();
    auto vs = CreateVS();
    auto gs = CreateGS();

    RenderablePass * pass = new RenderablePass( ps, vs, gs );
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
    //FIXM: register parameters here
    auto shader = new PixelShader( GetLightScatteringPixelShaderCode(), new ShaderParameters() );
    
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

// ****************************
//
GeometryShader *    LightScatteringEffect::CreateGS        ()
{
    auto params = new ShaderParameters();

    params->AddParameter( ShaderParamFactory::CreateViewportMatrixParameter() );

    auto shader = new GeometryShader( GetLightScatteringGeometryShaderCode(), params );

    return shader;
}

} //bv
