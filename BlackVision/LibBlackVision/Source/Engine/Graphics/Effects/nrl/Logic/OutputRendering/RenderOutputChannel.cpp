#include "stdafx.h"

#include "RenderOutputChannel.h"

#include <cassert>

#include "Engine/Graphics/Effects/Utils/RenderTargetStackAllocator.h"


namespace bv { namespace nrl {

// **************************
//
RenderOutputChannel::RenderOutputChannel                                    ( RenderTargetStackAllocator * allocator, unsigned int numTrackedRenderTargets )
    : m_activeRenderTargetIdx( 0 )
    , m_isActive( false )
{
    for( unsigned int i = 0; i < numTrackedRenderTargets; ++i )
    {
        auto rt = allocator->CreateRenderTarget( RenderTarget::RTSemantic::S_DRAW_ONLY );

        m_renderTargets.push_back( rt );
    }
}

// **************************
//
RenderOutputChannel::~RenderOutputChannel                                   ()
{
    for( auto rt : m_renderTargets )
    {
        delete rt;
    }
}

// **************************
//
unsigned int                RenderOutputChannel::GetNumRenderTargets        () const
{
    return m_renderTargets.size();
}

// **************************
//
const bv::RenderTarget *    RenderOutputChannel::GetActiveRenderTarget      () const
{
    return GetRenderTarget( 0 );
}

// **************************
//
const bv::RenderTarget * RenderOutputChannel::GetPreviousRenderTarget        () const
{
    return GetRenderTarget( -1 );
}

// **************************
//
const RenderTarget *    RenderOutputChannel::GetRenderTarget                ( int reversedIdx ) const
{
    assert( (-reversedIdx) < (int) m_renderTargets.size() );

    int idx = (int) m_renderTargets.size() + reversedIdx - 1;

    return m_renderTargets[ ( idx + m_activeRenderTargetIdx ) % m_renderTargets.size() ];
}

// **************************
//
unsigned int            RenderOutputChannel::GetNumRenderTargets            () const
{
    return ( unsigned int ) m_renderTargets.size();
}

// **************************
//
void                    RenderOutputChannel::UpdateActiveRenderTargetIdx    ()
{
    m_activeRenderTargetIdx = ( m_activeRenderTargetIdx + 1 ) % GetNumRenderTargets();
}

// **************************
//
bool                    RenderOutputChannel::IsActive                       () const
{
    return m_isActive;
}

// **************************
//
void                    RenderOutputChannel::SetActiveFlag                  ( bool isActive )
{
    m_isActive = true;
}

} //bv
} //nrl
