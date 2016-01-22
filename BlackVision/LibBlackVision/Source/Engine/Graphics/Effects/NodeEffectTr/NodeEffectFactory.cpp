#include "NodeEffectFactory.h"

#include <cassert>

#include "Engine/Graphics/Effects/NodeEffectTr/NodeEffectTr.h"
#include "Engine/Graphics/Effects/NodeEffectTr/NodeEffectLogic.h"

#include "Engine/Graphics/Effects/NodeEffectTr/Impl/DefaultPreFullscreenEffectLogic.h"


namespace bv {

namespace {

// **************************
//
NodeEffectTr *  CreateNodeEffect( NodeEffectLogic * logic )
{
    return new NodeEffectTr( logic );
}

// **************************
//
NodeEffectTr *  CreateDefaultNodeEffect()
{
    NodeEffectLogic * logic = new NodeEffectLogic();
    
    logic->SetPreFullscreenEffectLogic( new DefaultPreFullscreenEffectLogic() );

    return CreateNodeEffect( logic );
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
