#pragma once

#include "Engine/Graphics/Effects/Utils/RenderTargetStackAllocator.h"

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/OutputLogic.h"
#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/RenderResult.h"

#include "Engine/Graphics/Effects/nrl/Logic/NRenderContext.h"


namespace bv { namespace nrl {

class NRenderLogicState
{
private:

    RenderTargetStackAllocator      m_renderTargetAllocator;
    RenderQueueStackAllocator       m_renderQueueAllocator;

    NRenderContext					m_ctx;

    OutputLogic                     m_outputLogic;
    RenderResult                    m_renderResult;

    bool                            m_initialized;

public:

                                NRenderLogicState   ( unsigned int width, unsigned int height, unsigned int numTrackedRenderTargetsPerOutputType, unsigned int sharedMemScaleFactor );

    RenderResult *              GetRenderResult     ();
    OutputLogic *               GetOutputLogic      ();
    NRenderContext *			GetRenderContext	();

    bool                        IsInitialized       () const;
    void                        Initialize          ( Renderer * renderer, audio::AudioRenderer * audio );

};

// ************************
//
inline RenderResult *           render_result   ( NRenderLogicState & state )
{
    return state.GetRenderResult();
}

// ************************
//
inline OutputLogic  *           output_logic    ( NRenderLogicState & state )
{
    return state.GetOutputLogic();
}

// ************************
//
inline NRenderContext *			context			( NRenderLogicState & state )
{
	return state.GetRenderContext();
}

} // nrl
} // bv
