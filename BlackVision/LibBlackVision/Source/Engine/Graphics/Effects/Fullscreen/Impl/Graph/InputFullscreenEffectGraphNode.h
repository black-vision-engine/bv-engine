#pragma once

#include "FullscreenEffectGraphNode.h"


namespace bv {

class InputFullscreenEffectGraphNode;

DEFINE_PTR_TYPE(InputFullscreenEffectGraphNode)
DEFINE_CONST_PTR_TYPE(InputFullscreenEffectGraphNode)

class InputFullscreenEffectGraphNode : public FullscreenEffectGraphNode
{

public:

                            InputFullscreenEffectGraphNode                 ();
                            ~InputFullscreenEffectGraphNode                ();

    unsigned int                                        GetNumInputNodes    () const override;

    const std::vector< FullscreenEffectGraphNodePtr > & GetInputVec         () const override; 

};

} //bv
