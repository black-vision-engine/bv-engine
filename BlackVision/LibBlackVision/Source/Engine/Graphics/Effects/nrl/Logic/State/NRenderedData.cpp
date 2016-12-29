#include "stdafx.h"

#include "NRenderedData.h"

#include <cassert>

#include "Engine/Graphics/Resources/RenderTarget.h"


namespace bv { namespace nrl {

// **************************
//
NRenderedData::NRenderedData   ( unsigned int numRenderTargets )
    : m_renderTargets( numRenderTargets )        
{
    for( unsigned int i = 0; i < (unsigned int) m_renderTargets.size(); ++i )
    {
        m_renderTargets[ i ] = nullptr;
    }
}

// **************************
//
NRenderedData::~NRenderedData  ()
{
}

// **************************
//
void                    NRenderedData::SetEntry                 ( unsigned int i, const RenderTarget * rt )
{
    assert( i < GetNumEntries() );

    m_renderTargets[ i ] = rt;
}

// **************************
//
const RenderTarget *    NRenderedData::GetEntry                 ( unsigned int i ) const
{
    assert( i < GetNumEntries() );

    return m_renderTargets[ i ];
}

// **************************
//
unsigned int            NRenderedData::GetNumEntries            () const
{
    return (unsigned int) m_renderTargets.size();
}

// **************************
//
const RenderTargetVec & NRenderedData::GetAllRenderTargets      () const
{
    return m_renderTargets;
}

// **************************
//
void                    NRenderedData::SetLastRenderTargetIdx   ( unsigned int idx )
{
    m_lastRenderTargetIdx = idx;
}

// **************************
//
unsigned int            NRenderedData::GetLastRenderTargetIdx   () const
{
    return m_lastRenderTargetIdx;
}

} //nrl
} //bv
