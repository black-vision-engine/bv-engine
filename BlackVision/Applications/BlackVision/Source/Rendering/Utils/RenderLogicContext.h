#pragma once

#include "Engine/Graphics/Renderers\Renderer.h"
#include "Rendering/Utils/RenderTargetStackAllocator.h"
#include "Rendering/Logic/RenderLogic.h"


namespace bv {

class RenderLogicContext
{
private:

    Renderer *                      m_renderer;
    RenderTargetStackAllocator *    m_rtAllocator;
    RenderLogic *                   m_renderLogic;
    RenderTarget *                  m_boundRenderTarget;

public:

        RenderLogicContext  ( Renderer * renderer, RenderTargetStackAllocator * rtStackAllocator, RenderLogic * renderLogic );
        ~RenderLogicContext ();

    Renderer *                      GetRenderer             () const;
    RenderTargetStackAllocator *    GetRenderTargetAllocator() const;
    RenderLogic *                   GetRenderLogic          () const;
    RenderTarget *                  GetBoundRenderTarget    () const;

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
