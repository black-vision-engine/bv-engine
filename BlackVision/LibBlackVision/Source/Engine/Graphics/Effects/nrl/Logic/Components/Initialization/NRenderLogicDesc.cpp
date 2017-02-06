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

// **************************
//
unsigned int                    NRenderLogicDesc::GetMainWidth                    () const
{
    return m_mainWidth;
}

// **************************
//
unsigned int                    NRenderLogicDesc::GetMainHeight                   () const
{
    return m_mainHeight;
}

// **************************
//
void                            NRenderLogicDesc::SetMainWidth                    ( unsigned int width )
{
    m_mainWidth = width;
}

// **************************
//
void                            NRenderLogicDesc::SetMainHeight                   ( unsigned int height )
{
    m_mainHeight = height;
}

} //nrl
} //bv
