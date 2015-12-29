#include "MixChannelsFullscreenEffect.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParamFactory.h"

#include "Engine/Types/Values/ValuesFactory.h"


namespace bv {

// **************************
//
MixChannelsFullscreenEffect::MixChannelsFullscreenEffect     ( unsigned char rIdx, unsigned char gIdx, unsigned char bIdx, unsigned char aIdx )
    : m_rIdx( 0 )
    , m_gIdx( 1 )
    , m_bIdx( 2 )
    , m_aIdx( 3 )
{
    m_channelMaskVal = ValuesFactory::CreateValueInt( "channelMask" );

    SetRIdx( rIdx );
    SetGIdx( gIdx );
    SetBIdx( bIdx );
    SetAIdx( aIdx );
}

// **************************
//
MixChannelsFullscreenEffect::~MixChannelsFullscreenEffect    ()
{
}

// **************************
//
void    MixChannelsFullscreenEffect::SetRIdx    ( unsigned char idx )
{
    assert( idx < 4 );

    m_rIdx = idx;

    m_channelMaskVal->SetValue( GetChannelMask() );
}

// **************************
//
void    MixChannelsFullscreenEffect::SetGIdx    ( unsigned char idx )
{
    assert( idx < 4 );

    m_gIdx = idx;

    m_channelMaskVal->SetValue( GetChannelMask() );
}

// **************************
//
void    MixChannelsFullscreenEffect::SetBIdx    ( unsigned char idx )
{
    assert( idx < 4 );

    m_bIdx = idx;

    m_channelMaskVal->SetValue( GetChannelMask() );
}

// **************************
//
void    MixChannelsFullscreenEffect::SetAIdx    ( unsigned char idx )
{
    assert( idx < 4 );

    m_aIdx = idx;

    m_channelMaskVal->SetValue( GetChannelMask() );
}

// **************************
//
unsigned char   MixChannelsFullscreenEffect::GetRIdx    () const
{
    return m_rIdx;
}

// **************************
//
unsigned char   MixChannelsFullscreenEffect::GetGIdx    () const
{
    return m_gIdx;
}

// **************************
//
unsigned char   MixChannelsFullscreenEffect::GetBIdx    () const
{
    return m_bIdx;
}

// **************************
//
unsigned char   MixChannelsFullscreenEffect::GetAIdx    () const
{
    return m_aIdx;
}

// **************************
//
RenderableEntity *  MixChannelsFullscreenEffect::CreateFullscreenQuad   () const
{
    return CreateDefaultFullscrQuad( CreatePS() );
}

// **************************
//
PixelShader *       MixChannelsFullscreenEffect::CreatePS               () const
{
    ShaderParameters * shaderParams = new ShaderParameters();

    GenericShaderParam * param = ShaderParamFactory::CreateGenericParameter( m_channelMaskVal.get() );

    shaderParams->AddParameter( param );

    //FIXME: add empty textures (nullptr) and create samplers. Textures can be set later on
    return new PixelShader( ReadFullscreenShader( "mixchannels.frag" ), shaderParams );
}

// **************************
//
int                 MixChannelsFullscreenEffect::GetChannelMask         () const
{
    return ( m_aIdx & 0x3 << 6 ) | ( m_bIdx & 0x3 << 4 ) | ( m_gIdx & 0x3 << 2 ) | ( m_rIdx & 0x3 << 0 );
}

} //bv
