    #include "stdafx.h"

#include "RenderChannel.h"

#include <cassert>

#include "Engine/Graphics/Effects/Utils/RenderTargetStackAllocator.h"


namespace bv { namespace nrl {

// **************************
//
RenderChannel::RenderChannel                                    ( RenderTargetStackAllocator * allocator, unsigned int numTrackedRenderTargets )
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
RenderChannel::~RenderChannel                                   ()
{
    for( auto rt : m_renderTargets )
    {
        delete rt;
    }
}

// **************************
//
unsigned int                RenderChannel::GetNumRenderTargets        () const
{
    return (unsigned int) m_renderTargets.size();
}

// **************************
//
const bv::RenderTarget *    RenderChannel::GetActiveRenderTarget      () const
{
    return GetRenderTarget( 0 );
}

// **************************
//
const bv::RenderTarget * RenderChannel::GetPreviousRenderTarget        () const
{
    return GetRenderTarget( -1 );
}

// **************************
//
const RenderTarget *    RenderChannel::GetRenderTarget                ( int reversedIdx ) const
{
    assert( (-reversedIdx) < (int) m_renderTargets.size() );

    int idx = (int) m_renderTargets.size() + reversedIdx - 1;

    return m_renderTargets[ ( idx + m_activeRenderTargetIdx ) % m_renderTargets.size() ];
}

// **************************
//
void                    RenderChannel::UpdateActiveRenderTargetIdx    ()
{
    m_activeRenderTargetIdx = ( m_activeRenderTargetIdx + 1 ) % GetNumRenderTargets();
}

// **************************
//
bool                    RenderChannel::IsActive                       () const
{
    return m_isActive;
}

// **************************
//
void                    RenderChannel::SetActiveFlag                  ( bool isActive )
{
    m_isActive = isActive;
}

} //bv
} //nrl
