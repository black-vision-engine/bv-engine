#include "NodeEffectRenderLogicSelector.h"

#include <cassert>

#include "Engine/Graphics/SceneGraph/SceneNode.h"

#include "Rendering/Logic/FrameRendering/NodeEffect/DefaultEffectRenderLogic.h"
#include "Rendering/Logic/FrameRendering/NodeEffect/AlphaMaskRenderLogic.h"
#include "Rendering/Logic/FrameRendering/NodeEffect/NodeMaskRenderLogic.h"
#include "Rendering/Logic/FrameRendering/NodeEffect/WireframeRenderLogic.h"
#include "Rendering/Logic/FrameRendering/NodeEffect/LightScatteringRenderLogic.h"
#include "Rendering/Logic/FrameRendering/NodeEffect/AlphaChannelRenderLogic.h"

namespace bv {

// ****************************
//
NodeEffectRenderLogicSelector::NodeEffectRenderLogicSelector                    ()
{
    m_nodeRenderLogicVec.push_back( new DefaultEffectRenderLogic() );
    m_nodeRenderLogicVec.push_back( new AlphaMaskRenderLogic() );
    m_nodeRenderLogicVec.push_back( new NodeMaskRenderLogic() );
    m_nodeRenderLogicVec.push_back( new WireframeRenderLogic() );
    m_nodeRenderLogicVec.push_back( new LightScatteringRenderLogic() );
    //m_nodeRenderLogicVec.push_back( new AlphaChannelRenderLogic() );
}

// ****************************
//
NodeEffectRenderLogicSelector::~NodeEffectRenderLogicSelector                   ()
{
    for ( auto rl : m_nodeRenderLogicVec )
    {
        delete rl;
    }
}

// ****************************
//
NodeEffectRenderLogic * NodeEffectRenderLogicSelector::GetNodeEffectRenderLogic ( SceneNode * node )
{
    assert( (unsigned int) node->GetNodeEffect()->GetType() < (unsigned int) NodeEffect::Type::T_TOTAL );

    return m_nodeRenderLogicVec[ (unsigned int) node->GetNodeEffect()->GetType() ];
}

} //bv
