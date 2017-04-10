#pragma once

#include <vector>

#include "Engine/Graphics/Effects/Logic/Components/RenderChannel.h"


namespace bv { 

class RenderedChannelsDataDesc
{
private:

    std::vector< bool >             m_channelsEnabledState;

    unsigned int                    m_numTrackedRenderTargets;

public:

                    RenderedChannelsDataDesc    ();

    bool            IsEnabled                   ( RenderChannelType rct ) const;

    void            SetEnabled                  ( RenderChannelType rct );
    void            SetDisabled                 ( RenderChannelType rct );

    unsigned int    GetNumTrackedRenderTargets  () const;
    void            SetNumTrackedRenderTargets  ( unsigned int numTrackedRenderTargets );

};


} //bv
