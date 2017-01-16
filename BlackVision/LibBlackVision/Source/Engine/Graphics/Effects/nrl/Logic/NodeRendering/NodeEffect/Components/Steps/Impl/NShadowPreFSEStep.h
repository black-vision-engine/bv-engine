#pragma once

#include "Engine/Types/Values/TypedValues.h"

#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/Steps/NPreFullscreenEffectStep.h"


namespace bv { namespace nrl {

class NShadowPreFSEStep : public NPreFullscreenEffectStep
{
private:

    typedef NPreFullscreenEffectStep Parent;

private:

    NRenderedData   m_renderResult;

public:

				                    NShadowPreFSEStep		( IValuePtr blurSize, IValuePtr shift, IValuePtr inner, IValuePtr outer );

    virtual unsigned int            GetNumOutputs           () const override;

protected:

    virtual void                    ReadInputState          () override;
    virtual void                    AllocateRenderTargets   ( NRenderContext * ctx ) override;
    virtual const NRenderedData *   ApplyImpl               ( SceneNodeRepr * nodeRepr, NRenderContext * ctx ) override;

public:

    virtual bool                    IsIdle                  ( SceneNodeRepr * nodeRepr ) const override;
    virtual bool                    IsFinal                 ( SceneNodeRepr * nodeRepr ) const override;

	Float32                         GetBlurSize             () const;
	glm::vec2                       GetShift				() const;
	bool							GetInner				() const;
	bool							GetOuter				() const;
};

} // nrl
} // bv
