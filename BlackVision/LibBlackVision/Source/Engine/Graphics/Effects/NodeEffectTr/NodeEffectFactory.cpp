#include "NodeEffectFactory.h"

#include <cassert>

#include "Engine/Graphics/Effects/NodeEffectTr/NodeEffectTr.h"
#include "Engine/Graphics/Effects/NodeEffectTr/NodeEffectLogic.h"

//PRE
#include "Engine/Graphics/Effects/NodeEffectTr/Impl/DefaultPreFullscreenEffectLogic.h"
#include "Engine/Graphics/Effects/NodeEffectTr/Impl/AlphaMaskPreFullscreenEffectLogic.h"
#include "Engine/Graphics/Effects/NodeEffectTr/Impl/NodeMaskPreFullscreenEffectLogic.h"
#include "Engine/Graphics/Effects/NodeEffectTr/Impl/InterlacePreFullscreenEffectLogic.h"
#include "Engine/Graphics/Effects/NodeEffectTr/Impl/RenderOffscreenPreFullscreenEffectLogic.h"

//FSE
#include "Engine/Graphics/Effects/Fullscreen/FullscreenEffectFactory.h"

//POST
#include "Engine/Graphics/Effects/NodeEffectTr/Impl/DefaultPostFullscreenEffectLogic.h"
#include "Engine/Graphics/Effects/NodeEffectTr/Impl/WireframePostFullscreenEffectLogic.h"


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
NodeEffectLogic *  CreateNodeEffectLogic()
{
    return new NodeEffectLogic();
}

// **************************
//
void    SetLogicComponents( NodeEffectLogic * logic, PreFullscreenEffectLogic * pre, FullscreenEffectTr * fse, PostFullscreenEffectLogic * post )
{
    assert( logic );
    assert( pre || fse || post );

    if( pre )
    {
        logic->SetPreFullscreenEffectLogic( pre );
    }

    if( fse )
    {
        logic->SetFullscreenEffect( fse );
    }

    if( post )
    {
        logic->SetPostFullscreenEffectLogic( post );
    }
}

// **************************
//
NodeEffectTr *  CreateDefaultNodeEffect()
{
    auto logic = CreateNodeEffectLogic();
    
    SetLogicComponents( logic, new DefaultPreFullscreenEffectLogic(), nullptr, nullptr );

    return CreateNodeEffect( logic );
}

// **************************
//
NodeEffectTr *  CreateAlphaMaskNodeEffect()
{
    auto logic = CreateNodeEffectLogic();

    auto pre  = new AlphaMaskPreFullscreenEffectLogic( 0.01f, 0.99f );
    auto fse  = CreateFullscreenEffect( FullscreenEffectType::FET_BLIT_WITH_ALPHA );

    SetLogicComponents( logic, pre, fse, nullptr );

    return CreateNodeEffect( logic );
}

// **************************
//
NodeEffectTr *  CreateNodeMaskNodeEffect()
{
    auto logic = CreateNodeEffectLogic();

    auto pre  = new NodeMaskPreFullscreenEffectLogic( 0.01f );
    auto fse  = CreateFullscreenEffect( FullscreenEffectType::FET_BLIT_WITH_ALPHA_MASK );
    auto post = new DefaultPostFullscreenEffectLogic( 2 );

    SetLogicComponents( logic, pre, fse, post );

    return CreateNodeEffect( logic );
}

// **************************
//
NodeEffectTr *  CreateWireframeNodeEffect()
{
    auto logic = CreateNodeEffectLogic();

    auto pre  = new DefaultPreFullscreenEffectLogic();
    auto post = new WireframePostFullscreenEffectLogic();

    SetLogicComponents( logic, pre, nullptr, post );

    return CreateNodeEffect( logic );
}

// **************************
//
NodeEffectTr *  CreateMixchannelsNodeEffect()
{
    auto logic = CreateNodeEffectLogic();

    auto pre  = new RenderOffscreenPreFullscreenEffectLogic();
    auto post = new WireframePostFullscreenEffectLogic();

    SetLogicComponents( logic, pre, nullptr, post );

    return CreateNodeEffect( logic );
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
