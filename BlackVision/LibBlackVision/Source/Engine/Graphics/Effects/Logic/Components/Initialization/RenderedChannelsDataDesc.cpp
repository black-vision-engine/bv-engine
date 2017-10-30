#include "stdafx.h"

#include "RenderedChannelsDataDesc.h"


namespace bv { 

// **************************
//
RenderedChannelsDataDesc::RenderedChannelsDataDesc()
    : m_channelsEnabledState( (unsigned int) RenderChannelType::RCT_TOTAL )
    , m_numTrackedRenderTargets( 2 ) // FIXME: nrl - this default value may be read from some other place
{
    for ( auto it = m_channelsEnabledState.begin(); it != m_channelsEnabledState.end(); ++it )
    {
        *it = false;
    }

    // Enable first channel by default.
    m_channelsEnabledState[ 0 ] = true;
}

// **************************
//
bool            RenderedChannelsDataDesc::IsEnabled   ( RenderChannelType rct ) const
{
    return m_channelsEnabledState[ (unsigned int) rct ];
}

// **************************
//
void            RenderedChannelsDataDesc::SetEnabled  ( RenderChannelType rct )
{
    m_channelsEnabledState[ (unsigned int) rct ] = true;
}

// **************************
//
void            RenderedChannelsDataDesc::SetDisabled ( RenderChannelType rct )
{
    m_channelsEnabledState[ (unsigned int) rct ] = false;
}

// **************************
//
unsigned int    RenderedChannelsDataDesc::GetNumTrackedRenderTargets  () const
{
    return m_numTrackedRenderTargets;
}

// **************************
//
void            RenderedChannelsDataDesc::SetNumTrackedRenderTargets  ( unsigned int numTrackedRenderTargets )
{
    m_numTrackedRenderTargets = numTrackedRenderTargets;
}


} //bv
