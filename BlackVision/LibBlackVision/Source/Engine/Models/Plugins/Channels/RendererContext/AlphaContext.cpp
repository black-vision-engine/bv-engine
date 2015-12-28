#include "AlphaContext.h"


namespace bv { namespace model {

// ******************************
//
AlphaContext::AlphaContext()
{
    blendEnabled    = false;
    blendColor      = glm::vec4( 0.f, 0.f, 0.f, 0.f );
    srcRGBBlendMode	= SrcBlendMode::SBM_SRC_ALPHA;
    dstRGBBlendMode	= DstBlendMode::DBM_ONE_MINUS_SRC_ALPHA;
	srcAlphaBlendMode = SrcBlendMode::SBM_ONE;
    dstAlphaBlendMode = DstBlendMode::DBM_ONE;
}

// ******************************
//
AlphaContext *		AlphaContext::Clone		() const
{
	auto ctx = new AlphaContext();
    ctx->blendEnabled = blendEnabled;
    ctx->blendColor = blendColor;
    ctx->srcRGBBlendMode = srcRGBBlendMode;
    ctx->dstRGBBlendMode = dstRGBBlendMode;
    ctx->srcAlphaBlendMode = srcAlphaBlendMode;
    ctx->dstAlphaBlendMode = dstAlphaBlendMode;
	return ctx;
}

} //model
} //bv
