#include "WireframeEffect.h"

#include "Engine/Models/Builder/RendererStatesBuilder.h"

#include "Engine/Graphics/Effects/WireframeShaders.h"


namespace bv {

// ****************************
//
WireframeEffect::WireframeEffect ()
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
WireframeEffect::~WireframeEffect()
{
    auto pass = GetPass( 0 );

    delete pass->GetPixelShader();
    delete pass->GetVertexShader();
    delete pass->GetGeometryShader();
}

// ****************************
//
PixelShader *       WireframeEffect::CreatePS        ()
{
    auto shader = new PixelShader( GetWireframePixelShaderCode(), new ShaderParameters() );

    return shader;
}

// ****************************
//
VertexShader *      WireframeEffect::CreateVS        ()
{
    auto shader = new VertexShader( GetWireframeVertexShaderCode(), new ShaderParameters() );

    return shader;
}

// ****************************
//
GeometryShader *    WireframeEffect::CreateGS        ()
{
    auto shader = new GeometryShader( GetWireframeGeometryShaderCode(), new ShaderParameters() );

    return shader;
}

} //bv
