#pragma once

#include "Engine/Graphics/Effects/NodeEffects/NodeEffect.h"


namespace bv {

class MixChannelsNodeEffect : public NodeEffect
{
public:

        MixChannelsNodeEffect   ();
        ~MixChannelsNodeEffect  ();

};

DEFINE_PTR_TYPE(MixChannelsNodeEffect)
DEFINE_CONST_PTR_TYPE(MixChannelsNodeEffect)

} //bv
