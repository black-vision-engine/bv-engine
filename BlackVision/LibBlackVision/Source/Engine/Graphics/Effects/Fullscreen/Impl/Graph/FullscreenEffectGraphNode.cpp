#include "stdafx.h"

#include "FullscreenEffectGraphNode.h"

#include "Engine/Graphics/Effects/FullScreen/FullscreenEffect.h"

#include "Engine/Graphics/Effects/Fullscreen/Impl/CompositeFullscreenEffect.h"


namespace  bv {

// ****************************
//
FullscreenEffectGraphNode::FullscreenEffectGraphNode               ( FullscreenEffect * fullscreenEffect )
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
FullscreenEffect *                                  FullscreenEffectGraphNode::GetEffect   ()
{
    return m_fullscreenEffect;
}

// ****************************
//
void                                                FullscreenEffectGraphNode::AddInput    ( FullscreenEffect * fullscreenEffect )
{
    AddInput( std::make_shared< FullscreenEffectGraphNode >( fullscreenEffect ) );
}

// ****************************
//
void                                                FullscreenEffectGraphNode::AddInput    ( FullscreenEffectGraphNodePtr node )
{
    m_inputNodes.push_back( node );
    if( auto composite = dynamic_cast< CompositeFullscreenEffect * >( m_fullscreenEffect ) )
    {
        std::vector< FullscreenEffectGraphNodePtr > v( 1 );
        v[ 0 ] = node;
        composite->AddAdditionalPreLogicInputs( v );
    }
}

// ****************************
//
unsigned int                                        FullscreenEffectGraphNode::GetNumInputNodes    () const
{
    return (unsigned int) m_inputNodes.size();
}

// ****************************
//
const std::vector< FullscreenEffectGraphNodePtr > & FullscreenEffectGraphNode::GetInputVec () const
{
    return m_inputNodes;
}

} //bv
