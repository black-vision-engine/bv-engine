#pragma once

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

} //bv
