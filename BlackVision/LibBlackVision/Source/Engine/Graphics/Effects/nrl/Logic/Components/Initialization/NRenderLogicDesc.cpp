#include "stdafx.h"

#include "NRenderLogicDesc.h"


namespace bv { namespace nrl {

// **************************
//
NRenderLogicDesc::NRenderLogicDesc        ()
{
    /// FIXME: implement basic descs initialization here
}

// **************************
//
NRenderLogicDesc::~NRenderLogicDesc       ()
{
}

// **************************
//
RenderedChannelsDataDesc  &     NRenderLogicDesc::AccessRenderedChannelsDataDesc  ()
{
    return m_rcd;
}

// **************************
//
OutputLogicDesc  &              NRenderLogicDesc::AccessOutputLogicDesc           ()
{
    return m_old;
}

} //nrl
} //bv
