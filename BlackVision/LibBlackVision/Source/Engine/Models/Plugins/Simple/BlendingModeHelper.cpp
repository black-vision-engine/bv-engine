#include "stdafx.h"
#include "BlendingModeHelper.h"

#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"


namespace bv {
namespace model {

const std::string        BlendingModeHelper::PARAM_BLEND_MODE     = "blend mode";


// ***********************
//
void         BlendingModeHelper::SetBlendRendererContext   ( DefaultPixelShaderChannelPtr psc, IParameterPtr param )
{
    assert( param != nullptr );
    assert( psc != nullptr );

    auto ctx = psc->GetRendererContext();
    auto blendModeParam = QueryTypedParam< std::shared_ptr< ParamEnum< BlendingModeHelper::BlendMode > > >( param );


    BlendingModeHelper::BlendMode blendMode = blendModeParam->Evaluate();
    if( blendMode == BlendingModeHelper::BlendMode::Normal )
    {
        // No blending
        ctx->alphaCtx->srcRGBBlendMode = model::AlphaContext::SrcBlendMode::SBM_ONE;
        ctx->alphaCtx->dstRGBBlendMode = model::AlphaContext::DstBlendMode::DBM_ZERO;
    }
    else if( blendMode == BlendingModeHelper::BlendMode::Multiply )
    {
        // Color = Srgb * Drgb
        ctx->alphaCtx->srcRGBBlendMode = model::AlphaContext::SrcBlendMode::SBM_DST_COLOR;
        ctx->alphaCtx->dstRGBBlendMode = model::AlphaContext::DstBlendMode::DBM_ZERO;
    }
    else if( blendMode == BlendingModeHelper::BlendMode::Average )
    {
        // Color = ( Srgb + Drgb ) / 2
        ctx->alphaCtx->srcRGBBlendMode = model::AlphaContext::SrcBlendMode::SBM_CONSTANT_COLOR;
        ctx->alphaCtx->dstRGBBlendMode = model::AlphaContext::DstBlendMode::DBM_CONSTANT_COLOR;
        ctx->alphaCtx->blendColor = glm::vec4( 0.5f, 0.5f, 0.5f, 1.0f );
    }
    else if( blendMode == BlendingModeHelper::BlendMode::Add )
    {
        // Color = clamp( Srgb + Drgb )
        // Gl clamps color automatically
        ctx->alphaCtx->srcRGBBlendMode = model::AlphaContext::SrcBlendMode::SBM_ONE;
        ctx->alphaCtx->dstRGBBlendMode = model::AlphaContext::DstBlendMode::DBM_ONE;
    }
    else    // BlendingModeHelper::BlendMode::Alpha
    {
        // Color = Srgb + Drgb * (1 - Sa)
        ctx->alphaCtx->srcRGBBlendMode = model::AlphaContext::SrcBlendMode::SBM_ONE;
        ctx->alphaCtx->dstRGBBlendMode = model::AlphaContext::DstBlendMode::DBM_ONE_MINUS_SRC_ALPHA;

        // Version without premultiplied alpha
        //ctx->alphaCtx->srcRGBBlendMode = model::AlphaContext::SrcBlendMode::SBM_SRC_ALPHA;
        //ctx->alphaCtx->dstRGBBlendMode = model::AlphaContext::DstBlendMode::DBM_ONE_MINUS_SRC_ALPHA;
    }
}


}	// model

}	// bv