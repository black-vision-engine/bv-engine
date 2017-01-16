#include "stdafx.h"

#include "OutputInstance.h"


namespace bv { namespace nrl {

// *********************************
//
OutputInstance::OutputInstance      ()
    : m_isEnabled( false )
{
}

// *********************************
//
OutputInstance::~OutputInstance     ()
{
}

// *********************************
//
bool    OutputInstance::IsEnabled   () const
{
    return m_isEnabled;
}

// *********************************
//
void    OutputInstance::Enable      ()
{
    m_isEnabled = true;
}

// *********************************
//
void    OutputInstance::Disable     ()
{
    m_isEnabled = false;
}

} //nrl
} //bv
