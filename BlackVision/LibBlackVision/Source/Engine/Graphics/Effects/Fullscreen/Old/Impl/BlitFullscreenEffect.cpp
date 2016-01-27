#include "BlitFullscreenEffect.h"

#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParamFactory.h"

#include "Engine/Types/Values/ValuesFactory.h"


namespace bv {

// ****************************
//
BlitFullscreenEffect::BlitFullscreenEffect      ( Texture2DPtr tex, bool useAlpha )
{
    m_alphaVal = ValuesFactory::CreateValueFloat( "alpha" );

    SetAlpha( 1.f );

    auto quad = CreateDefaultFullscrQuad( CreatePS( tex, useAlpha ) );
    SetFullscreenQuad( quad );

    if ( useAlpha )
    {
        auto eff  = quad->GetRenderableEffect();
        auto pass = eff->GetPass( 0 );

        auto sinst = pass->GetStateInstance();

        auto as = RenderStateAccessor::AccessAlphaState( sinst );

        as->blendEnabled = true;
    }
}

// ****************************
//
BlitFullscreenEffect::~BlitFullscreenEffect     ()
{
}

// ****************************
//
float   BlitFullscreenEffect::GetAlpha          () const
{
    return m_alpha;
}

// ****************************
//
void    BlitFullscreenEffect::SetAlpha          ( float alpha )
{
    assert( alpha >= 0.f && alpha <= 1.f );

    m_alpha = alpha;

    m_alphaVal->SetValue( alpha );
}

// ****************************
//
RenderableEntity *  BlitFullscreenEffect::CreateFullscreenQuad    () const
{
    assert( false );
/*
    auto eff  = quad->GetRenderableEffect();
    auto pass = eff->GetPass( 0 );

    auto sinst = pass->GetStateInstance();

    auto as = RenderStateAccessor::AccessAlphaState( sinst );
    auto ds = RenderStateAccessor::AccessDepthState( sinst );
    auto cs = RenderStateAccessor::AccessCullState( sinst );

    as->blendEnabled = false;
    ds->enabled = false;
    cs->enabled = false;

    return quad;
*/
    return nullptr;
}

// ****************************
//
PixelShader *       BlitFullscreenEffect::CreatePS                ( Texture2DPtr tex, bool useAlpha ) const
{
    PixelShader * shader = nullptr;

    ShaderParameters * shaderParams = new ShaderParameters();
    shaderParams->AddTexture( tex );

    auto sampler = CreateDefaultSampler( "Texture" );

    if( useAlpha )
    {
        GenericShaderParam * param = ShaderParamFactory::CreateGenericParameter( m_alphaVal.get() );

        shaderParams->AddParameter( param );

        shader = new PixelShader( ReadFullscreenShader( "blit_alpha.frag" ), shaderParams );
    }
    else
    {
        shader = new PixelShader( ReadFullscreenShader( "blit_no_alpha.frag" ), shaderParams );
    }

    shader->AddTextureSampler( sampler );

    return shader;
}

} // bv
