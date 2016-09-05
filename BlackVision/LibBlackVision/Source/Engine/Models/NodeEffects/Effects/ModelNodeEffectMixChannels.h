#pragma once

#include "Engine/Models/NodeEffects/ModelNodeEffect.h"
#include "CoreDEF.h"


namespace bv { namespace model {

class ModelNodeEffectMixChannels : public ModelNodeEffect
{
public:
                    ModelNodeEffectMixChannels  ( NodeEffectType type );

    static UInt32   GetChannelMixMask           ( UInt32 rIdx, UInt32 gIdx, UInt32 bIdx, UInt32 aIdx );

};

} // model
} // bv
