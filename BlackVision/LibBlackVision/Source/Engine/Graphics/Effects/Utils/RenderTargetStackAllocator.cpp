#include "stdafx.h"

#include "RenderTargetStackAllocator.h"

#include <cassert>

#include "Engine/Graphics/Renderers/Renderer.h"

#include "Engine/Graphics/Effects/Utils/RenderTargetFactory.h"


//Use the SRP Luke


#include "Memory/MemoryLeaks.h"



namespace bv {


// *********************************
//
RenderTargetStackAllocator::RenderTargetStackAllocator      ( unsigned int width, unsigned int height, TextureFormat fmt )
    : m_width( width )
    , m_height( height ) 
    , m_fmt( fmt )
    , m_topIdx( -1 )
{
}

// *********************************
//
RenderTargetStackAllocator::~RenderTargetStackAllocator     ()
{
    for( auto rt : m_allocatedRenderTargets )
    {
        delete rt;
    }
}

// *********************************
//
RenderTarget *      RenderTargetStackAllocator::Allocate    ( RenderTarget::RTSemantic semantic )
{
    m_topIdx++;

    if( m_topIdx >=  (int) m_allocatedRenderTargets.size() )
    {
        auto rt = RenderTargetFactory::CreateRenderTarget( semantic, m_width, m_height, m_fmt );
        m_allocatedRenderTargets.push_back( rt );
    }

    assert( m_topIdx < (int) m_allocatedRenderTargets.size() );

    //FIXME: we assume that usage patterns are exactly the same between consecutive frames which may not hold in a more general case
    //FIXME: in such case additional means of RT management should be used (first of all m_topActiveRenderTargetIdx must be removed from GPU - the hard part
    //FIXME: and the it has to be removed from m_allocatedRenderTargets and replaced with the new one created according to required semantics).
    assert( m_allocatedRenderTargets[ m_topIdx ]->Semantic() == semantic );

    return m_allocatedRenderTargets[ m_topIdx ];
}

// *********************************
//
RenderTarget *      RenderTargetStackAllocator::Top         ()
{
    assert( m_topIdx >= 0 );

    if ( m_topIdx < 0 )
    {
        return nullptr;
    }

    return m_allocatedRenderTargets[ m_topIdx ];
}

// *********************************
//
bool                RenderTargetStackAllocator::Free        ()
{
    assert( m_topIdx >= 0 );

    if ( m_topIdx >= 0 )
    {
        --m_topIdx;

        return true;
    }

    return false;
}

// *********************************
//
bool                RenderTargetStackAllocator::Free                        ( unsigned int num )
{
    auto fsc = true;

    for( unsigned int i = 0; i < num; ++i )
    {
        fsc &= Free();
    }

    return fsc;
}

// *********************************
//
unsigned int        RenderTargetStackAllocator::GetTopIndex                 () const
{
    return m_topIdx;
}

// *********************************
//
RenderTarget *      RenderTargetStackAllocator::CreateRenderTarget          ( RenderTarget::RTSemantic semantic )
{
    return RenderTargetFactory::CreateRenderTarget( semantic, m_width, m_height, m_fmt );
}

// *********************************
//
RenderTarget *      RenderTargetStackAllocator::CreateCustomRenderTarget    ( unsigned int width, unsigned int height, RenderTarget::RTSemantic semantic )
{
    return RenderTargetFactory::CreateRenderTarget( semantic, width, height, m_fmt );
}

// *********************************
//
RenderTarget *      RenderTargetStackAllocator::CreateCustomRenderTarget    ( UInt32 levels, RenderTarget::RTSemantic semantic )
{
	return RenderTargetFactory::CreateRenderTarget( semantic, m_width, m_height, m_fmt, levels );
}

} //bv
