#pragma once

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/Effects/Utils/RenderTargetStackAllocator.h"


namespace bv { namespace nrl { 

class NRenderContext
{
private:

    Renderer *                      m_renderer;
    RenderTargetStackAllocator *    m_rtAllocator;

	const RenderTarget *			m_boundRenderTarget;

public:

                                    NRenderContext		    ();
                                    ~NRenderContext		    ();

    Renderer *                      GetRenderer             () const;
    RenderTargetStackAllocator *    GetRenderTargetAllocator() const;

    void                            SetRenderer             ( Renderer * renderer );
    void                            SetAllocator            ( RenderTargetStackAllocator * allocator );

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
inline RenderTargetStackAllocator * allocator		( NRenderContext * ctx )
{
    return ctx->GetRenderTargetAllocator();
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
