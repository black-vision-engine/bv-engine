#pragma once

#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/Steps/FullscreenEffectStep.h"
#include "Engine/Graphics/Effects/Logic/FullscreenRendering/FullscreenEffect.h"


namespace bv { 

class FullscreenEffect;

class ColorCorrectionFSEStep : public FullscreenEffectStep
{
private:

    typedef FullscreenEffectStep Parent;

private:

    FullscreenEffect *  m_colorCorrectionEffect;

    float               m_minThreshold;

    float               m_alpha;

public:

                                    ColorCorrectionFSEStep  ( FullscreenEffectType nnodeEffectType );
				                    ~ColorCorrectionFSEStep ();

        virtual unsigned int        GetNumRequiredInputs    () const override;

protected:

        virtual void                ApplyImpl               ( RenderContext * ctx, const RenderedData * input ) override;
        virtual void                FreeRenderTargets       ( RenderContext * ctx, const RenderedData * input );

public:

        virtual bool                IsIdle                  ( SceneNodeRepr * nodeRepr ) const override;
        virtual bool                IsFinal                 ( SceneNodeRepr * nodeRepr ) const override;
        virtual void                AppendRenderPasses_DIRTY_HACK   ( std::set< const RenderablePass * > * passes ) const override;
};


} // bv
