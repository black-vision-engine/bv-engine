#pragma once

#include "Engine/Graphics/Effects/NodeEffects/NodeEffect.h"


namespace bv {

class NodeMaskNodeEffect : public NodeEffect
{
public:

            NodeMaskNodeEffect  ();
            ~NodeMaskNodeEffect ();
};

DEFINE_PTR_TYPE(NodeMaskNodeEffect)
DEFINE_CONST_PTR_TYPE(NodeMaskNodeEffect)

} //bv
