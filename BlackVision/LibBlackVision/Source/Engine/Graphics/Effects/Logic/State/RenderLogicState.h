#pragma once

#include "Engine/Graphics/Effects/Utils/RenderTargetStackAllocator.h"

#include "Engine/Graphics/Effects/Logic/Components/RenderContext.h"


namespace bv { 

class RenderLogicState
{
private:

    RenderTargetStackAllocator      m_renderTargetAllocator;
    RenderQueueStackAllocator       m_renderQueueAllocator;

    RenderContext					m_ctx;

    bool                            m_initialized;
    bool                            m_editMode;

public:

                                    RenderLogicState                ( unsigned int width, unsigned int height );

    RenderContext *			        GetRenderContext	            ();
    RenderTargetStackAllocator *    GetRenderTargetStackAllocator   ();
    RenderQueueStackAllocator *     GetRenderQueueStackAllocator    ();

    bool                            IsInitialized                   () const;
    bool                            IsEditMode                      () const;

    void                            SwitchEditMode                  ( bool value );

    void                            Initialize                      ( Renderer * renderer, audio::AudioRenderer * audio );

};


// ************************
//
inline RenderContext *			        context			        ( RenderLogicState & state )
{
	return state.GetRenderContext();
}

// ************************
//
inline RenderTargetStackAllocator *		render_target_allocator ( RenderLogicState & state )
{
    return state.GetRenderTargetStackAllocator();
}

// ************************
//
inline RenderQueueStackAllocator *		render_queu_allocator   ( RenderLogicState & state )
{
    return state.GetRenderQueueStackAllocator();
}


} // bv
