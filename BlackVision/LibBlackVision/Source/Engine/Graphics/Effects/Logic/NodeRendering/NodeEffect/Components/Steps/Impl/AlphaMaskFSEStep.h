#pragma once

#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/Steps/FullscreenEffectStep.h"


namespace bv { 

class FullscreenEffect;

class AlphaMaskFSEStep : public FullscreenEffectStep
{
private:

    typedef FullscreenEffectStep Parent;

private:

    FullscreenEffect * m_blitWithAlphaEffect;

    float               m_minThreshold;
    float               m_maxThreshold;

    float               m_alpha;

public:

				                    AlphaMaskFSEStep       ( float minAlphaThreshold, float maxAlphaThreshold );
				                    ~AlphaMaskFSEStep      ();

        virtual unsigned int        GetNumRequiredInputs    () const override;

protected:

        virtual void                ReadInputState          ();
        virtual void                ApplyImpl               ( RenderContext * ctx, const RenderedData * input ) override;
        virtual void                FreeRenderTargets       ( RenderContext * ctx, const RenderedData * input );

public:

        virtual bool                IsIdle                  ( SceneNodeRepr * nodeRepr ) const override;
        virtual bool                IsFinal                 ( SceneNodeRepr * nodeRepr ) const override;
        virtual void                AppendRenderPasses_DIRTY_HACK   ( std::set< const RenderablePass * > * passes ) const override;

private:

    float                           GetAlpha                () const;

};


} // bv
