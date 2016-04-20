#include "stdafx.h"
#include "GridLinesEffect.h"

#include "Engine/Models/Builder/RendererStatesBuilder.h"
#include "Engine/Graphics/Shaders/Parameters/ShaderParamFactory.h"
#include "Engine/Graphics/Effects/Utils/ShaderSourceProvider.h"



namespace bv
{

// ***********************
//
GridLinesEffect::GridLinesEffect()
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

// ***********************
//
GridLinesEffect::~GridLinesEffect()
{}


// ****************************
//
PixelShader *       GridLinesEffect::CreatePS        ()
{
    auto params = new ShaderParameters();

    params->AddParameter( ShaderParamFactory::CreateGenericParameter( "color", ParamType::PT_FLOAT4 ) );

    auto shader = new PixelShader( CustomShaderSourceProvider->ReadShader( "grid_lines.frag" ), params );
    
    return shader;
}

// ****************************
//
VertexShader *      GridLinesEffect::CreateVS        ()
{
    auto params = new ShaderParameters();

    auto shader = new VertexShader( CustomShaderSourceProvider->ReadShader( "grid_lines.vert" ), params );

    return shader;
}

// ****************************
//
GeometryShader *    GridLinesEffect::CreateGS        ()
{
    return nullptr;
}


}	// bv

