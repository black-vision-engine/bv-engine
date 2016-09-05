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
	static auto ret = std::vector< FullscreenEffectGraphNodePtr >();
    return ret;
}

} //bv

#include "Memory/MemoryLeaks.h"




