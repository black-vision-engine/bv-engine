#include "BlurFullscreenEffect.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParamFactory.h"

#include "Engine/Types/Values/ValuesFactory.h"

#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Engine/Interfaces/IValue.h"

namespace bv {

// **************************
//
void            BlurFullscreenEffect::SetBlurSize   ( float s )
{
    m_blurSize = s;
    m_blurSizeVal->SetValue( s );
}

// **************************
//
float           BlurFullscreenEffect::GetBlurSize   () const
{
    return m_blurSize;
}

// **************************
//
void            BlurFullscreenEffect::SetVertical ( bool val )
{
    m_vertical = val;
    m_verticalVal->SetValue( val );

    if( m_pixelShader )
    {
        static_cast< ShaderParamInt * >( m_pixelShader->GetParameters()->AccessParam( 2 ) )->SetValue( val );
    }
}

// **************************
//
bool            BlurFullscreenEffect::GetVertical () const
{
    return m_vertical;
}

// **************************
//
void            BlurFullscreenEffect::SetTexture  ( Texture2DPtr tex )
{
    m_texture = tex;
    m_textureSize->SetValue( glm::vec2( float( tex->GetWidth() ), float( tex->GetHeight() ) ) );
    
    if( m_pixelShader )
    {
        m_pixelShader->GetParameters()->SetTexture( 0, tex );
    }
}

// **************************
//
PixelShader *   BlurFullscreenEffect::CreatePS            () const
{
    ShaderParameters * shaderParams = new ShaderParameters();

    auto param = ShaderParamFactory::CreateGenericParameter( m_blurSizeVal.get() );
    shaderParams->AddParameter( param );

    param = ShaderParamFactory::CreateGenericParameter( m_textureSize.get() );
    shaderParams->AddParameter( param );

    shaderParams->AddTexture( m_texture );

    param = ShaderParamFactory::CreateGenericParameter( m_verticalVal.get() );
    shaderParams->AddParameter( param );

    //FIXME: add empty textures (nullptr) and create samplers. Textures can be set later on
    auto shader = new PixelShader( ReadFullscreenShader( "blur.frag" ), shaderParams );

    auto samplerTex = CreateDefaultSampler( "Tex0" );

    shader->AddTextureSampler( samplerTex );

    return shader;
}

// **************************
//
BlurFullscreenEffect::BlurFullscreenEffect  ( Texture2DPtr tex )
    : m_pixelShader( nullptr )
{
    m_blurSizeVal   = ValuesFactory::CreateValueFloat( "blurSize" );

    m_textureSize   = ValuesFactory::CreateValueVec2( "textureSize" );

    m_textureSize->SetValue( glm::vec2( float( tex->GetWidth() ), float( tex->GetHeight() ) ) );

    m_texture       = tex;

    m_verticalVal   = ValuesFactory::CreateValueInt( "vertical" );
    m_verticalVal->SetValue( 0 );
}

// **************************
//
BlurFullscreenEffect::~BlurFullscreenEffect ()
{}

// ****************************
//
RenderableEntity *  BlurFullscreenEffect::CreateFullscreenQuad  () const
{
    m_pixelShader = CreatePS();
    auto quad = CreateDefaultFullscrQuad( m_pixelShader );

    auto eff  = quad->GetRenderableEffect();
    auto pass = eff->GetPass( 0 );

    auto sinst = pass->GetStateInstance();
    
    auto as = RenderStateAccessor::AccessAlphaState( sinst );

    as->blendEnabled = true;

    return quad;
}

} // bv
