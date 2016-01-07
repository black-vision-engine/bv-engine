#pragma once

#include "Rendering/Logic/FullScreen/FullscreenEffectTr.h"


namespace bv {

class FullscreenEffectGraph;

class CompositeFullscreenEffect : public FullscreenEffectTr
{
private:

   FullscreenEffectGraph *  m_graph; 

public:

                    CompositeFullscreenEffect   ( FullscreenEffectGraph * graph );
                    ~CompositeFullscreenEffect  ();

    virtual void    Render                      ( FullscreenEffectContext * ctx ) override;

};

DEFINE_PTR_TYPE(CompositeFullscreenEffect)
DEFINE_CONST_PTR_TYPE(CompositeFullscreenEffect)

} //bv

