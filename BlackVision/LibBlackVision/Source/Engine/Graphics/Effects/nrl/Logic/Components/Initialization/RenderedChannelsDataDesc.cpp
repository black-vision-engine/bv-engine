#include "stdafx.h"

#include "RenderedChannelsDataDesc.h"


namespace bv { namespace nrl {

// **************************
//
RenderedChannelsDataDesc::RenderedChannelsDataDesc()
    : m_channelsEnabledState( (unsigned int) RenderChannelType::RCT_TOTAL )
{
    for ( auto it = m_channelsEnabledState.begin(); it != m_channelsEnabledState.end(); ++it )
    {
        *it = false;
    }
}

// **************************
//
bool        RenderedChannelsDataDesc::IsEnabled   ( RenderChannelType rct )
{
    return m_channelsEnabledState[ (unsigned int) rct ];
}

// **************************
//
void        RenderedChannelsDataDesc::SetEnabled  ( RenderChannelType rct )
{
    m_channelsEnabledState[ (unsigned int) rct ] = true;
}

// **************************
//
void        RenderedChannelsDataDesc::SetDisabled ( RenderChannelType rct )
{
    m_channelsEnabledState[ (unsigned int) rct ] = false;
}

} //nrl
} //bv
