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
NodeEffectTrPtr  CreateNodeEffect( NodeEffectLogic * logic, NodeEffectType neType )
{
    return std::make_shared< NodeEffectTr >( logic, neType );
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
NodeEffectTrPtr  CreateDefaultNodeEffect()
{
    auto logic = CreateNodeEffectLogic();
    
    SetLogicComponents( logic, new DefaultPreFullscreenEffectLogic(), nullptr, nullptr );

    return CreateNodeEffect( logic, NodeEffectType::NET_DEFAULT );
}

// **************************
//
NodeEffectTrPtr  CreateAlphaMaskNodeEffect()
{
    auto logic = CreateNodeEffectLogic();

    auto pre  = new AlphaMaskPreFullscreenEffectLogic( 0.01f, 0.99f );
    auto fse  = CreateFullscreenEffect( FullscreenEffectType::FET_BLIT_WITH_ALPHA, pre->GetValues() );

    SetLogicComponents( logic, pre, fse, nullptr );

    return CreateNodeEffect( logic, NodeEffectType::NET_ALPHA_MASK );
}

// **************************
//
NodeEffectTrPtr  CreateNodeMaskNodeEffect()
{
    auto logic = CreateNodeEffectLogic();

    auto pre  = new NodeMaskPreFullscreenEffectLogic( 0.01f );
    auto fse  = CreateFullscreenEffect( FullscreenEffectType::FET_BLIT_WITH_ALPHA_MASK, pre->GetValues() );
    auto post = new DefaultPostFullscreenEffectLogic( 2 );

    SetLogicComponents( logic, pre, fse, post );

    return CreateNodeEffect( logic, NodeEffectType::NET_NODE_MASK );
}

// **************************
//
NodeEffectTrPtr  CreateWireframeNodeEffect()
{
    auto logic = CreateNodeEffectLogic();

    auto pre  = new DefaultPreFullscreenEffectLogic();
    auto post = new WireframePostFullscreenEffectLogic();

    SetLogicComponents( logic, pre, nullptr, post );

    return CreateNodeEffect( logic, NodeEffectType::NET_WIREFRAME );
}

// **************************
//
NodeEffectTrPtr  CreateMixchannelsNodeEffect()
{
    auto logic = CreateNodeEffectLogic();

    auto pre  = new RenderOffscreenPreFullscreenEffectLogic();
    auto fse  = CreateFullscreenEffect( FullscreenEffectType::FET_MIX_CHANNELS );

    SetLogicComponents( logic, pre, fse, nullptr );

    return CreateNodeEffect( logic, NodeEffectType::NET_MIX_CHANNELS );
}
 
} // anonoymnous


// **************************
//
NodeEffectTrPtr    CreateNodeEffect( NodeEffectType nodeEffectType )
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
        //Interlace and so on
        default:
            assert( false );
    }

    return nullptr;
}

} //bv
