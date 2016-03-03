#include "stdafx.h"

#include "InputFullscreenEffectGraphNode.h"


namespace  bv {

// ****************************
//
InputFullscreenEffectGraphNode::InputFullscreenEffectGraphNode    ()
	: FullscreenEffectGraphNode( nullptr )
{}

// ****************************
//
InputFullscreenEffectGraphNode::~InputFullscreenEffectGraphNode    ()
{}

// ****************************
//
unsigned int										InputFullscreenEffectGraphNode::GetNumInputNodes    () const
{
	return 0;
}

// ****************************
//
const std::vector< FullscreenEffectGraphNodePtr > & InputFullscreenEffectGraphNode::GetInputVec () const
{
    return std::vector< FullscreenEffectGraphNodePtr >();
}

} //bv
