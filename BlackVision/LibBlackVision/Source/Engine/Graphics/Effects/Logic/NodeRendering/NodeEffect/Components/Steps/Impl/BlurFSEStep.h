#pragma once

#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/Steps/FullscreenEffectStep.h"


namespace bv { namespace nrl {

class FullscreenEffect;

class BlurFSEStep : public FullscreenEffectStep
{
private:

    typedef FullscreenEffectStep Parent;

private:

    FullscreenEffect *		m_blurEffect;
	FullscreenEffect *		m_simpleBlitEffect;

	const RenderTarget *	m_tmpRT;

public:

				                BlurFSEStep        ();
				                ~BlurFSEStep       ();

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

    Float32						GetBlurSize				() const;

	void						FastBlur				( RenderContext * ctx, const RenderedData & input, Float32 blurSize, const RenderTarget * output );
	void						BlurInput				( RenderContext * ctx, const RenderedData & input, Float32 blurSizeW, Float32 blurSizeH, const RenderTarget * output );
	void						ResizeInput				( RenderContext * ctx, const RenderedData & input, const RenderTarget * output );
	void						SetQuadTransform		( Float32 scaleX, Float32 scaleY, Float32 translateX, Float32 translateY );
	void						SetUVTransform			( Float32 scaleX, Float32 scaleY, Float32 translateX, Float32 translateY );
};

} // nrl
} // bv
