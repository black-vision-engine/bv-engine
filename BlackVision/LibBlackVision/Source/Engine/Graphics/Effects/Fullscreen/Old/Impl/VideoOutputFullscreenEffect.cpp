#include "VideoOutputFullscreenEffect.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParamFactory.h"

#include "Engine/Types/Values/ValuesFactory.h"




#include "Memory/MemoryLeaks.h"



namespace bv {

// **************************
//
VideoOutputFullscreenEffect::VideoOutputFullscreenEffect      ( Texture2DPtr tex0, Texture2DPtr tex1 )
    : m_pixelShader( nullptr )
    , m_useInterlace( true )
    , m_startEven( false )
    , m_height( 1080 )
    , m_rIdx( 0 )
    , m_gIdx( 1 )
    , m_bIdx( 2 )
    , m_aIdx( 3 )
    , m_alpha( 1.0f )
    , m_overwriteAlpha( 1.0f )
{
    m_useInterlaceValue = ValuesFactory::CreateValueInt     ( "useInterlace" );
    m_startEvenValue    = ValuesFactory::CreateValueInt     ( "startEven" );
    m_heightValue       = ValuesFactory::CreateValueFloat   ( "height" );
    m_channelMaskVal    = ValuesFactory::CreateValueInt     ( "channelMask" );
    m_alphaVal          = ValuesFactory::CreateValueFloat   ( "alpha" );
    m_overwriteAlphaVal = ValuesFactory::CreateValueInt     ( "overwriteAlpha" );

    SetInterlaceEnabled( true );

    SetStartEven( false );

    SetHeight( 1080 );

    SetRIdx( 0 );
    SetGIdx( 1 );
    SetBIdx( 2 );
    SetAIdx( 3 );

    SetAlpha( 1.0f );

    SetOverwriteAlpha( false );

    auto quad = CreateDefaultFullscrQuad( CreatePS( tex0, tex1 ) );
    SetFullscreenQuad( quad );
}

// **************************
//
VideoOutputFullscreenEffect::~VideoOutputFullscreenEffect         ()
{
}

// **************************
//
void                        VideoOutputFullscreenEffect::SwapTextures           ()
{
    auto params = m_pixelShader->GetParameters();

    auto tex0 = params->GetTexture( 0 );
    auto tex1 = params->GetTexture( 1 );

    params->SetTexture( 0, tex1 );
    params->SetTexture( 1, tex0 );
}

// **************************
//
void                        VideoOutputFullscreenEffect::SetInterlaceEnabled    ( bool enabled )
{
    m_useInterlace = enabled;

    m_useInterlaceValue->SetValue( enabled );
    
}

// **************************
//
bool                        VideoOutputFullscreenEffect::GetInterlaceEnabled    () const
{
    return m_useInterlace;
}

// **************************
//
void                        VideoOutputFullscreenEffect::SetStartEven           ( bool startEven )
{
    m_startEven = startEven;

    m_startEvenValue->SetValue( startEven );
}

// **************************
//
bool                        VideoOutputFullscreenEffect::GetStartEven           () const
{
    return m_startEven;
}

// **************************
//
void                        VideoOutputFullscreenEffect::SetHeight              ( int height )
{
    assert( height > 0 );

    m_height = height;

    m_heightValue->SetValue( (float) height );
}

// **************************
//
int                         VideoOutputFullscreenEffect::GetHeight              () const
{
    return m_height;
}

// **************************
//
void                        VideoOutputFullscreenEffect::SetRIdx                ( unsigned char idx )
{
    assert( idx < 4 );

    m_rIdx = idx;

    m_channelMaskVal->SetValue( GetChannelMask() );
}

// **************************
//
void                        VideoOutputFullscreenEffect::SetGIdx                ( unsigned char idx )
{
    assert( idx < 4 );

    m_gIdx = idx;

    m_channelMaskVal->SetValue( GetChannelMask() );
}

// **************************
//
void                        VideoOutputFullscreenEffect::SetBIdx                ( unsigned char idx )
{
    assert( idx < 4 );

    m_bIdx = idx;

    m_channelMaskVal->SetValue( GetChannelMask() );
}

// **************************
//
void                        VideoOutputFullscreenEffect::SetAIdx                ( unsigned char idx )
{
    assert( idx < 4 );

    m_aIdx = idx;

    m_channelMaskVal->SetValue( GetChannelMask() );
}

// **************************
//
unsigned char               VideoOutputFullscreenEffect::GetRIdx                () const
{
    return m_rIdx;
}

// **************************
//
unsigned char               VideoOutputFullscreenEffect::GetGIdx                () const
{
    return m_gIdx;
}

// **************************
//
unsigned char               VideoOutputFullscreenEffect::GetBIdx                () const
{
    return m_bIdx;
}


// **************************
//
unsigned char               VideoOutputFullscreenEffect::GetAIdx                () const
{
    return m_aIdx;
}

// **************************
//
void                        VideoOutputFullscreenEffect::SetAlpha               ( float alpha )
{
    assert( alpha >= 0.0f && alpha <= 1.0f );

    m_alpha = alpha;

    m_alphaVal->SetValue( alpha );
}

// **************************
//
float                       VideoOutputFullscreenEffect::GetAlpha               () const
{
    return m_alpha;
}

// **************************
//
void                        VideoOutputFullscreenEffect::SetOverwriteAlpha      ( bool overwriteAlpha )
{
    m_overwriteAlpha = overwriteAlpha;

    m_overwriteAlphaVal->SetValue( overwriteAlpha );
}

// **************************
//
bool                        VideoOutputFullscreenEffect::GetOverwriteAlpha      () const
{
    return m_overwriteAlpha;
}

// **************************
//
RenderableEntity *  VideoOutputFullscreenEffect::CreateFullscreenQuad           () const
{
    assert( false );

    return nullptr;
}

// **************************
//
PixelShader *               VideoOutputFullscreenEffect::CreatePS               ( Texture2DPtr tex0, Texture2DPtr tex1 ) const
{
    ShaderParameters * shaderParams = new ShaderParameters();

    shaderParams->AddParameter( ShaderParamFactory::CreateGenericParameter( m_useInterlaceValue.get() ) );
    shaderParams->AddParameter( ShaderParamFactory::CreateGenericParameter( m_startEvenValue.get() ) );
    shaderParams->AddParameter( ShaderParamFactory::CreateGenericParameter( m_heightValue.get() ) );
    shaderParams->AddParameter( ShaderParamFactory::CreateGenericParameter( m_channelMaskVal.get() ) );
    shaderParams->AddParameter( ShaderParamFactory::CreateGenericParameter( m_alphaVal.get() ) );
    shaderParams->AddParameter( ShaderParamFactory::CreateGenericParameter( m_overwriteAlphaVal.get() ) );

    shaderParams->AddTexture( tex0 );
    shaderParams->AddTexture( tex1 );

    auto samplerTex0    = CreateDefaultSampler( "Tex0" );
    auto samplerTex1    = CreateDefaultSampler( "Tex1" );

    auto shader = new PixelShader( ReadFullscreenShader( "interlaceoverwritechannels.frag" ), shaderParams );

    shader->AddTextureSampler( samplerTex0 );
    shader->AddTextureSampler( samplerTex1 );

    m_pixelShader = shader;

    return shader;
}

// **************************
//
int                         VideoOutputFullscreenEffect::GetChannelMask         () const
{
    return ( ( m_aIdx & 0x3 ) << 6 ) | ( ( m_bIdx & 0x3 ) << 4 ) | ( ( m_gIdx & 0x3 ) << 2 ) | ( ( m_rIdx & 0x3 ) << 0 );
}

} //bv
