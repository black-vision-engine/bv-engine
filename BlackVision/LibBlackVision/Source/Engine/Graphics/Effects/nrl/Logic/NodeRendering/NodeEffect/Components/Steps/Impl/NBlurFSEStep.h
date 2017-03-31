#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/Steps/NFullscreenEffectStep.h"


namespace bv { namespace nrl {

class NFullscreenEffect;

class NBlurFSEStep : public NFullscreenEffectStep
{
private:

    typedef NFullscreenEffectStep Parent;

private:

    NFullscreenEffect *		m_blurEffect;
	NFullscreenEffect *		m_simpleBlitEffect;

	const RenderTarget *	m_tmpRT;

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
    virtual void                AppendRenderPasses_DIRTY_HACK   ( std::set< const RenderablePass * > * passes ) const override;

private:

    Float32						GetBlurSize				() const;

	void						FastBlur				( NRenderContext * ctx, const NRenderedData & input, Float32 blurSize, const RenderTarget * output );
	void						BlurInput				( NRenderContext * ctx, const NRenderedData & input, Float32 blurSizeW, Float32 blurSizeH, const RenderTarget * output );
	void						ResizeInput				( NRenderContext * ctx, const NRenderedData & input, const RenderTarget * output );
	void						SetQuadTransform		( Float32 scaleX, Float32 scaleY, Float32 translateX, Float32 translateY );
	void						SetUVTransform			( Float32 scaleX, Float32 scaleY, Float32 translateX, Float32 translateY );
};

} // nrl
} // bv
