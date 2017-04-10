#include "stdafx.h"

#include "RenderedData.h"

#include <cassert>

#include "Engine/Graphics/Resources/RenderTarget.h"


namespace bv { namespace nrl {

// **************************
//
RenderedData::RenderedData   ( unsigned int numRenderTargets )
    : m_renderTargets( numRenderTargets )        
{
    for( unsigned int i = 0; i < (unsigned int) m_renderTargets.size(); ++i )
    {
        m_renderTargets[ i ] = nullptr;
    }
}

// **************************
//
RenderedData::~RenderedData  ()
{
}

// **************************
//
void                    RenderedData::SetEntry                 ( unsigned int i, const RenderTarget * rt )
{
    assert( i < GetNumEntries() );

    m_renderTargets[ i ] = rt;
}

// **************************
//
const RenderTarget *    RenderedData::GetEntry                 ( unsigned int i ) const
{
    assert( i < GetNumEntries() );

    return m_renderTargets[ i ];
}

// **************************
//
unsigned int            RenderedData::GetNumEntries            () const
{
    return (unsigned int) m_renderTargets.size();
}

// **************************
//
const RenderTargetVec & RenderedData::GetAllRenderTargets      () const
{
    return m_renderTargets;
}

// **************************
//
void                    RenderedData::SetLastRenderTargetIdx   ( unsigned int idx )
{
    m_lastRenderTargetIdx = idx;
}

// **************************
//
unsigned int            RenderedData::GetLastRenderTargetIdx   () const
{
    return m_lastRenderTargetIdx;
}

} //nrl
} //bv
