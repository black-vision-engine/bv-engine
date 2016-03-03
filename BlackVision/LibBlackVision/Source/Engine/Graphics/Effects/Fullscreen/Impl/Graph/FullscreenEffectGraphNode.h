#pragma once

#include <vector>

#include "CoreDEF.h"


namespace bv {

class FullscreenEffect;
class FullscreenEffectGraphNode;

DEFINE_PTR_TYPE(FullscreenEffectGraphNode)
DEFINE_CONST_PTR_TYPE(FullscreenEffectGraphNode)

class FullscreenEffectGraphNode
{
private:

    FullscreenEffect *                              m_fullscreenEffect;

    std::vector< FullscreenEffectGraphNodePtr >     m_inputNodes;

public:

                            FullscreenEffectGraphNode                       ( FullscreenEffect * fullscreenEffect );
                            ~FullscreenEffectGraphNode                      ();


    FullscreenEffect *                                  AddInput           ();

    void                                                AddInput            ( FullscreenEffect * fullscreenEffect );
    void                                                AddInput            ( FullscreenEffectGraphNodePtr node );

    virtual unsigned int                                GetNumInputNodes    () const;

    virtual const std::vector< FullscreenEffectGraphNodePtr > & GetInputVec () const; 

};

} //bv
