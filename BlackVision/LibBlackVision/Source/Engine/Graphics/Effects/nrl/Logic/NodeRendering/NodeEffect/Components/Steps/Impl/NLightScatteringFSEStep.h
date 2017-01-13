#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/Steps/NFullscreenEffectStep.h"


namespace bv { namespace nrl {

class NFullscreenEffect;

class NLightScatteringFSEStep : public NFullscreenEffectStep
{
private:

    typedef NFullscreenEffectStep Parent;

private:

    NFullscreenEffect *			m_lsEffect;

public:

				                NLightScatteringFSEStep        ();
				                ~NLightScatteringFSEStep       ();

    virtual unsigned int        GetNumRequiredInputs    () const override;

protected:

    virtual void                ReadInputState          ();
    virtual void                ApplyImpl               ( NRenderContext * ctx, const NRenderedData * input ) override;
	virtual void                FreeRenderTargets       ( NRenderContext * ctx, const NRenderedData * input );

public:

    virtual bool                IsIdle                  ( SceneNodeRepr * nodeRepr ) const override;
    virtual bool                IsFinal                 ( SceneNodeRepr * nodeRepr ) const override;

};

} // nrl
} // bv
