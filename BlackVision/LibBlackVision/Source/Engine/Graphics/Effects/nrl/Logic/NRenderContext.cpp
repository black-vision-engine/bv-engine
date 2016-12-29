#include "stdafx.h"

#include "NRenderContext.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/Effects/Utils/RenderTargetStackAllocator.h"


namespace bv { namespace nrl { 


// ***************************
//
NRenderContext::NRenderContext		()
    : m_renderer( nullptr )
	, m_rtAllocator( nullptr )
{    
}

// ***************************
//
NRenderContext::~NRenderContext		()
{
}

// ***************************
//
Renderer *	NRenderContext::GetRenderer			() const
{
    return m_renderer;
}

// ***************************
//
RenderTargetStackAllocator *	NRenderContext::GetRenderTargetAllocator		() const
{
    return m_rtAllocator;
}

// ***************************
//
void                            NRenderContext::SetRenderer						( Renderer * renderer )
{
	m_renderer = renderer;
}

// ***************************
//
void                            NRenderContext::SetAllocator					( RenderTargetStackAllocator * allocator )
{
	m_rtAllocator = allocator;
}

// ***************************
//
const RenderTarget *            NRenderContext::GetBoundRenderTarget			() const
{
    return m_boundRenderTarget;
}

// ***************************
//
void                            NRenderContext::Enable							( const RenderTarget * rt )
{
    assert( rt != nullptr );

    SetBoundRenderTarget( rt );

    renderer( this )->Enable( rt );
}

// ***************************
//
void                            NRenderContext::Disable							( const RenderTarget * rt )
{
    renderer( this )->Disable( rt );
    
    SetBoundRenderTarget( nullptr );
}

// ***************************
//
const RenderTarget *            NRenderContext::DisableBoundRT					()
{
    auto res = GetBoundRenderTarget();

    if( res != nullptr )
    {
        Disable( res );
    }

    return res;
}

// ***************************
//
void                            NRenderContext::ClearBoundRT					( const glm::vec4 & col )
{
    renderer( this )->SetClearColor( col );
    renderer( this )->ClearBuffers();
}

// ***************************
//
void                            NRenderContext::SetBoundRenderTarget			( const RenderTarget * rt )
{
    m_boundRenderTarget = rt;
}

} // nrl
} // bv
