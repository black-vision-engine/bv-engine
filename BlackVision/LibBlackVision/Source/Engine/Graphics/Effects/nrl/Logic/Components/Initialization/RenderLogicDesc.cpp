#include "stdafx.h"

#include "RenderLogicDesc.h"


namespace bv { namespace nrl {

// **************************
//
RenderLogicDesc::RenderLogicDesc        ()
    : m_mainWidth( 1920 )               // FIXME: nrl - read it from some cpp file with statically set values
    , m_mainHeight( 1080 )              // FIXME: nrl - read it from some cpp file with statically set values
    , m_numTrackedRenderTargets( 2 )    // FIXME: nrl - read it from some cpp file with statically set values
{
}

// **************************
//
RenderLogicDesc::~RenderLogicDesc       ()
{
}

// **************************
//
RenderedChannelsDataDesc  &     RenderLogicDesc::AccessRenderedChannelsDataDesc  ()
{
    return m_rcd;
}

// **************************
//
OutputLogicDesc  &              RenderLogicDesc::AccessOutputLogicDesc           ()
{
    return m_old;
}

// **************************
//
unsigned int                    RenderLogicDesc::GetMainWidth                    () const
{
    return m_mainWidth;
}

// **************************
//
unsigned int                    RenderLogicDesc::GetMainHeight                   () const
{
    return m_mainHeight;
}

// **************************
//
unsigned int                    RenderLogicDesc::GetNumTrackedRenderTargets      () const
{
    return m_numTrackedRenderTargets;
}

// **************************
//
void                            RenderLogicDesc::SetMainWidth                    ( unsigned int width )
{
    m_mainWidth = width;
}

// **************************
//
void                            RenderLogicDesc::SetMainHeight                   ( unsigned int height )
{
    m_mainHeight = height;
}

// **************************
//
void                            RenderLogicDesc::SetNumTrackedRenderTargets      ( unsigned int numRenderTargets )
{
    m_numTrackedRenderTargets = numRenderTargets;
}

} //nrl
} //bv
