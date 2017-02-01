#pragma once

#include "Engine/Graphics/Effects/Utils/RenderTargetStackAllocator.h"

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/OutputLogic.h"

#include "Engine/Graphics/Effects/nrl/Logic/Components/NRenderContext.h"

#include "Engine/Graphics/Effects/nrl/Logic/Components/RenderedChannelsData.h"


namespace bv { namespace nrl {

class NRenderLogicState
{
private:

    RenderTargetStackAllocator      m_renderTargetAllocator;
    RenderQueueStackAllocator       m_renderQueueAllocator;

    RenderedChannelsData            m_renderedChannelsData;

    NRenderContext					m_ctx;

    OutputLogic                     m_outputLogic;

    bool                            m_initialized;

public:

                                NRenderLogicState       ( unsigned int width, unsigned int height, unsigned int numTrackedRenderTargetsPerOutputType );

    OutputLogic *               GetOutputLogic          ();
    NRenderContext *			GetRenderContext	    ();
    RenderedChannelsData *      GetRenderedChannelsData ();

    bool                        IsInitialized           () const;

    void                        Initialize              ( Renderer * renderer, audio::AudioRenderer * audio );

};


// ************************
//
inline OutputLogic  *           output_logic            ( NRenderLogicState & state )
{
    return state.GetOutputLogic();
}

// ************************
//
inline RenderedChannelsData *   rendered_channels_data  ( NRenderLogicState & state )
{
    return state.GetRenderedChannelsData();
}

// ************************
//
inline NRenderContext *			context			        ( NRenderLogicState & state )
{
	return state.GetRenderContext();
}

} // nrl
} // bv
