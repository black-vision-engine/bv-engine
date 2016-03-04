#include "stdafx.h"

#include "NodeEffectFactory.h"

#include <cassert>

#include "Engine/Graphics/Effects/NodeEffect/NodeEffect.h"
#include "Engine/Graphics/Effects/NodeEffect/NodeEffectLogic.h"

//PRE
#include "Engine/Graphics/Effects/NodeEffect/Logic/PreImpl/DefaultPreFullscreenEffectLogic.h"
#include "Engine/Graphics/Effects/NodeEffect/Logic/PreImpl/AlphaMaskPreFullscreenEffectLogic.h"
#include "Engine/Graphics/Effects/NodeEffect/Logic/PreImpl/NodeMaskPreFullscreenEffectLogic.h"
#include "Engine/Graphics/Effects/NodeEffect/Logic/PreImpl/InterlacePreFullscreenEffectLogic.h"
#include "Engine/Graphics/Effects/NodeEffect/Logic/PreImpl/RenderOffscreenPreFullscreenEffectLogic.h"
#include "Engine/Graphics/Effects/NodeEffect/Logic/PreImpl/BlurPreFullscreenEffectLogic.h"
#include "Engine/Graphics/Effects/NodeEffect/Logic/PreImpl/ShadowPreFullscreenEffectLogic.h"

//FSE
#include "Engine/Graphics/Effects/Fullscreen/FullscreenEffectFactory.h"

//POST
#include "Engine/Graphics/Effects/NodeEffect/Logic/PostImpl/DefaultPostFullscreenEffectLogic.h"
#include "Engine/Graphics/Effects/NodeEffect/Logic/PostImpl/WireframePostFullscreenEffectLogic.h"


namespace bv {

namespace {

// **************************
//
NodeEffectPtr  CreateNodeEffect( NodeEffectLogic * logic, NodeEffectType neType )
{
    return std::make_shared< NodeEffect >( logic, neType );
}

// **************************
//
NodeEffectLogic *  CreateNodeEffectLogic()
{
    return new NodeEffectLogic();
}

// **************************
//
void    SetLogicComponents( NodeEffectLogic * logic, PreFullscreenEffectLogic * pre, FullscreenEffectInstance * fse, PostFullscreenEffectLogic * post )
{
    assert( logic && ( pre || fse || post ) );

    logic->SetComponent( pre );
    logic->SetComponent( fse );
    logic->SetComponent( post );
}

// **************************
//
NodeEffectPtr       CreateDefaultNodeEffect()
{
    auto logic = CreateNodeEffectLogic();
    
    SetLogicComponents( logic, new DefaultPreFullscreenEffectLogic(), nullptr, nullptr );

    return CreateNodeEffect( logic, NodeEffectType::NET_DEFAULT );
}

// **************************
//
NodeEffectPtr       CreateAlphaMaskNodeEffect()
{
    auto logic = CreateNodeEffectLogic();

    auto pre  = new AlphaMaskPreFullscreenEffectLogic( 0.01f, 0.99f );
    auto fse  = CreateFullscreenEffectInstance( FullscreenEffectType::FET_BLIT_WITH_ALPHA, pre->GetValues() );

    SetLogicComponents( logic, pre, fse, nullptr );

    return CreateNodeEffect( logic, NodeEffectType::NET_ALPHA_MASK );
}

// **************************
//
NodeEffectPtr       CreateNodeMaskNodeEffect()
{
    auto logic = CreateNodeEffectLogic();

    auto pre  = new NodeMaskPreFullscreenEffectLogic( 0.01f );
    auto fse  = CreateFullscreenEffectInstance( FullscreenEffectType::FET_BLIT_WITH_ALPHA_MASK, pre->GetValues() );
    auto post = new DefaultPostFullscreenEffectLogic( 2 );

    SetLogicComponents( logic, pre, fse, post );

    return CreateNodeEffect( logic, NodeEffectType::NET_NODE_MASK );
}

// **************************
//
NodeEffectPtr       CreateWireframeNodeEffect()
{
    auto logic = CreateNodeEffectLogic();

    auto pre  = new DefaultPreFullscreenEffectLogic();
    auto post = new WireframePostFullscreenEffectLogic();

    SetLogicComponents( logic, pre, nullptr, post );

    return CreateNodeEffect( logic, NodeEffectType::NET_WIREFRAME );
}

// **************************
//
NodeEffectPtr       CreateMixchannelsNodeEffect()
{
    auto logic = CreateNodeEffectLogic();

    auto pre  = new RenderOffscreenPreFullscreenEffectLogic();
    auto fse  = CreateFullscreenEffectInstance( FullscreenEffectType::FET_MIX_CHANNELS );

    SetLogicComponents( logic, pre, fse, nullptr );

    return CreateNodeEffect( logic, NodeEffectType::NET_MIX_CHANNELS );
}
 
// **************************
//
NodeEffectPtr       CreateBlurEffect()
{
    auto logic = CreateNodeEffectLogic();

    auto pre  = new BlurPreFullscreenEffectLogic();
    auto fse  = CreateFullscreenEffectInstance( FullscreenEffectType::FET_BLUR, pre->GetValues() );

    SetLogicComponents( logic, pre, fse, nullptr );

    return CreateNodeEffect( logic, NodeEffectType::NET_BLUR );
}

// **************************
//
NodeEffectPtr       CreateShadowEffect()
{
    auto logic = CreateNodeEffectLogic();

    auto pre  = new ShadowPreFullscreenEffectLogic();
    auto fse  = CreateFullscreenEffectInstance( FullscreenEffectType::FET_SHADOW, pre->GetValues() );

    SetLogicComponents( logic, pre, fse, nullptr );

    return CreateNodeEffect( logic, NodeEffectType::NET_SHADOW );
}

} // anonoymnous


// **************************
//
NodeEffectPtr       CreateNodeEffect( NodeEffectType nodeEffectType )
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
        case NodeEffectType::NET_BLUR:
            return CreateBlurEffect();
        case NodeEffectType::NET_SHADOW:
            return CreateShadowEffect();

        //Interlace and so on
        default:
            assert( false );
    }

    return nullptr;
}

} //bv
