#include "stdafx.h"

#include "BoundingBoxEffect.h"

#include "Engine/Models/Builder/RendererStatesBuilder.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParamFactory.h"

#include "Engine/Graphics/Effects/Utils/ShaderSourceProvider.h"


namespace bv {

// ****************************
//
BoundingBoxEffect::BoundingBoxEffect ()
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
BoundingBoxEffect::~BoundingBoxEffect()
{}

// ****************************
//
PixelShader *       BoundingBoxEffect::CreatePS        ()
{
    //FIXM: register additional parameters here
    auto shader = new PixelShader( CustomShaderSourceProvider->ReadShader( "bounding_box.frag" ), new ShaderParameters() );
    
    return shader;
}

// ****************************
//
VertexShader *      BoundingBoxEffect::CreateVS        ()
{
    auto params = new ShaderParameters();

    params->AddParameter( ShaderParamFactory::CreateMVPParameter() );
    params->AddParameter( ShaderParamFactory::CreateMVParameter() );

    auto shader = new VertexShader( CustomShaderSourceProvider->ReadShader( "bounding_box.vert" ), params );

    return shader;
}

// ****************************
//
GeometryShader *    BoundingBoxEffect::CreateGS        ()
{
    auto params = new ShaderParameters();

    params->AddParameter( ShaderParamFactory::CreateViewportMatrixParameter() );

    auto shader = new GeometryShader( CustomShaderSourceProvider->ReadShader( "bounding_box.geom" ), params );

    return shader;
}

} //bv
