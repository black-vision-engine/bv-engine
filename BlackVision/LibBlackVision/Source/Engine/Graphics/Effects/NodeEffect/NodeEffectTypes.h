#pragma once


namespace bv {

enum class NodeEffectType : int
{
    NET_DEFAULT = 0,
    NET_ALPHA_MASK,
    NET_NODE_MASK,
    NET_WIREFRAME,
    NET_MIX_CHANNELS,

    NET_TOTAL
};

} //bv
