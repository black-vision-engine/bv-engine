#include "MixChannelsFullscreenEffect.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParamFactory.h"

#include "Engine/Types/Values/ValuesFactory.h"


namespace bv {

// **************************
//
MixChannelsFullscreenEffect::MixChannelsFullscreenEffect     ( unsigned char rIdx, unsigned char gIdx, unsigned char bIdx, unsigned char aIdx )
{
    SetRIdx( rIdx );
    SetGIdx( gIdx );
    SetBIdx( bIdx );
    SetAIdx( aIdx );

    m_channelMask = ValuesFactory::CreateValueInt( "channelMask" );
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
}

// **************************
//
void    MixChannelsFullscreenEffect::SetGIdx    ( unsigned char idx )
{
    assert( idx < 4 );

    m_gIdx = idx;
}

// **************************
//
void    MixChannelsFullscreenEffect::SetBIdx    ( unsigned char idx )
{
    assert( idx < 4 );

    m_bIdx = idx;
}

// **************************
//
void    MixChannelsFullscreenEffect::SetAIdx    ( unsigned char idx )
{
    assert( idx < 4 );

    m_aIdx = idx;
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

    m_channelMask->SetValue( GetChannelMask() );

    GenericShaderParam * param = ShaderParamFactory::CreateGenericParameter( m_channelMask.get() );

    shaderParams->AddParameter( param );

    return new PixelShader( ReadFullscreenShader( "mixchannels.frag" ), shaderParams );
}

// **************************
//
int                 MixChannelsFullscreenEffect::GetChannelMask         () const
{
    return ( m_aIdx & 0x3 << 6 ) | ( m_bIdx & 0x3 << 4 ) | ( m_gIdx & 0x3 << 2 ) | ( m_rIdx & 0x3 << 6 );
}

} //bv
