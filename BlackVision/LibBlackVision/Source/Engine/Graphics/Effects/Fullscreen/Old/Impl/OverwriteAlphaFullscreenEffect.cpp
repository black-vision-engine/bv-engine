#include "OverwriteAlphaFullscreenEffect.h"

#include <cassert>

#include "Engine/Graphics/Shaders/Parameters/ShaderParamFactory.h"

#include "Engine/Types/Values/ValuesFactory.h"




#include "Memory/MemoryLeaks.h"



namespace bv {

// **************************
//
OverwriteAlphaFullscreenEffect::OverwriteAlphaFullscreenEffect              ( float alpha )
    : m_alpha( alpha )
{
    assert( alpha >= 0.0f && alpha <= 1.0f );

    m_alphaVal = ValuesFactory::CreateValueFloat( "alpha" );
}

// **************************
//
OverwriteAlphaFullscreenEffect::~OverwriteAlphaFullscreenEffect             ()
{
}

// **************************
//
void    OverwriteAlphaFullscreenEffect::SetAlpha                            ( float alpha )
{
    assert( alpha >= 0.0f && alpha <= 1.0f );

    m_alpha = alpha;

    m_alphaVal->SetValue( alpha );
}

// **************************
//
float   OverwriteAlphaFullscreenEffect::GetAlpha                            () const
{
    return m_alpha;
}

// **************************
//
RenderableEntity *      OverwriteAlphaFullscreenEffect::CreateFullscreenQuad    () const
{
    return CreateDefaultFullscrQuad( CreatePS() );
}

// **************************
//
PixelShader *           OverwriteAlphaFullscreenEffect::CreatePS                () const
{
    ShaderParameters * shaderParams = new ShaderParameters();

    GenericShaderParam * param = ShaderParamFactory::CreateGenericParameter( m_alphaVal.get() );

    shaderParams->AddParameter( param );

    //FIXME: add empty textures (nullptr) and create samplers. Textures can be set later on
    return new PixelShader( ReadFullscreenShader( "overwritealpha.frag" ), shaderParams );
}

} //bv
