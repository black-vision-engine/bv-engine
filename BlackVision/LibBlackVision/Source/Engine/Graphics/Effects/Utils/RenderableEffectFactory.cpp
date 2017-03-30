#include "stdafx.h"
#include "RenderableEffectFactory.h"



#include "Engine/Models/Builder/RendererStatesBuilder.h"
#include "Engine/Graphics/Shaders/Parameters/ShaderParamFactory.h"
#include "Engine/Graphics/Effects/Utils/ShaderSourceProvider.h"


namespace bv
{


// ***********************
//
RenderableEffectPtr         RenderableEffectFactory::CreateBoundingBoxEffect()
{
    auto psParams = new ShaderParameters();
    psParams->AddParameter( ShaderParamFactory::CreateGenericParameter( "color", ParamType::PT_FLOAT4 ) );

    auto vsParams = new ShaderParameters();
    vsParams->AddParameter( ShaderParamFactory::CreateMVPParameter() );


    auto ps = new PixelShader( CustomShaderSourceProvider->ReadShader( "bounding_box.frag" ), psParams );
    auto vs = new VertexShader( CustomShaderSourceProvider->ReadShader( "bounding_box.vert" ), vsParams );

    RenderablePass * pass = new RenderablePass( ps, vs, nullptr );
    auto sinst = pass->GetStateInstance();

    RendererStatesBuilder::Create( sinst );

    auto as = RenderStateAccessor::AccessAlphaState( sinst );
    auto ds = RenderStateAccessor::AccessDepthState( sinst );
    auto cs = RenderStateAccessor::AccessCullState( sinst );

    as->blendEnabled = true;
    ds->enabled = true;
    cs->enabled = false;

    return std::make_shared< RenderableEffect >( pass );
}

// ***********************
//
RenderableEffectPtr         RenderableEffectFactory::CreateGridLinesEffect()
{
    auto vsParams = new ShaderParameters();
    vsParams->AddParameter( ShaderParamFactory::CreateMVPParameter() );

    auto psParams = new ShaderParameters();
    psParams->AddParameter( ShaderParamFactory::CreateGenericParameter( "color", ParamType::PT_FLOAT4 ) );

    auto ps = new PixelShader( CustomShaderSourceProvider->ReadShader( "grid_lines.frag" ), vsParams );
    auto vs = new VertexShader( CustomShaderSourceProvider->ReadShader( "grid_lines.vert" ), psParams );

    RenderablePass * pass = new RenderablePass( ps, vs, nullptr );
    auto sinst = pass->GetStateInstance();

    RendererStatesBuilder::Create( sinst );

    auto as = RenderStateAccessor::AccessAlphaState( sinst );
    auto ds = RenderStateAccessor::AccessDepthState( sinst );
    auto cs = RenderStateAccessor::AccessCullState( sinst );

    as->blendEnabled = true;
    ds->enabled = false;
    cs->enabled = false;

    return std::make_shared< RenderableEffect >( pass );
}

// ***********************
//
RenderableEffectPtr         RenderableEffectFactory::CreateWireframeEffect()
{
    auto vsParams = new ShaderParameters();

    vsParams->AddParameter( ShaderParamFactory::CreateMVPParameter() );
    vsParams->AddParameter( ShaderParamFactory::CreateMVParameter() );

    auto gsParam = new ShaderParameters();
    gsParam->AddParameter( ShaderParamFactory::CreateViewportMatrixParameter() );

    auto vs = new VertexShader( CustomShaderSourceProvider->ReadShader( "wireframe.vert" ), vsParams );
    auto ps = new PixelShader( CustomShaderSourceProvider->ReadShader( "wireframe.frag" ), new ShaderParameters() );
    auto gs = new GeometryShader( CustomShaderSourceProvider->ReadShader( "wireframe.geom" ), gsParam );

    RenderablePass * pass = new RenderablePass( ps, vs, gs );
    auto sinst = pass->GetStateInstance();

    RendererStatesBuilder::Create( sinst );

    auto as = RenderStateAccessor::AccessAlphaState( sinst );
    auto ds = RenderStateAccessor::AccessDepthState( sinst );
    auto cs = RenderStateAccessor::AccessCullState( sinst );

    as->blendEnabled = true;
    ds->enabled = false;
    cs->enabled = false;

    return std::make_shared< RenderableEffect >( pass );
}

// ***********************
//
RenderableEffectPtr         RenderableEffectFactory::CreateLightScatteringEffect()
{
    return RenderableEffectPtr();
}


}	// bv


