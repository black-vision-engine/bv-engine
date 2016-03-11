#pragma once

#include "Engine/Graphics/Effects/FullScreen/FullscreenEffect.h"

#include "Engine/Graphics/Effects/FullScreen/Impl/Graph/FullscreenEffectGraphNode.h"

#include "Engine/Graphics/Effects/Fullscreen/Impl/Graph/InputFullscreenEffectGraphNode.h"

#include <vector>

namespace bv {

class FullscreenEffectGraph;
class RenderTarget;

class CompositeFullscreenEffect : public FullscreenEffect
{
private:

   FullscreenEffectGraph *                  m_graph;
   std::vector< RenderTarget * >            m_sourceRenderTargets;

public:

                            CompositeFullscreenEffect   ( FullscreenEffectGraph * graph );
                            ~CompositeFullscreenEffect  ();

    virtual void            Update                      () override;
    virtual void            Render                      ( FullscreenEffectContext * ctx ) override;

    virtual unsigned int    GetNumInputs                () const override;

    virtual void            SynchronizeInputData        ( FullscreenEffectContext * ctx ) override;

    bool                    AddAdditionalPreLogicInputs ( const std::vector< InputFullscreenEffectGraphNodePtr > & additionalNodes );

    virtual std::vector< IValuePtr >    GetValues       () const override;

    virtual void                        AddTexture      ( const ITextureDescriptorConstPtr & txDesc ) override;

    FullscreenEffectGraph *             GetGraph        ();

    virtual void                        GetRenderPasses ( std::set< const RenderablePass * > * passes ) const override;

private:

    void            AddTexture                  ( FullscreenEffectGraphNodePtr node, const ITextureDescriptorConstPtr & txDesc );
    void            RenderGraphNode             ( FullscreenEffectGraphNodePtr node, FullscreenEffectContext * ctx );
    void            Update                      ( FullscreenEffectGraphNodePtr node );
    void            GetRenderPasses             ( FullscreenEffectGraphNodePtr node, std::set< const RenderablePass * > * passes ) const;

};

DEFINE_PTR_TYPE(CompositeFullscreenEffect)
DEFINE_CONST_PTR_TYPE(CompositeFullscreenEffect)

} //bv

