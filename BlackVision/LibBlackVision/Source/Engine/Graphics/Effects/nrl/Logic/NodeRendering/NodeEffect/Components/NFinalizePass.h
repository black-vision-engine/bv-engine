#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/NNodeEffectRenderPass.h"

#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/Steps/NFinalizeStep.h"


namespace bv { namespace nrl {

class NFinalizePass : public NNodeEffectRenderPass
{
private:

    NFinalizeStep *     m_finalizeStep;

public:

                        NFinalizePass   ( NFinalizeStep * finalizeStep );
                        ~NFinalizePass  ();

    virtual void        ReadInputState  () override;

    virtual void        Apply           ( SceneNodeRepr * nodeRepr, NRenderContext * ctx ) override;

    virtual bool        IsFinal         ( SceneNodeRepr * nodeRepr ) const override;
    virtual bool        IsIdle          ( SceneNodeRepr * nodeRepr ) const override;

    virtual IValuePtr   GetValue        ( const std::string & name ) const override;

    virtual void        AppendRenderPasses_DIRTY_HACK   ( std::set< const RenderablePass * > * passes ) const override;

};

} // nrl
} // bv
