#pragma once

#include "Engine/Audio/AudioRenderer.h"
#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/Effects/Utils/RenderTargetStackAllocator.h"
#include "Engine/Graphics/Effects/Utils/RenderQueueStackAllocator.h"


namespace bv { namespace nrl { 

class NRenderContext
{
private:


    Renderer *                      m_renderer;
    RenderTargetStackAllocator *    m_rtAllocator;
    RenderQueueStackAllocator *     m_renderQueueAllocator;

	const RenderTarget *			m_boundRenderTarget;

    audio::AudioRenderer *          m_audio;

public:

                                    NRenderContext		    ();
                                    ~NRenderContext		    ();

    Renderer *                      GetRenderer             () const;
    audio::AudioRenderer *          GetAudio                () const;
    RenderTargetStackAllocator *    GetRenderTargetAllocator() const;
    RenderQueueStackAllocator *     GetRenderQueueAllocator () const;

    void                            SetRenderer             ( Renderer * renderer );
    void                            SetAudio                ( audio::AudioRenderer * audio );
    void                            SetAllocator            ( RenderTargetStackAllocator * allocator );
    void                            SetRenderQueueAllocator ( RenderQueueStackAllocator * allocator );

	const RenderTarget *            GetBoundRenderTarget    () const;

    void                            Enable                  ( const RenderTarget * rt );
    void                            Disable                 ( const RenderTarget * rt );

    const RenderTarget *            DisableBoundRT          ();
    
    void                            ClearBoundRT            ( const glm::vec4 & col );

    void                            SetBoundRenderTarget    ( const RenderTarget * rt );

};

// *******************************
//
inline Renderer *                   renderer		( NRenderContext * ctx )
{
    return ctx->GetRenderer();
}

// *******************************
//
inline audio::AudioRenderer *       audio           ( NRenderContext * ctx )
{
    return ctx->GetAudio();
}

// *******************************
//
inline RenderTargetStackAllocator * allocator		( NRenderContext * ctx )
{
    return ctx->GetRenderTargetAllocator();
}

// *******************************
//
inline RenderQueueStackAllocator * queue_allocator	( NRenderContext * ctx )
{
    return ctx->GetRenderQueueAllocator();
}

// *******************************
//
inline void                         enable			( NRenderContext * ctx, const RenderTarget * rt )
{
    ctx->Enable( rt );
}

// *******************************
//
inline void                         disable			( NRenderContext * ctx, RenderTarget * rt )
{
    ctx->Disable( rt );
}

// *******************************
//
inline const RenderTarget *         disableBoundRT	( NRenderContext * ctx )
{
    return ctx->DisableBoundRT();
}

// *******************************
//
inline void                         clearBoundRT	( NRenderContext * ctx, const glm::vec4 & col )
{
    ctx->ClearBoundRT( col );
}

} // nrl
} // bv
