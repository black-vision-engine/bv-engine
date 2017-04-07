#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/NodeEffectRenderPass.h"

#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/Steps/PreFullscreenEffectStep.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/Steps/FullscreenEffectStep.h"


namespace bv { namespace nrl {

class FullscreenEffectPass : public NodeEffectRenderPass
{
private:

    PreFullscreenEffectStep *  m_preFullscreenStep;
    FullscreenEffectStep *     m_fullscreenStep;

public:

                        FullscreenEffectPass   ( PreFullscreenEffectStep * preFullscreenEffectStep, FullscreenEffectStep * fullscreenEffectStep );
                        ~FullscreenEffectPass  ();

    virtual void        ReadInputState          () override;

    virtual void        Apply                   ( SceneNodeRepr * nodeRepr, RenderContext * ctx ) override;

    virtual bool        IsFinal                 ( SceneNodeRepr * nodeRepr ) const override;
    virtual bool        IsIdle                  ( SceneNodeRepr * nodeRepr ) const override;

    virtual IValuePtr   GetValue                ( const std::string & name ) const override;

    virtual void        AppendRenderPasses_DIRTY_HACK   ( std::set< const RenderablePass * > * passes ) const override;

};

} // nrl
} // bv
