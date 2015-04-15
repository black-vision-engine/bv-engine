#pragma once

#include "CoreDEF.h"

#include "Engine/Graphics/State/RendererStateEnums.h"


namespace bv {

class CullState
{
public:

    bool enabled;       // true
    bool isCCWOrdered;  // true

public:

    CullState  ();

};

DEFINE_PTR_TYPE(CullState)
DEFINE_CONST_PTR_TYPE(CullState)

} //bv
