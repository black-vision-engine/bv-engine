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

public:

        RenderLogicContext  ( Renderer * renderer, RenderTargetStackAllocator * rtStackAllocator, RenderLogic * renderLogic );
        ~RenderLogicContext ();

    Renderer *                      GetRenderer             () const;
    RenderTargetStackAllocator *    GetRenderTargetAllocator() const;
    RenderLogic *                   GetRenderLogic          () const;

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

} // bv
