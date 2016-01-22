#include "NodeEffectFactory.h"

#include <cassert>


namespace bv {

namespace {

// **************************
//
NodeEffectTr *  CreateDefaultNodeEffect()
{
    return nullptr;
}

// **************************
//
NodeEffectTr *  CreateAlphaMaskNodeEffect()
{
    return nullptr;
}

// **************************
//
NodeEffectTr *  CreateNodeMaskNodeEffect()
{
    return nullptr;
}

// **************************
//
NodeEffectTr *  CreateWireframeNodeEffect()
{
    return nullptr;
}

// **************************
//
NodeEffectTr *  CreateMixchannelsNodeEffect()
{
    return nullptr;
}
 
} // anonoymnous


// **************************
//
NodeEffectTr *    CreateNodeEffect( NodeEffectType nodeEffectType )
{
    switch( nodeEffectType )
    {
        case  NodeEffectType::NET_DEFAULT:
            return CreateDefaultNodeEffect();
        case NodeEffectType::NET_ALPHA_MASK:
            return CreateAlphaMaskNodeEffect();
        case NodeEffectType::NET_NODE_MASK:
            return CreateNodeMaskNodeEffect();
        case NodeEffectType::NET_WIREFRAME:
            return CreateWireframeNodeEffect();
        case NodeEffectType::NET_MIX_CHANNELS:
            return CreateMixchannelsNodeEffect();
        default:
            assert( false );
    }

    return nullptr;
}

} //bv
