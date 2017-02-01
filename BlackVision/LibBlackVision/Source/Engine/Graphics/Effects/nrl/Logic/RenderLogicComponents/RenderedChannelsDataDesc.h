#pragma once

#include <vector>

#include "Engine/Graphics/Effects/nrl/Logic/RenderLogicComponents/RenderedChannelsData.h"

namespace bv { namespace nrl {

class RenderedChannelsDataDesc
{
private:

    std::vector< bool >             m_channelsEnabledState;

public:

                RenderedChannelsDataDesc();

    bool        IsEnabled               ( RenderChannelType rct );

    void        SetEnabled              ( RenderChannelType rct );
    void        SetDisabled             ( RenderChannelType rct );

};

} //nrl
} //bv
