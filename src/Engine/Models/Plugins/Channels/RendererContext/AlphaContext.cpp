#include "AlphaContext.h"


namespace bv { namespace model {

// ******************************
//
AlphaContext::AlphaContext()
{
    blendEnabled    = false;
    testEnabled     = false;
    referenceAlpha  = 0.f;
    blendColor      = glm::vec4(0.f, 0.f, 0.f, 0.f);
}

} //model
} //bv
