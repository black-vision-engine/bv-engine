#include "ModelNodeEffectMixChannels.h"


namespace bv { namespace model {

// ********************************
//
ModelNodeEffectMixChannels::ModelNodeEffectMixChannels  ( NodeEffectType type )
    : ModelNodeEffect( type )
{
}

// **************************
//
UInt32    ModelNodeEffectMixChannels:: GetChannelMixMask ( UInt32 rIdx, UInt32 gIdx, UInt32 bIdx, UInt32 aIdx )
{
    return ( aIdx & 0x3 ) << 6 | ( bIdx & 0x3 ) << 4 | ( gIdx & 0x3 ) << 2 | ( rIdx & 0x3 ) << 0;
}

} // model
} // bv
