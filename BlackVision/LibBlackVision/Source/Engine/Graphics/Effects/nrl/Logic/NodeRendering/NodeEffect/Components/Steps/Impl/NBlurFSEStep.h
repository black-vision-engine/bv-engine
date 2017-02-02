#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/Steps/NFullscreenEffectStep.h"


namespace bv { namespace nrl {

class NFullscreenEffect;
class RenderTargetsAllocators;

class NBlurFSEStep : public NFullscreenEffectStep
{
private:

    typedef NFullscreenEffectStep Parent;

private:

    NFullscreenEffect *		m_blurEffect;
	NFullscreenEffect *		m_simpleBlitEffect;

	RenderTargetsAllocators * m_rtAllocators;

public:

				                NBlurFSEStep        ();
				                ~NBlurFSEStep       ();

    virtual unsigned int        GetNumRequiredInputs    () const override;

protected:

    virtual void                ReadInputState          ();
    virtual void                ApplyImpl               ( NRenderContext * ctx, const NRenderedData * input ) override;
	virtual void                FreeRenderTargets       ( NRenderContext * ctx, const NRenderedData * input );

public:

    virtual bool                IsIdle                  ( SceneNodeRepr * nodeRepr ) const override;
    virtual bool                IsFinal                 ( SceneNodeRepr * nodeRepr ) const override;

private:
	Float32						GetBlurSize				() const;

	const RenderTarget *		FastBlur				( NRenderContext * ctx, const NRenderedData & input, Float32 blurSize, const RenderTarget * output );
	const RenderTarget *		BlurInput				( NRenderContext * ctx, const NRenderedData & input, Float32 blurSize, const RenderTarget * output );
	void						ResizeInput				( NRenderContext * ctx, const NRenderedData & input, const RenderTarget * output );
};

} // nrl
} // bv
