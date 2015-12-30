#pragma once

#include "Engine/Graphics/Effects/NodeEffects/NodeEffect.h"


namespace bv {

class AlphaMaskNodeEffect : public NodeEffect
{
public:

    AlphaMaskNodeEffect     ();
    ~AlphaMaskNodeEffect    ();
};

DEFINE_PTR_TYPE(AlphaMaskNodeEffect)
DEFINE_CONST_PTR_TYPE(AlphaMaskNodeEffect)

} //bv
