#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/Components/Steps/NPreFullscreenEffectStep.h"


namespace bv { namespace nrl {

class NAlphaMaskPreFSEStep : public NPreFullscreenEffectStep
{
private:

    typedef NPreFullscreenEffectStep Parent;

private:

    NRenderedData   m_renderResult;

    float           m_minThreshold;
    float           m_maxThreshold;

    float           m_alpha;

public:

				                    NAlphaMaskPreFSEStep    ( NRenderComponentStatePtr initialState, float minAlphaThreshold, float maxAlphaThreshold );

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

};

} // nrl
} // bv
