#pragma once

#include <vector>

#include "CoreDEF.h"


namespace bv {

class FullscreenEffectTr;
class FullscreenEffectGraphNode;

DEFINE_PTR_TYPE(FullscreenEffectGraphNode)
DEFINE_CONST_PTR_TYPE(FullscreenEffectGraphNode)

class FullscreenEffectGraphNode
{
private:

    FullscreenEffectTr *                            m_fullscreenEffect;

    std::vector< FullscreenEffectGraphNodePtr >     m_inputNodes;

public:

                            FullscreenEffectGraphNode               ( FullscreenEffectTr * fullscreenEffect );
                            ~FullscreenEffectGraphNode              ();


    FullscreenEffectTr *                                GetEffect   ();

    void                                                AddInput    ( FullscreenEffectTr * fullscreenEffect );
    void                                                AddInput    ( FullscreenEffectGraphNodePtr node );

    const std::vector< FullscreenEffectGraphNodePtr > & GetInputVec () const; 

};

} //bv
