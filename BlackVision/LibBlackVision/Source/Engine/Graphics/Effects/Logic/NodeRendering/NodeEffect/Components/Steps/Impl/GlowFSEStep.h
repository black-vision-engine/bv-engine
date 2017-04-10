#pragma once

#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/Steps/FullscreenEffectStep.h"


namespace bv { 

class FullscreenEffect;

class GlowFSEStep : public FullscreenEffectStep
{
private:

    typedef FullscreenEffectStep Parent;

private:

    FullscreenEffect *			m_blurEffect;
	FullscreenEffect *			m_glowEffect;

public:

				                GlowFSEStep        ();
				                ~GlowFSEStep       ();

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

    Float32                     GetBlurSize             () const;
	bool						GetInner				() const;
	bool						GetOuter				() const;

};


} // bv
