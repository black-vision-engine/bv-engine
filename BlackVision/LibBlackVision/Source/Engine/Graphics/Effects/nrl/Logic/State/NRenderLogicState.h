#pragma once

#include "Engine/Graphics/Effects/Utils/RenderTargetStackAllocator.h"

#include "Engine/Graphics/Effects/nrl/Logic/Components/NRenderContext.h"


namespace bv { namespace nrl {

class NRenderLogicState
{
private:

    RenderTargetStackAllocator      m_renderTargetAllocator;
    RenderQueueStackAllocator       m_renderQueueAllocator;

    NRenderContext					m_ctx;

    bool                            m_initialized;

public:

                                    NRenderLogicState               ( unsigned int width, unsigned int height );

    NRenderContext *			    GetRenderContext	            ();
    RenderTargetStackAllocator *    GetRenderTargetStackAllocator   ();
    RenderQueueStackAllocator *     GetRenderQueueStackAllocator    ();

    bool                            IsInitialized                   () const;

    void                            Initialize                      ( Renderer * renderer, audio::AudioRenderer * audio );

};


// ************************
//
inline NRenderContext *			        context			        ( NRenderLogicState & state )
{
	return state.GetRenderContext();
}

// ************************
//
inline RenderTargetStackAllocator *		render_target_allocator ( NRenderLogicState & state )
{
    return state.GetRenderTargetStackAllocator();
}

// ************************
//
inline RenderQueueStackAllocator *		render_queu_allocator   ( NRenderLogicState & state )
{
    return state.GetRenderQueueStackAllocator();
}

} // nrl
} // bv
