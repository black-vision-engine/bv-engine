#pragma once

#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/Components/Steps/PreFullscreenEffectStep.h"


namespace bv { 

class AlphaMaskPreFSEStep : public PreFullscreenEffectStep
{
private:

    typedef PreFullscreenEffectStep Parent;

private:

    RenderedData   m_renderResult;

    float           m_minThreshold;
    float           m_maxThreshold;

    float           m_alpha;

public:

				                    AlphaMaskPreFSEStep    ( RenderComponentStatePtr initialState, float minAlphaThreshold, float maxAlphaThreshold );

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

};


} // bv
