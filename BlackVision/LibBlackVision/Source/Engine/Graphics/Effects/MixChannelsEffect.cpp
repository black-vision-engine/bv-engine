#include "MixChannelsEffect.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParamFactory.h"
#include "Engine/Models/Builder/RendererStatesBuilder.h"

#include "Engine/Types/Values/ValuesFactory.h"

#include "Engine/Models/Plugins/EngineConstantsMapper.h"

#include "Engine/Graphics/Effects/MixChannelsShaders.h"


namespace bv {

// ****************************
//
MixChannelsEffect::MixChannelsEffect ( Texture2DPtr tex )
    : m_mixMaskParam( nullptr )
    , m_maskMaskParam( nullptr )
    , m_channelMixMaskVal( nullptr )
    , m_channelMaskMaskVal( nullptr )
{
    m_channelMixMaskVal     = ValuesFactory::CreateValueInt( "channelMixMask" );
    m_channelMaskMaskVal    = ValuesFactory::CreateValueVec4( "channelMaskMask" );
    
    auto ps = CreatePS( tex );
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

    m_channelMixMaskVal->SetValue( GetChannelMask() );
}

// **************************
//
void    MixChannelsEffect::SetGIdx    ( unsigned char idx )
{
    assert( idx < 4 );

    m_gIdx = idx;

    m_channelMixMaskVal->SetValue( GetChannelMask() );
}

// **************************
//
void    MixChannelsEffect::SetBIdx    ( unsigned char idx )
{
    assert( idx < 4 );

    m_bIdx = idx;

    m_channelMixMaskVal->SetValue( GetChannelMask() );
}

// **************************
//
void    MixChannelsEffect::SetAIdx      ( unsigned char idx )
{
    assert( idx < 4 );

    m_aIdx = idx;

    m_channelMixMaskVal->SetValue( GetChannelMask() );
}

// **************************
//
glm::vec4   MixChannelsEffect::GetMask         () const
{
    return m_maskMask;
}

// **************************
//
void    MixChannelsEffect::SetMask      ( const glm::vec4 & mask )
{
    m_maskMask = mask;

    m_channelMaskMaskVal->SetValue( mask );
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
PixelShader *       MixChannelsEffect::CreatePS        ( Texture2DPtr tex )
{
    ShaderParameters * shaderParams = new ShaderParameters();

    GenericShaderParam * param0 = ShaderParamFactory::CreateGenericParameter( m_channelMixMaskVal.get() );
    GenericShaderParam * param1 = ShaderParamFactory::CreateGenericParameter( m_channelMaskMaskVal.get() );

    shaderParams->AddParameter( param0 );
    shaderParams->AddParameter( param1 );

    auto shader = new PixelShader( GetMixChannelPixelShaderCode(), shaderParams );

    //FIXME: create samplers

    //FIXME: add textures
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
    return ( ( m_aIdx & 0x3 ) << 6 ) | ( ( m_bIdx & 0x3 ) << 4 ) | ( ( m_gIdx & 0x3 ) << 2 ) | ( ( m_rIdx & 0x3 ) << 0 );
}

} // bv

