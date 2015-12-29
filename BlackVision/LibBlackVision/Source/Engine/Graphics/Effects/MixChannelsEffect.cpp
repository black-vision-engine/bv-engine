#include "MixChannelsEffect.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParamFactory.h"
#include "Engine/Models/Builder/RendererStatesBuilder.h"

#include "Engine/Types/Values/ValuesFactory.h"

#include "Engine/Models/Plugins/EngineConstantsMapper.h"

#include "Engine/Graphics/Effects/MixChannelsShaders.h"


namespace bv {

// ****************************
//
MixChannelsEffect::MixChannelsEffect ()
    : m_maskParam( nullptr )
    , m_channelMaskVal( nullptr )
{
    m_channelMaskVal    = ValuesFactory::CreateValueInt( "channelMask" );
    
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
MixChannelsEffect::~MixChannelsEffect()
{
    auto pass = GetPass( 0 );

    delete pass->GetPixelShader();
    delete pass->GetVertexShader();
}

// **************************
//
void    MixChannelsEffect::SetRIdx    ( unsigned char idx )
{
    assert( idx < 4 );

    m_rIdx = idx;

    m_channelMaskVal->SetValue( GetChannelMask() );
}

// **************************
//
void    MixChannelsEffect::SetGIdx    ( unsigned char idx )
{
    assert( idx < 4 );

    m_gIdx = idx;

    m_channelMaskVal->SetValue( GetChannelMask() );
}

// **************************
//
void    MixChannelsEffect::SetBIdx    ( unsigned char idx )
{
    assert( idx < 4 );

    m_bIdx = idx;

    m_channelMaskVal->SetValue( GetChannelMask() );
}

// **************************
//
void    MixChannelsEffect::SetAIdx    ( unsigned char idx )
{
    assert( idx < 4 );

    m_aIdx = idx;

    m_channelMaskVal->SetValue( GetChannelMask() );
}

// **************************
//
unsigned char   MixChannelsEffect::GetRIdx    () const
{
    return m_rIdx;
}

// **************************
//
unsigned char   MixChannelsEffect::GetGIdx    () const
{
    return m_gIdx;
}

// **************************
//
unsigned char   MixChannelsEffect::GetBIdx    () const
{
    return m_bIdx;
}

// **************************
//
unsigned char   MixChannelsEffect::GetAIdx    () const
{
    return m_aIdx;
}

// ****************************
//
PixelShader *       MixChannelsEffect::CreatePS        ()
{
    ShaderParameters * shaderParams = new ShaderParameters();

    GenericShaderParam * param = ShaderParamFactory::CreateGenericParameter( m_channelMaskVal.get() );

    shaderParams->AddParameter( param );

    auto shader = new PixelShader( GetMixChannelPixelShaderCode(), shaderParams );

    return shader;
}

// ****************************
//
VertexShader *      MixChannelsEffect::CreateVS        ()
{
    auto params     = new ShaderParameters();
    auto mvpParam   = ShaderParamFactory::CreateMVPParameter();

    params->AddParameter( mvpParam );

    auto shader = new VertexShader( GetMixChannelVertexShaderCode(), params );

    return shader;
}

// ****************************
//
int                 MixChannelsEffect::GetChannelMask   () const
{
    return ( m_aIdx & 0x3 << 6 ) | ( m_bIdx & 0x3 << 4 ) | ( m_gIdx & 0x3 << 2 ) | ( m_rIdx & 0x3 << 0 );
}

} // bv

