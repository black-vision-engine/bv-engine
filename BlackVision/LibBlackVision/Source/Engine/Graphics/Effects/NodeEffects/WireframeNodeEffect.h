#pragma once

#include "Engine/Graphics/Effects/NodeEffects/NodeEffect.h"


namespace bv {

class WireframeNodeEffect : public NodeEffect
{
public:

    WireframeNodeEffect     ();
    ~WireframeNodeEffect    ();
};

DEFINE_PTR_TYPE(WireframeNodeEffect)
DEFINE_CONST_PTR_TYPE(WireframeNodeEffect)

} //bv
