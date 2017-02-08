#include "stdafx.h"

#include "NRenderLogicDesc.h"


namespace bv { namespace nrl {

// **************************
//
NRenderLogicDesc::NRenderLogicDesc        ()
    : m_mainWidth( 1920 )               // FIXME: nrl - read it from some cpp file with statically set values
    , m_mainHeight( 1080 )              // FIXME: nrl - read it from some cpp file with statically set values
    , m_numTrackedRenderTargets( 2 )    // FIXME: nrl - read it from some cpp file with statically set values
{
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
unsigned int                    NRenderLogicDesc::GetNumTrackedRenderTargets      () const
{
    return m_numTrackedRenderTargets;
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

// **************************
//
void                            NRenderLogicDesc::SetNumTrackedRenderTargets      ( unsigned int numRenderTargets )
{
    m_numTrackedRenderTargets = numRenderTargets;
}

} //nrl
} //bv
