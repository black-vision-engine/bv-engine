#include "stdafx.h"

#include "Output.h"


namespace bv { namespace nrl {

// *********************************
//
Output::Output                                              ( unsigned int width, unsigned int height )
    : m_state( width, height )
    , m_isEnabled( false )
{
}

// *********************************
//
Instance::~Instance                                         ()
{
}

// *********************************
//
bool    Output::IsEnabled                                   () const
{
    return m_isEnabled;
}

// *********************************
//
void    Output::Enable                                      ()
{
    m_isEnabled = true;
}

// *********************************
//
void    Output::Disable                                     ()
{
    m_isEnabled = false;
}

// *********************************
//
NOutputState &  Output::AccessOutputState                   ()
{
    return m_state;
}

// *********************************
//
RenderChannelType   Output::GetActiveRenderChannel          () const
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
    return m_state.GetChannelMapping();
}

// *********************************
//
glm::vec4           OutputInstance::GetChannelMask          () const
{
    return m_state.GetChannelMask();
}

} //nrl
} //bv
