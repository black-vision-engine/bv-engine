#include "stdafx.h"
#include "BlendHelper.h"

#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"


namespace bv {
namespace model {


const std::string        BlendHelper::PARAM::BLEND_MODE		    = "background blend mode";

const std::string        BlendHelper::PARAM::COLOR_BLEND_MODE   = "color blend mode";
const std::string        BlendHelper::PARAM::ALPHA_BLEND_MODE   = "alpha blend mode";
const std::string        BlendHelper::PARAM::BLEND_ENABLE	    = "blend enable";



// ================================ //
//
void		BlendHelper::SetBlendRendererContext	( DefaultPixelShaderChannelPtr psc, const SimpleParameterImpl< IntInterpolator, int, ModelParamType::MPT_ENUM > & param )
{
	SetBlendRendererContext( psc, static_cast< const ParamEnum< BlendHelper::BlendMode > *>( &param ) );
}

// ***********************
//
void        BlendHelper::SetBlendRendererContext    ( DefaultPixelShaderChannelPtr psc, const SimpleParameterImpl<IntInterpolator, int, ModelParamType::MPT_ENUM>& colorParam, const SimpleParameterImpl<IntInterpolator, int, ModelParamType::MPT_ENUM>& alphaParam )
{
    SetBlendRendererContext( psc, static_cast< const ParamEnum< BlendHelper::BlendMode > *>( &colorParam ), static_cast< const ParamEnum< BlendHelper::BlendMode > *>( &alphaParam ) );
}

// ================================ //
//
void		BlendHelper::SetBlendRendererContext	( DefaultPixelShaderChannelPtr psc, const ParamEnum< BlendHelper::BlendMode > * param )
{
    assert( &param != nullptr );
    assert( psc != nullptr );

    auto ctx = psc->GetRendererContext();

	ctx->alphaCtx->srcAlphaBlendMode = model::AlphaContext::SrcBlendMode::SBM_ONE;
	ctx->alphaCtx->dstAlphaBlendMode = model::AlphaContext::DstBlendMode::DBM_ONE;

    SetBlendColorContext( ctx, param );
}

// ***********************
//
void        BlendHelper::SetBlendRendererContext    (   DefaultPixelShaderChannelPtr psc,
                                                        const ParamEnum< BlendHelper::BlendMode > * colorBlendParam,
                                                        const ParamEnum< BlendHelper::BlendMode > * alphaBlendParam )
{
    assert( &colorBlendParam != nullptr );
    assert( &alphaBlendParam != nullptr );
    assert( psc != nullptr );

    auto ctx = psc->GetRendererContext();

    ctx->alphaCtx->srcAlphaBlendMode = model::AlphaContext::SrcBlendMode::SBM_ONE;
    ctx->alphaCtx->dstAlphaBlendMode = model::AlphaContext::DstBlendMode::DBM_ONE;

    SetBlendColorContext( ctx, colorBlendParam );
    SetBlendAlphaContext( ctx, alphaBlendParam );
}

// ***********************
//
void        BlendHelper::SetBlendColorContext       ( RendererContextPtr & ctx, const ParamEnum< BlendHelper::BlendMode > * param )
{
    BlendHelper::BlendMode blendMode = param->Evaluate();
    switch( blendMode )
    {
    case bv::model::BlendHelper::BM_Multiply:
        // Color = Srgb * Drgb
        ctx->alphaCtx->srcRGBBlendMode = model::AlphaContext::SrcBlendMode::SBM_DST_COLOR;
        ctx->alphaCtx->dstRGBBlendMode = model::AlphaContext::DstBlendMode::DBM_ZERO;
        break;
    case bv::model::BlendHelper::BM_Average:
        // Color = ( Srgb + Drgb ) / 2
        ctx->alphaCtx->srcRGBBlendMode = model::AlphaContext::SrcBlendMode::SBM_CONSTANT_COLOR;
        ctx->alphaCtx->dstRGBBlendMode = model::AlphaContext::DstBlendMode::DBM_CONSTANT_COLOR;
        ctx->alphaCtx->blendColor.r = 0.5f;
        ctx->alphaCtx->blendColor.g = 0.5f;
        ctx->alphaCtx->blendColor.b = 0.5f;
        break;
    case bv::model::BlendHelper::BM_Add:
        // Color = clamp( Srgb + Drgb )
        // Gl clamps color automatically
        ctx->alphaCtx->srcRGBBlendMode = model::AlphaContext::SrcBlendMode::SBM_ONE;
        ctx->alphaCtx->dstRGBBlendMode = model::AlphaContext::DstBlendMode::DBM_ONE;
        break;
    case bv::model::BlendHelper::BM_None:
        // No blending
        ctx->alphaCtx->srcRGBBlendMode = model::AlphaContext::SrcBlendMode::SBM_ONE;
        ctx->alphaCtx->dstRGBBlendMode = model::AlphaContext::DstBlendMode::DBM_ZERO;
        break;
    case bv::model::BlendHelper::BM_Normal:
        // Color = Srgb + Drgb * (1 - Sa)
        ctx->alphaCtx->srcRGBBlendMode = model::AlphaContext::SrcBlendMode::SBM_ONE;
        ctx->alphaCtx->dstRGBBlendMode = model::AlphaContext::DstBlendMode::DBM_ONE_MINUS_SRC_ALPHA;
        break;
    case bv::model::BlendHelper::BM_Alpha:
    default:
        // BM_Alpha
        // For all not premultiplied textures
        ctx->alphaCtx->srcRGBBlendMode = model::AlphaContext::SrcBlendMode::SBM_SRC_ALPHA;
        ctx->alphaCtx->dstRGBBlendMode = model::AlphaContext::DstBlendMode::DBM_ONE_MINUS_SRC_ALPHA;
        break;
    }
}

void        BlendHelper::SetBlendAlphaContext       ( RendererContextPtr & ctx, const ParamEnum< BlendHelper::BlendMode > * param )
{
    BlendHelper::BlendMode blendMode = param->Evaluate();
    switch( blendMode )
    {
    case bv::model::BlendHelper::BM_Multiply:
        // Color = Srgb * Drgb
        ctx->alphaCtx->srcAlphaBlendMode = model::AlphaContext::SrcBlendMode::SBM_DST_ALPHA;
        ctx->alphaCtx->dstAlphaBlendMode = model::AlphaContext::DstBlendMode::DBM_ZERO;
        break;
    case bv::model::BlendHelper::BM_Average:
        // Color = ( Srgb + Drgb ) / 2
        ctx->alphaCtx->srcAlphaBlendMode = model::AlphaContext::SrcBlendMode::SBM_CONSTANT_ALPHA;
        ctx->alphaCtx->dstAlphaBlendMode = model::AlphaContext::DstBlendMode::DBM_CONSTANT_ALPHA;
        ctx->alphaCtx->blendColor.a = 0.5f;
        break;
    case bv::model::BlendHelper::BM_Add:
        // Color = clamp( Srgb + Drgb )
        // Gl clamps color automatically
        ctx->alphaCtx->srcAlphaBlendMode = model::AlphaContext::SrcBlendMode::SBM_ONE;
        ctx->alphaCtx->dstAlphaBlendMode = model::AlphaContext::DstBlendMode::DBM_ONE;
        break;
    case bv::model::BlendHelper::BM_None:
        // No blending
        ctx->alphaCtx->srcAlphaBlendMode = model::AlphaContext::SrcBlendMode::SBM_ONE;
        ctx->alphaCtx->dstAlphaBlendMode = model::AlphaContext::DstBlendMode::DBM_ZERO;
        break;
    case bv::model::BlendHelper::BM_Normal:
        // Color = Srgb + Drgb * (1 - Sa)
        ctx->alphaCtx->srcAlphaBlendMode = model::AlphaContext::SrcBlendMode::SBM_ONE;
        ctx->alphaCtx->dstAlphaBlendMode = model::AlphaContext::DstBlendMode::DBM_ONE_MINUS_SRC_ALPHA;
        break;
    case bv::model::BlendHelper::BM_Alpha:
    default:
        // BM_Alpha
        // For all not premultiplied textures
        ctx->alphaCtx->srcAlphaBlendMode = model::AlphaContext::SrcBlendMode::SBM_SRC_ALPHA;
        ctx->alphaCtx->dstAlphaBlendMode = model::AlphaContext::DstBlendMode::DBM_ONE_MINUS_SRC_ALPHA;
        break;
    }
}

// ================================ //
//
void		BlendHelper::UpdateBlendState			( DefaultPixelShaderChannelPtr psc,
													  ValueParamState< bool > & blenEnable,
													  ValueParamState< BlendHelper::BlendMode > & blendMode )
{
	bool contextUpdateNeeded = false;

    if( blenEnable.Changed() )
    {
        auto ctx = psc->GetRendererContext();
        ctx->alphaCtx->blendEnabled = blenEnable.GetParameter().Evaluate();

        contextUpdateNeeded = true;
    }

    if( blendMode.Changed() )
    {
        BlendHelper::SetBlendRendererContext( psc, blendMode.GetParameter() );
        contextUpdateNeeded = true;
    }

	if( contextUpdateNeeded )
	{
		HelperPixelShaderChannel::SetRendererContextUpdate( psc );
	}
}

// ***********************
//
void        BlendHelper::UpdateBlendState           (   DefaultPixelShaderChannelPtr psc,
                                                        ValueParamState< bool > & blenEnable,
                                                        ValueParamState< BlendHelper::BlendMode > & /*colorBlendMode*/,
                                                        ValueParamState< BlendHelper::BlendMode > & /*alphaBlendMode*/ )
{
    bool contextUpdateNeeded = false;
    auto ctx = psc->GetRendererContext();

    if( blenEnable.Changed() )
    {
        ctx->alphaCtx->blendEnabled = blenEnable.GetParameter().Evaluate();

        contextUpdateNeeded = true;
    }
    assert( false );

    //if( colorBlendMode.Changed() )
    //{
    //    BlendHelper::SetBlendColorContext( ctx, &colorBlendMode.GetParameter() );
    //    contextUpdateNeeded = true;
    //}

    //if( alphaBlendMode.Changed() )
    //{
    //    BlendHelper::SetBlendAlphaContext( ctx, &colorBlendMode.GetParameter() );
    //    contextUpdateNeeded = true;
    //}


    if( contextUpdateNeeded )
    {
        HelperPixelShaderChannel::SetRendererContextUpdate( psc );
    }
}


}	// model

}	// bv