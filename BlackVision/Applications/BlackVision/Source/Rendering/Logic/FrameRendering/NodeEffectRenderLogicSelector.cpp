#include "NodeEffectRenderLogicSelector.h"

#include <cassert>

#include "Engine/Graphics/SceneGraph/SceneNode.h"

#include "Rendering/Logic/FrameRendering/NodeEffect/NodeEffectRenderLogic.h"
#include "Rendering/Logic/FrameRendering/NodeEffect/DefaultEffectRenderLogic.h"
#include "Rendering/Logic/FrameRendering/NodeEffect/AlphaMaskRenderLogic.h"
#include "Rendering/Logic/FrameRendering/NodeEffect/NodeMaskRenderLogic.h"
#include "Rendering/Logic/FrameRendering/NodeEffect/WireframeRenderLogic.h"


namespace bv {

// ****************************
//
NodeEffectRenderLogicSelector::NodeEffectRenderLogicSelector                    ()
{
    // FIXME: change the public interface of the render logic effects
    m_nodeRenderLogicVec.push_back( new DefaultEffectRenderLogic( nullptr, nullptr ) );
    m_nodeRenderLogicVec.push_back( new AlphaMaskRenderLogic( nullptr, nullptr ) );
    m_nodeRenderLogicVec.push_back( new NodeMaskRenderLogic( nullptr, nullptr ) );
    m_nodeRenderLogicVec.push_back( new WireframeRenderLogic( nullptr, nullptr ) );
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
