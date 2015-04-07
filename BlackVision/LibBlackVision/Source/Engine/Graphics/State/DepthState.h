#pragma once

#include "CoreDEF.h"

#include "Engine/Graphics/State/RendererStateEnums.h"


namespace bv {

class DepthState
{
public:

    bool                enabled;        // true
    bool                writable;       // true
    DepthCompareMode    compareMode;    // DCM_LEQUAL

public:

    DepthState  ();

};

DEFINE_PTR_TYPE(DepthState)
DEFINE_CONST_PTR_TYPE(DepthState)

} //bv
