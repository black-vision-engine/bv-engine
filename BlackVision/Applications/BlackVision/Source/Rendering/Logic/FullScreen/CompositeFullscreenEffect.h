#pragma once

#include "Rendering/Logic/FullScreen/FullscreenEffectTr.h"
#include "Rendering/Logic/FullScreen/FullscreenEffectGraphNode.h"


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

private:

    void            RenderGraphNode             ( FullscreenEffectGraphNodePtr node, Renderer * renderer, RenderTarget * outputRenderTarget, RenderTargetStackAllocator * allocator );

};

DEFINE_PTR_TYPE(CompositeFullscreenEffect)
DEFINE_CONST_PTR_TYPE(CompositeFullscreenEffect)

} //bv
