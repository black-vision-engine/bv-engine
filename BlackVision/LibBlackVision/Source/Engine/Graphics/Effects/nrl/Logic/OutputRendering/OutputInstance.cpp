#include "stdafx.h"

#include "OutputInstance.h"


namespace bv { namespace nrl {

// *********************************
//
OutputInstance::OutputInstance                              ( unsigned int width, unsigned int height )
    : m_state( width, height )
    , m_isEnabled( false )
{
}

// *********************************
//
OutputInstance::~OutputInstance     ()
{
}

// *********************************
//
bool    OutputInstance::IsEnabled                           () const
{
    return m_isEnabled;
}

// *********************************
//
void    OutputInstance::Enable                              ()
{
    m_isEnabled = true;
}

// *********************************
//
void    OutputInstance::Disable                             ()
{
    m_isEnabled = false;
}

// *********************************
//
NOutputState &  OutputInstance::AccessOutputState           ()
{
    return m_state;
}

// *********************************
//
RenderChannelType   OutputInstance::GetActiveRenderChannel  () const
{
    return m_state.GetActiveRenderChannel();
}

// *********************************
//
unsigned int        OutputInstance::GetWidth                () const
{
    return m_state.GetWidth();
}

// *********************************
//
unsigned int        OutputInstance::GetHeight               () const
{
    return m_state.GetHeight();
}

// *********************************
//
unsigned int        OutputInstance::GetChannelMapping       () const
{
    return m_state.GetChannelMask();
}

} //nrl
} //bv
