#include "BlurFullscreenEffect.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParamFactory.h"

#include "Engine/Types/Values/ValuesFactory.h"

#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

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
PixelShader *   BlurFullscreenEffect::CreatePS            () const
{
    ShaderParameters * shaderParams = new ShaderParameters();

    auto param = ShaderParamFactory::CreateGenericParameter( m_blurSizeVal.get() );
    shaderParams->AddParameter( param );

    param = ShaderParamFactory::CreateGenericParameter( m_textureSize.get() );
    shaderParams->AddParameter( param );

    shaderParams->AddTexture( m_texture );

    //FIXME: add empty textures (nullptr) and create samplers. Textures can be set later on
    auto shader = new PixelShader( ReadFullscreenShader( "blur.frag" ), shaderParams );

    auto samplerTex = CreateDefaultSampler( "Tex0" );

    shader->AddTextureSampler( samplerTex );

    return shader;
}

// **************************
//
BlurFullscreenEffect::BlurFullscreenEffect  ( Texture2DPtr tex )
{
    m_blurSizeVal   = ValuesFactory::CreateValueFloat( "blurSize" );

    m_textureSize   = ValuesFactory::CreateValueVec2( "textureSize" );

    m_textureSize->SetValue( glm::vec2( float( tex->GetWidth() ), float( tex->GetHeight() ) ) );

    m_texture       = tex;
}

// **************************
//
BlurFullscreenEffect::~BlurFullscreenEffect ()
{}

// ****************************
//
RenderableEntity *  BlurFullscreenEffect::CreateFullscreenQuad  () const
{
    auto quad = CreateDefaultFullscrQuad( CreatePS() );

    auto eff  = quad->GetRenderableEffect();
    auto pass = eff->GetPass( 0 );

    auto sinst = pass->GetStateInstance();
    
    auto as = RenderStateAccessor::AccessAlphaState( sinst );

    as->blendEnabled = true;

    return quad;
}

} // bv
