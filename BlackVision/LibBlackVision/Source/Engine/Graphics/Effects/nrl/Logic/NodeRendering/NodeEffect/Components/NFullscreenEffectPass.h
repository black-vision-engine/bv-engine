#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/NNodeEffectRenderPass.h"

#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/Steps/NPreFullscreenEffectStep.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/Steps/NFullscreenEffectStep.h"


namespace bv { namespace nrl {

class NFullscreenEffectPass : public NNodeEffectRenderPass
{
private:

    NPreFullscreenEffectStep *  m_preFullscreenStep;
    NFullscreenEffectStep *     m_fullscreenStep;

public:

                        NFullscreenEffectPass   ( NPreFullscreenEffectStep * preFullscreenEffectStep, NFullscreenEffectStep * fullscreenEffectStep );
                        ~NFullscreenEffectPass  ();

    virtual void        ReadInputState          () override;

    virtual void        Apply                   ( SceneNodeRepr * nodeRepr, NRenderContext * ctx ) override;

    virtual bool        IsFinal                 ( SceneNodeRepr * nodeRepr ) const override;
    virtual bool        IsIdle                  ( SceneNodeRepr * nodeRepr ) const override;

    virtual IValuePtr   GetValue                ( const std::string & name ) const override;

    virtual void        AppendRenderPasses_DIRTY_HACK   ( std::set< const RenderablePass * > * passes ) const override;

};

} // nrl
} // bv
