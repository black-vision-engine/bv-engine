#include "AlphaContext.h"


namespace bv { namespace model {

// ******************************
//
AlphaContext::AlphaContext()
{
    blendEnabled    = false;
    blendColor      = glm::vec4( 0.f, 0.f, 0.f, 0.f );
    srcBlendMode    = SrcBlendMode::SBM_SRC_ALPHA;
    dstBlendMode    = DstBlendMode::DBM_ONE_MINUS_SRC_ALPHA;
}

// ******************************
//
AlphaContext *		AlphaContext::Clone		() const
{
	auto ctx = new AlphaContext();
    ctx->blendEnabled = blendEnabled;
    ctx->blendColor = blendColor;
    ctx->srcBlendMode = srcBlendMode;
    ctx->dstBlendMode = dstBlendMode;
	return ctx;
}

} //model
} //bv
