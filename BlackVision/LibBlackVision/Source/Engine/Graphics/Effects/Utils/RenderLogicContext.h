#pragma once

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/Effects/Utils/RenderTargetStackAllocator.h"
#include "Engine/Graphics/Effects/Utils/RenderQueueStackAllocator.h"
#include "Engine/Graphics/Effects/Logic/RenderLogic.h"
#include "Engine/Audio/AudioRenderer.h"


namespace bv {

class RenderLogicContext
{
private:

    Renderer *                      m_renderer;
    RenderTargetStackAllocator *    m_rtAllocator;
    RenderQueueStackAllocator *     m_renderQueueAllocator;
    RenderLogic *                   m_renderLogic;
    RenderTarget *                  m_boundRenderTarget;
    
    audio::AudioRenderer *          m_audioRenderer;

public:

        RenderLogicContext  ( Renderer * renderer, RenderTargetStackAllocator * rtStackAllocator, RenderQueueStackAllocator * rqStackAllocator, RenderLogic * renderLogic, audio::AudioRenderer * audioRenderer );
        ~RenderLogicContext ();

    Renderer *                      GetRenderer             () const;
    RenderTargetStackAllocator *    GetRenderTargetAllocator() const;
    RenderQueueStackAllocator *     GetRenderQueueAllocator () const;
    RenderLogic *                   GetRenderLogic          () const;
    RenderTarget *                  GetBoundRenderTarget    () const;

    audio::AudioRenderer *          GetAudioRenderer        () const;

    void                            SetBoundRenderTarget    ( RenderTarget * target );

};

// *******************************
//
inline Renderer *                   renderer( RenderLogicContext * ctx )
{
    return ctx->GetRenderer();
}

// *******************************
//
inline RenderTargetStackAllocator * allocator( RenderLogicContext * ctx )
{
    return ctx->GetRenderTargetAllocator();
}

// *******************************
//
inline RenderLogic *                logic   ( RenderLogicContext * ctx )
{
    return ctx->GetRenderLogic();
}

// *******************************
//
inline audio::AudioRenderer *       audioRenderer   ( RenderLogicContext * ctx )
{
    return ctx->GetAudioRenderer();
}

// *******************************
//
inline void                         enable  ( RenderLogicContext * ctx, RenderTarget * rt )
{
    ctx->SetBoundRenderTarget( rt );    
    renderer( ctx )->Enable( rt );
}

// *******************************
//
inline void                         disable ( RenderLogicContext * ctx, RenderTarget * rt )
{
    renderer( ctx )->Disable( rt );
    ctx->SetBoundRenderTarget( nullptr );
}

// *******************************
//
inline RenderTarget *               disableBoundRT ( RenderLogicContext * ctx )
{
    auto res = ctx->GetBoundRenderTarget();

    if( res != nullptr )
    {
        renderer( ctx )->Disable( res );
        ctx->SetBoundRenderTarget( nullptr );
    }

    return res;
}

// *******************************
//
inline void                         clearBoundRT ( RenderLogicContext * ctx, const glm::vec4 & col )
{
    renderer( ctx )->SetClearColor( col );
    renderer( ctx )->ClearBuffers();
}

} // bv
