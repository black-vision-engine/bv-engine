#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/Components/Initialization/RenderedChannelsDataDesc.h"
#include "Engine/Graphics/Effects/nrl/Logic/Components/Initialization/OutputLogicDesc.h"


namespace bv { namespace nrl {

class NRenderLogicDesc
{
private:

    unsigned int                m_mainWidth;
    unsigned int                m_mainHeight;

    RenderedChannelsDataDesc    m_rcd;
    OutputLogicDesc             m_old;

public:

    NRenderLogicDesc        ();
    ~NRenderLogicDesc       ();

    RenderedChannelsDataDesc  &     AccessRenderedChannelsDataDesc  ();
    OutputLogicDesc  &              AccessOutputLogicDesc           ();

    unsigned int                    GetMainWidth                    () const;
    unsigned int                    GetMainHeight                   () const;

    void                            SetMainWidth                    ( unsigned int width );
    void                            SetMainHeight                   ( unsigned int height );

};

} //nrl
} //bv
