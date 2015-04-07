#pragma once

#include "CoreDEF.h"

#include "Engine/Graphics/State/RendererStateEnums.h"


namespace bv {

class StencilState
{
public:

    bool                    enabled;        // false
    StencilCompareMode      compareMode;    // SCM_NEVER
    unsigned int            reference;      // 0
    unsigned int            andMask;        // 0xFFFFFFFF
    unsigned int            writeMask;      // 0xFFFFFFFF

    StencilOperationType    actionFail;     // SOT_KEEP
    StencilOperationType    actionZFail;    // SOT_KEEP
    StencilOperationType    actionZPass;    // SOT_KEEP

public:

    StencilState  ();

};

DEFINE_PTR_TYPE(StencilState)
DEFINE_CONST_PTR_TYPE(StencilState)

} //bv
