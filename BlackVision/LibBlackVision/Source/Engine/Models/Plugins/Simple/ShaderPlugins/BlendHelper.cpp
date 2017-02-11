#include "stdafx.h"
#include "BlendHelper.h"

#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"


namespace bv {
namespace model {


const std::string        BlendHelper::PARAM::BLEND_MODE		= "background blend mode";
const std::string        BlendHelper::PARAM::BLEND_ENABLE	= "blend enable";



// ================================ //
//
void		BlendHelper::SetBlendRendererContext	( DefaultPixelShaderChannelPtr psc, const SimpleParameterImpl< IntInterpolator, int, ModelParamType::MPT_ENUM > & param )
{
	SetBlendRendererContext( psc, static_cast< const ParamEnum< BlendHelper::BlendMode > *>( &param ) );
}

// ================================ //
//
void		BlendHelper::SetBlendRendererContext	( DefaultPixelShaderChannelPtr psc, const ParamEnum< BlendHelper::BlendMode > * param )
{
    assert( &param != nullptr );
    assert( psc != nullptr );

    auto ctx = psc->GetRendererContext();

	ctx->alphaCtx->srcAlphaBlendMode = model::AlphaContext::SrcBlendMode::SBM_ONE;
	ctx->alphaCtx->dstAlphaBlendMode = model::AlphaContext::DstBlendMode::DBM_ZERO;

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
			ctx->alphaCtx->blendColor = glm::vec4( 0.5f, 0.5f, 0.5f, 1.0f );
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


}	// model

}	// bv