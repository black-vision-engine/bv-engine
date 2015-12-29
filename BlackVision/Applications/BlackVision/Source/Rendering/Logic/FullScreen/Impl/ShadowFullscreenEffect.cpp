#include "ShadowFullscreenEffect.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParamFactory.h"

#include "Engine/Types/Values/ValuesFactory.h"

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
void            ShadowFullscreenEffect::SetBlurSize   ( float s )
{
    m_blurSize = s;
    m_blurSizeVal->SetValue( s );
}

// **************************
//
float           ShadowFullscreenEffect::GetBlurSize   () const
{
    return m_blurSize;
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

    param = ShaderParamFactory::CreateGenericParameter( m_blurSizeVal.get() );
    shaderParams->AddParameter( param );

    //FIXME: add empty textures (nullptr) and create samplers. Textures can be set later on
    return new PixelShader( ReadFullscreenShader( "shadow.frag" ), shaderParams );
}

// **************************
//
ShadowFullscreenEffect::ShadowFullscreenEffect      ( Texture2DPtr tex )
{
    m_colorVal      = ValuesFactory::CreateValueVec4( "color" );
    m_shiftVal      = ValuesFactory::CreateValueVec2( "shift" );
    m_blurSizeVal   = ValuesFactory::CreateValueFloat( "blurSize" );

    m_texture       = tex;
}

// **************************
//
ShadowFullscreenEffect::~ShadowFullscreenEffect     ()
{}

// ****************************
//
RenderableEntity *  ShadowFullscreenEffect::CreateFullscreenQuad    () const
{
    assert( false );
    return nullptr;
}

} // bv
