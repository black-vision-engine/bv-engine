#pragma once

#include "Engine/Types/Values/TypedValues.h"

#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/Steps/PreFullscreenEffectStep.h"


namespace bv { namespace nrl {

class NodeMaskPreFSEStep : public PreFullscreenEffectStep
{
private:

    typedef PreFullscreenEffectStep Parent;

private:

    RenderedData   m_renderResult;

    float           m_minThreshold;

    float           m_alpha;
    int             m_maskIdx;
    int             m_fgIdx;

public:

				                    NodeMaskPreFSEStep     ( IValuePtr alphaVal, float minAlphaThreshold );

    virtual unsigned int            GetNumOutputs           () const override;

protected:

    virtual void                    ReadInputState          () override;
    virtual void                    AllocateRenderTargets   ( RenderContext * ctx ) override;
    virtual const RenderedData *   ApplyImpl               ( SceneNodeRepr * nodeRepr, RenderContext * ctx ) override;

public:

    virtual bool                    IsIdle                  ( SceneNodeRepr * nodeRepr ) const override;
    virtual bool                    IsFinal                 ( SceneNodeRepr * nodeRepr ) const override;

private:

    float                           GetAlpha                () const;
    int                             GetMaskIdx              () const;
    int                             GetFgIdx                () const;

};

} // nrl
} // bv
