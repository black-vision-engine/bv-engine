#pragma once

#include "Engine/Graphics/Effects/NodeEffects/NodeEffect.h"


namespace bv {

class NodeMaskNodeEffect : public NodeEffect
{
private:

    unsigned int m_bgNodeIdx;
    unsigned int m_fgNodeIdx;

public:

                    NodeMaskNodeEffect  ( unsigned int backgroundNodeIdx, unsigned int foregroundNodeIdx );
                    ~NodeMaskNodeEffect ();

    unsigned int    GetForegroundIdx    () const;
    unsigned int    GetBackgroundIdx    () const;

};

} //bv
