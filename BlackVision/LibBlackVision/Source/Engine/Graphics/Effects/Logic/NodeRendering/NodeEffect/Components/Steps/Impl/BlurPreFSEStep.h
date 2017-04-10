#pragma once

#include "Engine/Types/Values/TypedValues.h"

#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/Steps/PreFullscreenEffectStep.h"


namespace bv { 

class BlurPreFSEStep : public PreFullscreenEffectStep
{
private:

    typedef PreFullscreenEffectStep Parent;

private:

    RenderedData   m_renderResult;

public:

				                    BlurPreFSEStep			( IValuePtr blurSize );

    virtual unsigned int            GetNumOutputs           () const override;

protected:

    virtual void                    ReadInputState          () override;
    virtual void                    AllocateRenderTargets   ( RenderContext * ctx ) override;
    virtual const RenderedData *   ApplyImpl               ( SceneNodeRepr * nodeRepr, RenderContext * ctx ) override;

public:

    virtual bool                    IsIdle                  ( SceneNodeRepr * nodeRepr ) const override;
    virtual bool                    IsFinal                 ( SceneNodeRepr * nodeRepr ) const override;

	Float32                         GetBlurSize             () const;
};


} // bv
