#include "FullscreenEffectGraphNode.h"

#include "Rendering/Logic/FullScreen/FullscreenEffectTr.h"


namespace  bv {

// ****************************
//
FullscreenEffectGraphNode::FullscreenEffectGraphNode               ( FullscreenEffectTr * fullscreenEffect )
    : m_fullscreenEffect( fullscreenEffect )
{
}

// ****************************
//
FullscreenEffectGraphNode::~FullscreenEffectGraphNode              ()
{
    delete m_fullscreenEffect;
}

// ****************************
//
FullscreenEffectTr *                                FullscreenEffectGraphNode::GetEffect   ()
{
    return m_fullscreenEffect;
}

// ****************************
//
void                                                FullscreenEffectGraphNode::AddInput    ( FullscreenEffectTr * fullscreenEffect )
{
    m_inputNodes.push_back( std::make_shared< FullscreenEffectGraphNode >( fullscreenEffect ) );
}

// ****************************
//
const std::vector< FullscreenEffectGraphNodePtr > & FullscreenEffectGraphNode::GetInputVec () const
{
    return m_inputNodes;
}

} //bv
