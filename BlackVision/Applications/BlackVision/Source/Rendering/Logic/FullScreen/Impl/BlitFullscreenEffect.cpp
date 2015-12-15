#include "BlitFullscreenEffect.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParamFactory.h"

#include "Engine/Types/Values/ValuesFactory.h"


namespace bv {

// ****************************
//
BlitFullscreenEffect::BlitFullscreenEffect    ( bool useAlpha )
    : m_useAlpha( useAlpha )
{
    m_alphaVal = ValuesFactory::CreateValueFloat( "alpha" );

    m_alpha = 1.0f;

    m_alphaVal->SetValue( 1.f );
}

// ****************************
//
BlitFullscreenEffect::~BlitFullscreenEffect   ()
{
}

// ****************************
//
RenderableEntity *  BlitFullscreenEffect::CreateFullscreenQuad    () const
{
    return nullptr;
}

// ****************************
//
PixelShader *       BlitFullscreenEffect::CreatePS                () const
{
    ShaderParameters * shaderParams = new ShaderParameters();

    GenericShaderParam * param = ShaderParamFactory::CreateGenericParameter( m_alphaVal.get() );

    shaderParams->AddParameter( param );

    //FIXME: add empty textures (nullptr) and create samplers. Textures can be set later on
    return new PixelShader( ReadFullscreenShader( "blit_no_alpha.frag" ), shaderParams );
}

} // bv
