#include "OverwriteChannelsFullscreenEffect.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParamFactory.h"

#include "Engine/Types/Values/ValuesFactory.h"




#include "Memory/MemoryLeaks.h"



namespace bv {

// **************************
//
OverwriteChannelsFullscreenEffect::OverwriteChannelsFullscreenEffect                ( unsigned char rIdx, unsigned char gIdx, unsigned char bIdx, unsigned char aIdx, float alpha )
    : m_rIdx( 0 )
    , m_gIdx( 1 )
    , m_bIdx( 2 )
    , m_aIdx( 3 )
    , m_alpha( 1.0f )
    , m_overwriteAlpha( 1.0f )
{
    m_channelMaskVal    = ValuesFactory::CreateValueInt( "channelMask" );
    m_alphaVal          = ValuesFactory::CreateValueFloat( "alpha" );
    m_overwriteAlphaVal = ValuesFactory::CreateValueInt( "overwriteAlpha" );

    SetRIdx( rIdx );
    SetGIdx( gIdx );
    SetBIdx( bIdx );
    SetAIdx( aIdx );

    SetAlpha( alpha );

    SetOverwriteAlpha( false );
}

// **************************
//
OverwriteChannelsFullscreenEffect::~OverwriteChannelsFullscreenEffect               ()
{

}

// **************************
//
void                        OverwriteChannelsFullscreenEffect::SetRIdx              ( unsigned char idx )
{
    assert( idx < 4 );

    m_rIdx = idx;

    m_channelMaskVal->SetValue( GetChannelMask() );
}

// **************************
//
void                        OverwriteChannelsFullscreenEffect::SetGIdx              ( unsigned char idx )
{
    assert( idx < 4 );

    m_gIdx = idx;

    m_channelMaskVal->SetValue( GetChannelMask() );
}

// **************************
//
void                        OverwriteChannelsFullscreenEffect::SetBIdx              ( unsigned char idx )
{
    assert( idx < 4 );

    m_bIdx = idx;

    m_channelMaskVal->SetValue( GetChannelMask() );
}

// **************************
//
void                        OverwriteChannelsFullscreenEffect::SetAIdx              ( unsigned char idx )
{
    assert( idx < 4 );

    m_aIdx = idx;

    m_channelMaskVal->SetValue( GetChannelMask() );
}

// **************************
//
unsigned char               OverwriteChannelsFullscreenEffect::GetRIdx              () const
{
    return m_rIdx;
}

// **************************
//
unsigned char               OverwriteChannelsFullscreenEffect::GetGIdx              () const
{
    return m_gIdx;
}

// **************************
//
unsigned char               OverwriteChannelsFullscreenEffect::GetBIdx              () const
{
    return m_bIdx;
}


// **************************
//
unsigned char               OverwriteChannelsFullscreenEffect::GetAIdx              () const
{
    return m_aIdx;
}

// **************************
//
void                        OverwriteChannelsFullscreenEffect::SetAlpha             ( float alpha )
{
    assert( alpha >= 0.0f && alpha <= 1.0f );

    m_alpha = alpha;

    m_alphaVal->SetValue( alpha );
}

// **************************
//
float                       OverwriteChannelsFullscreenEffect::GetAlpha             () const
{
    return m_alpha;
}

// **************************
//
void                        OverwriteChannelsFullscreenEffect::SetOverwriteAlpha    ( bool overwriteAlpha )
{
    m_overwriteAlpha = overwriteAlpha;

    m_overwriteAlphaVal->SetValue( overwriteAlpha );
}

// **************************
//
bool                        OverwriteChannelsFullscreenEffect::GetOverwriteAlpha    () const
{
    return m_overwriteAlpha;
}

// **************************
//
RenderableEntity *  OverwriteChannelsFullscreenEffect::CreateFullscreenQuad         () const
{
    return CreateDefaultFullscrQuad( CreatePS() );
}

// **************************
//
PixelShader *               OverwriteChannelsFullscreenEffect::CreatePS             () const
{
    ShaderParameters * shaderParams = new ShaderParameters();

    shaderParams->AddParameter( ShaderParamFactory::CreateGenericParameter( m_channelMaskVal.get() ) );
    shaderParams->AddParameter( ShaderParamFactory::CreateGenericParameter( m_alphaVal.get() ) );
    shaderParams->AddParameter( ShaderParamFactory::CreateGenericParameter( m_overwriteAlphaVal.get() ) );

    //FIXME: add empty textures (nullptr) and create samplers. Textures can be set later on
    return new PixelShader( ReadFullscreenShader( "overwritechannels.frag" ), shaderParams );
}

// **************************
//
int                         OverwriteChannelsFullscreenEffect::GetChannelMask       () const
{
    return ( m_aIdx & 0x3 << 6 ) | ( m_bIdx & 0x3 << 4 ) | ( m_gIdx & 0x3 << 2 ) | ( m_rIdx & 0x3 << 0 );
}

} //bv
