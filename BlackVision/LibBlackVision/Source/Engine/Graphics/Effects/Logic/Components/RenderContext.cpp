#include "stdafx.h"

#include "RenderContext.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/Effects/Utils/RenderTargetStackAllocator.h"


namespace bv {  


// ***************************
//
RenderContext::RenderContext		()
    : m_renderer( nullptr )
    , m_audio( nullptr )
	, m_rtAllocator( nullptr )
    , m_renderQueueAllocator( nullptr )
{    
}

// ***************************
//
RenderContext::~RenderContext		()
{
}

// ***************************
//
Renderer *	RenderContext::GetRenderer			() const
{
    return m_renderer;
}

// ***************************
//
audio::AudioRenderer *          RenderContext::GetAudio                        () const
{
    return m_audio;
}

// ***************************
//
RenderTargetStackAllocator *	RenderContext::GetRenderTargetAllocator		() const
{
    return m_rtAllocator;
}

// ***************************
//
RenderQueueStackAllocator *     RenderContext::GetRenderQueueAllocator         () const
{
    return m_renderQueueAllocator;
}

// ***************************
//
void                            RenderContext::SetRenderer						( Renderer * renderer )
{
	m_renderer = renderer;
}

// ***************************
//
void                            RenderContext::SetAudio                        ( audio::AudioRenderer * audio )
{
    m_audio = audio;
}

// ***************************
//
void                            RenderContext::SetAllocator					( RenderTargetStackAllocator * allocator )
{
	m_rtAllocator = allocator;
}

// ***************************
//
void                            RenderContext::SetRenderQueueAllocator         ( RenderQueueStackAllocator * allocator )
{
    m_renderQueueAllocator = allocator;
}

// ***************************
//
const RenderTarget *            RenderContext::GetBoundRenderTarget			() const
{
    return m_boundRenderTarget;
}

// ***************************
//
void                            RenderContext::Enable							( const RenderTarget * rt )
{
    assert( rt != nullptr );

    SetBoundRenderTarget( rt );

    renderer( this )->Enable( rt );
}

// ***************************
//
void                            RenderContext::Disable							( const RenderTarget * rt )
{
    renderer( this )->Disable( rt );
    
    SetBoundRenderTarget( nullptr );
}

// ***************************
//
const RenderTarget *            RenderContext::DisableBoundRT					()
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
void                            RenderContext::ClearBoundRT					( const glm::vec4 & col )
{
    renderer( this )->SetClearColor( col );
    renderer( this )->ClearBuffers();
}

// ***************************
//
void                            RenderContext::SetBoundRenderTarget			( const RenderTarget * rt )
{
    m_boundRenderTarget = rt;
}


} // bv
