#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/Components/Initialization/RenderedChannelsDataDesc.h"
#include "Engine/Graphics/Effects/nrl/Logic/Components/Initialization/OutputLogicDesc.h"


namespace bv { namespace nrl {

class NRenderLogicDesc
{
private:

    RenderedChannelsDataDesc    m_rcd;
    OutputLogicDesc             m_old;

public:

    NRenderLogicDesc        ();
    ~NRenderLogicDesc       ();

    RenderedChannelsDataDesc  &     AccessRenderedChannelsDataDesc  ();
    OutputLogicDesc  &              AccessOutputLogicDesc           ();

};

} //nrl
} //bv
