#pragma once

#include "Engine/Types/Values/TypedValues.h"

#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/Steps/NPreFullscreenEffectStep.h"


namespace bv { namespace nrl {

class NNodeMaskPreFSEStep : public NPreFullscreenEffectStep
{
private:

    typedef NPreFullscreenEffectStep Parent;

private:

    NRenderedData   m_renderResult;

    float           m_minThreshold;

    float           m_alpha;
    int             m_maskIdx;
    int             m_fgIdx;

public:

				                    NNodeMaskPreFSEStep     ( IValuePtr alphaVal, float minAlphaThreshold );

    virtual unsigned int            GetNumOutputs           () const override;

protected:

    virtual void                    ReadInputState          () override;
    virtual void                    AllocateRenderTargets   ( NRenderContext * ctx ) override;
    virtual const NRenderedData *   ApplyImpl               ( SceneNodeRepr * nodeRepr, NRenderContext * ctx ) override;

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
