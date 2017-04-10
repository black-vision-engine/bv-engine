#pragma once

#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/NodeEffectRenderPass.h"

#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/Steps/FinalizeStep.h"


namespace bv { namespace nrl {

class FinalizePass : public NodeEffectRenderPass
{
private:

    FinalizeStep *     m_finalizeStep;

public:

                        FinalizePass   ( FinalizeStep * finalizeStep );
                        ~FinalizePass  ();

    virtual void        ReadInputState  () override;

    virtual void        Apply           ( SceneNodeRepr * nodeRepr, RenderContext * ctx ) override;

    virtual bool        IsFinal         ( SceneNodeRepr * nodeRepr ) const override;
    virtual bool        IsIdle          ( SceneNodeRepr * nodeRepr ) const override;

    virtual IValuePtr   GetValue        ( const std::string & name ) const override;

    virtual void        AppendRenderPasses_DIRTY_HACK   ( std::set< const RenderablePass * > * passes ) const override;

};

} // nrl
} // bv
