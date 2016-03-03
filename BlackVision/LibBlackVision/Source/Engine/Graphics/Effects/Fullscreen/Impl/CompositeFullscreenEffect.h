#pragma once

#include "Engine/Graphics/Effects/FullScreen/FullscreenEffect.h"

#include "Engine/Graphics/Effects/FullScreen/Impl/Graph/FullscreenEffectGraphNode.h"


namespace bv {

class FullscreenEffectGraph;

class CompositeFullscreenEffect : public FullscreenEffect
{
private:

   FullscreenEffectGraph *  m_graph; 

public:

                            CompositeFullscreenEffect   ( FullscreenEffectGraph * graph );
                            ~CompositeFullscreenEffect  ();

    virtual void            Update                      () override;
    virtual void            Render                      ( FullscreenEffectContext * ctx ) override;

    virtual unsigned int    GetNumInputs                () const override;

    virtual void            SynchronizeInputData        ( FullscreenEffectContext * ctx ) override;

    virtual std::vector< IValuePtr >    GetValues       () const override;

private:

    void            RenderGraphNode             ( FullscreenEffectGraphNodePtr node, Renderer * renderer, RenderTarget * outputRenderTarget, RenderTargetStackAllocator * allocator );

};

DEFINE_PTR_TYPE(CompositeFullscreenEffect)
DEFINE_CONST_PTR_TYPE(CompositeFullscreenEffect)

} //bv

