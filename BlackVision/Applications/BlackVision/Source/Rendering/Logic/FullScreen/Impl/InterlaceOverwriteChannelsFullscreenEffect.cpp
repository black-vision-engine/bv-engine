#include "InterlaceOverwriteChannelsFullscreenEffect.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParamFactory.h"

#include "Engine/Types/Values/ValuesFactory.h"


namespace bv {

// **************************
//
InterlaceOverwriteChannelsFullscreenEffect::InterlaceOverwriteChannelsFullscreenEffect      ( unsigned char rIdx, unsigned char gIdx, unsigned char bIdx, unsigned char aIdx, float alpha, bool startEven, int height )
    : m_startEven( startEven )
    , m_height( height )
    , m_rIdx( 0 )
    , m_gIdx( 1 )
    , m_bIdx( 2 )
    , m_aIdx( 3 )
    , m_alpha( 1.0f )
    , m_overwriteAlpha( 1.0f )
{
    m_startEvenValue    = ValuesFactory::CreateValueInt( "startEven" );
    m_heightValue       = ValuesFactory::CreateValueInt( "height" );
    m_channelMaskVal    = ValuesFactory::CreateValueInt( "channelMask" );
    m_alphaVal          = ValuesFactory::CreateValueFloat( "alpha" );
    m_overwriteAlphaVal = ValuesFactory::CreateValueInt( "overwriteAlpha" );

    SetStartEven( startEven );

    SetHeight( height );

    SetRIdx( rIdx );
    SetGIdx( gIdx );
    SetBIdx( bIdx );
    SetAIdx( aIdx );

    SetAlpha( alpha );

    SetOverwriteAlpha( false );
}

// **************************
//
InterlaceOverwriteChannelsFullscreenEffect::~InterlaceOverwriteChannelsFullscreenEffect         ()
{

}

// **************************
//
void                        InterlaceOverwriteChannelsFullscreenEffect::SetInterlaceTextures    ( Texture2DPtr tex0, Texture2DPtr tex1 )
{
    {tex0; tex1;}
}

// **************************
//
void                        InterlaceOverwriteChannelsFullscreenEffect::SetStartEven            ( bool startEven )
{
    m_startEven = startEven;

    m_startEvenValue->SetValue( startEven );
}

// **************************
//
bool                        InterlaceOverwriteChannelsFullscreenEffect::GetStartEven            () const
{
    return m_startEven;
}

// **************************
//
void                        InterlaceOverwriteChannelsFullscreenEffect::SetHeight               ( int height )
{
    assert( height > 0 );

    m_height = height;

    m_heightValue->SetValue( height );
}

// **************************
//
int                         InterlaceOverwriteChannelsFullscreenEffect::GetHeight               () const
{
    return m_height;
}

// **************************
//
void                        InterlaceOverwriteChannelsFullscreenEffect::SetRIdx                 ( unsigned char idx )
{
    assert( idx < 4 );

    m_rIdx = idx;

    m_channelMaskVal->SetValue( GetChannelMask() );
}

// **************************
//
void                        InterlaceOverwriteChannelsFullscreenEffect::SetGIdx                 ( unsigned char idx )
{
    assert( idx < 4 );

    m_gIdx = idx;

    m_channelMaskVal->SetValue( GetChannelMask() );
}

// **************************
//
void                        InterlaceOverwriteChannelsFullscreenEffect::SetBIdx                 ( unsigned char idx )
{
    assert( idx < 4 );

    m_bIdx = idx;

    m_channelMaskVal->SetValue( GetChannelMask() );
}

// **************************
//
void                        InterlaceOverwriteChannelsFullscreenEffect::SetAIdx                 ( unsigned char idx )
{
    assert( idx < 4 );

    m_aIdx = idx;

    m_channelMaskVal->SetValue( GetChannelMask() );
}

// **************************
//
unsigned char               InterlaceOverwriteChannelsFullscreenEffect::GetRIdx                 () const
{
    return m_rIdx;
}

// **************************
//
unsigned char               InterlaceOverwriteChannelsFullscreenEffect::GetGIdx                 () const
{
    return m_gIdx;
}

// **************************
//
unsigned char               InterlaceOverwriteChannelsFullscreenEffect::GetBIdx                 () const
{
    return m_bIdx;
}


// **************************
//
unsigned char               InterlaceOverwriteChannelsFullscreenEffect::GetAIdx                 () const
{
    return m_aIdx;
}

// **************************
//
void                        InterlaceOverwriteChannelsFullscreenEffect::SetAlpha                ( float alpha )
{
    assert( alpha >= 0.0f && alpha <= 1.0f );

    m_alpha = alpha;

    m_alphaVal->SetValue( alpha );
}

// **************************
//
float                       InterlaceOverwriteChannelsFullscreenEffect::GetAlpha                () const
{
    return m_alpha;
}

// **************************
//
void                        InterlaceOverwriteChannelsFullscreenEffect::SetOverwriteAlpha       ( bool overwriteAlpha )
{
    m_overwriteAlpha = overwriteAlpha;

    m_overwriteAlphaVal->SetValue( overwriteAlpha );
}

// **************************
//
bool                        InterlaceOverwriteChannelsFullscreenEffect::GetOverwriteAlpha       () const
{
    return m_overwriteAlpha;
}

// **************************
//
RenderableEntity *  InterlaceOverwriteChannelsFullscreenEffect::CreateFullscreenQuad            () const
{
    return CreateDefaultFullscrQuad( CreatePS() );
}

// **************************
//
PixelShader *               InterlaceOverwriteChannelsFullscreenEffect::CreatePS                () const
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
int                         InterlaceOverwriteChannelsFullscreenEffect::GetChannelMask          () const
{
    return ( m_aIdx & 0x3 << 6 ) | ( m_bIdx & 0x3 << 4 ) | ( m_gIdx & 0x3 << 2 ) | ( m_rIdx & 0x3 << 6 );
}

} //bv
