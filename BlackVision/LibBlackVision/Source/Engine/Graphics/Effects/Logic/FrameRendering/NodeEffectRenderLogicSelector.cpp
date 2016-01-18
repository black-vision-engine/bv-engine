#include "NodeEffectRenderLogicSelector.h"

#include <cassert>

#include "Engine/Graphics/SceneGraph/SceneNode.h"

#include "Engine/Graphics/Effects/Logic/FrameRendering/NodeEffect/DefaultEffectRenderLogic.h"
#include "Engine/Graphics/Effects/Logic/FrameRendering/NodeEffect/AlphaMaskRenderLogic.h"
#include "Engine/Graphics/Effects/Logic/FrameRendering/NodeEffect/NodeMaskRenderLogic.h"
#include "Engine/Graphics/Effects/Logic/FrameRendering/NodeEffect/WireframeRenderLogic.h"


namespace bv {

// ****************************
//
NodeEffectRenderLogicSelector::NodeEffectRenderLogicSelector                    ()
{
    m_nodeRenderLogicVec.push_back( new DefaultEffectRenderLogic() );
    m_nodeRenderLogicVec.push_back( new AlphaMaskRenderLogic() );
    m_nodeRenderLogicVec.push_back( new NodeMaskRenderLogic() );
    m_nodeRenderLogicVec.push_back( new WireframeRenderLogic() );
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
