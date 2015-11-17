#pragma once

#include "Engine/Graphics/Effects/NodeEffects/NodeEffect.h"


namespace bv {

class NodeMaskNodeEffect : public NodeEffect
{
public:

                    NodeMaskNodeEffect  ();
                    ~NodeMaskNodeEffect ();

    unsigned int    GetForegroundIdx    () const;
    unsigned int    GetBackgroundIdx    () const;

};

} //bv
