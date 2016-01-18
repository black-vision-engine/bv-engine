#include "BlitAlphaMaskFullscreenEffect.h"

#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParamFactory.h"

#include "Engine/Types/Values/ValuesFactory.h"


namespace bv {

// ****************************
//
BlitAlphaMaskFullscreenEffect::BlitAlphaMaskFullscreenEffect            ( Texture2DPtr tex, Texture2DPtr alpha )
{
    m_alphaVal = ValuesFactory::CreateValueFloat( "alpha" );
    m_alphaVal->SetValue( 1.f );

    auto quad = CreateDefaultFullscrQuad( CreatePS( tex, alpha ) );
    SetFullscreenQuad( quad );

    auto eff  = quad->GetRenderableEffect();
    auto pass = eff->GetPass( 0 );

    auto sinst = pass->GetStateInstance();

    auto as = RenderStateAccessor::AccessAlphaState( sinst );

    as->blendEnabled = true;
}

// ****************************
//
BlitAlphaMaskFullscreenEffect::~BlitAlphaMaskFullscreenEffect           ()
{
}

// ****************************
//
void                BlitAlphaMaskFullscreenEffect::SetAlpha             ( float alpha )
{
    assert( alpha >= 0.0f && alpha <= 1.0f );

    m_alphaVal->SetValue( alpha );
}

// ****************************
//
RenderableEntity *  BlitAlphaMaskFullscreenEffect::CreateFullscreenQuad () const
{
    assert( false );

    return nullptr;
}

// ****************************
//
PixelShader *       BlitAlphaMaskFullscreenEffect::CreatePS             ( Texture2DPtr tex, Texture2DPtr alpha ) const
{
    // FIXME: at some point allow usages with no alpha and implemented soubroutines
    bool useAlpha = true;

    PixelShader * shader = nullptr;

    ShaderParameters * shaderParams = new ShaderParameters();
    shaderParams->AddTexture( tex );
    shaderParams->AddTexture( alpha );

    auto samplerTex     = CreateDefaultSampler( "Texture" );
    auto samplerAlpha   = CreateDefaultSampler( "Mask" );

    if( useAlpha )
    {
        GenericShaderParam * param = ShaderParamFactory::CreateGenericParameter( m_alphaVal.get() );
        
        shaderParams->AddParameter( param );

        shader = new PixelShader( ReadFullscreenShader( "blit_mask_alpha.frag" ), shaderParams );
    }
    else
    {
        shader = new PixelShader( ReadFullscreenShader( "blit_mask_no_alpha.frag" ), shaderParams );
    }

    shader->AddTextureSampler( samplerTex );
    shader->AddTextureSampler( samplerAlpha );

    return shader;
}

} // bv
