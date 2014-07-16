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

} //model
} //bv
