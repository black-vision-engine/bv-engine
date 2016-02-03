#include "stdafx.h"

#include "MaskChannelsEffect.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParamFactory.h"
#include "Engine/Models/Builder/RendererStatesBuilder.h"

#include "Engine/Models/Plugins/EngineConstantsMapper.h"

#include "Engine/Graphics/Effects/MaskChannelsShaders.h"


namespace bv {

// ****************************
//
MaskChannelEffect::MaskChannelEffect ()
    : m_maskParam( nullptr )
{
    auto ps = CreatePS();
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
MaskChannelEffect::~MaskChannelEffect()
{
    auto pass = GetPass( 0 );

    delete pass->GetPixelShader();
    delete pass->GetVertexShader();
}

// ****************************
//
PixelShader *       MaskChannelEffect::CreatePS        ()
{
    auto params = new ShaderParameters();

    m_maskParam = static_cast< ShaderParamInt * >( ShaderParamFactory::CreateGenericParameter( "mask_type", ParamType::PT_INT ) );
    assert( m_maskParam != nullptr );

    auto shader = new PixelShader( GetMaskChannelPixelShaderCode(), params );

    return shader;
}

// ****************************
//
VertexShader *      MaskChannelEffect::CreateVS        ()
{
    auto params     = new ShaderParameters();
    auto mvpParam   = ShaderParamFactory::CreateMVPParameter();

    params->AddParameter( mvpParam );

    auto shader = new VertexShader( GetMaskChannelVertexShaderCode(), params );

    return shader;
}

} // bv

