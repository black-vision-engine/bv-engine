#include "ShadowFullscreenEffect.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParamFactory.h"

#include "Engine/Types/Values/ValuesFactory.h"

#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

namespace bv {

// **************************
//
void            ShadowFullscreenEffect::SetColor      ( const glm::vec4 & color )
{
    m_color = color;
    m_colorVal->SetValue( color );
}

// **************************
//
glm::vec4       ShadowFullscreenEffect::GetColor      () const
{
    return m_color;
}

// **************************
//
void            ShadowFullscreenEffect::SetShift      ( const glm::vec2 & xy )
{
    m_shift = xy;
    m_shiftVal->SetValue( xy );
}

// **************************
//
glm::vec2       ShadowFullscreenEffect::GetShift      () const
{
    return m_shift;
}

// **************************
//
PixelShader *   ShadowFullscreenEffect::CreatePS            () const
{
    ShaderParameters * shaderParams = new ShaderParameters();

    GenericShaderParam * param = ShaderParamFactory::CreateGenericParameter( m_colorVal.get() );
    shaderParams->AddParameter( param );

    param = ShaderParamFactory::CreateGenericParameter( m_shiftVal.get() );
    shaderParams->AddParameter( param );

    param = ShaderParamFactory::CreateGenericParameter( m_textureSize.get() );
    shaderParams->AddParameter( param );

    shaderParams->AddTexture( m_texture );
    shaderParams->AddTexture( m_bluredTexture );

    //FIXME: add empty textures (nullptr) and create samplers. Textures can be set later on
    auto shader = new PixelShader( ReadFullscreenShader( "shadow.frag" ), shaderParams );

    auto samplerTex = CreateDefaultSampler( "Tex0" );
    auto samplerBluredTex = CreateDefaultSampler( "BluredTex0" );

    shader->AddTextureSampler( samplerTex );
    shader->AddTextureSampler( samplerBluredTex );

    return shader;
}

// **************************
//
ShadowFullscreenEffect::ShadowFullscreenEffect      ( Texture2DPtr tex, Texture2DPtr bluredTexture )
{
    m_colorVal      = ValuesFactory::CreateValueVec4( "color" );
    m_shiftVal      = ValuesFactory::CreateValueVec2( "shift" );

    m_textureSize   = ValuesFactory::CreateValueVec2( "textureSize" );

    m_textureSize->SetValue( glm::vec2( float( tex->GetWidth() ), float( tex->GetHeight() ) ) );

    m_texture       = tex;
    m_bluredTexture = bluredTexture;
}

// **************************
//
ShadowFullscreenEffect::~ShadowFullscreenEffect     ()
{}

// ****************************
//
RenderableEntity *  ShadowFullscreenEffect::CreateFullscreenQuad    () const
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
