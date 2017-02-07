#include "stdafx.h"

#include "OutputInstance.h"


namespace bv { namespace nrl {

// *********************************
//
OutputInstance::OutputInstance                          ( unsigned int width, unsigned int height, FrameDataHandler * handler )
    : m_state( width, height )
    , m_frameDataHandler( handler )
    , m_isEnabled( false )
{
}

// *********************************
//
OutputInstance::~OutputInstance                         ()
{
    delete m_frameDataHandler;
}

// *********************************
//
void    OutputInstance::ProcessFrameData                ( NRenderContext * ctx, RenderedChannelsData * input )
{
    if( IsEnabled() )
    {
        SyncHandlerState();

        auto rct = m_state.GetActiveRenderChannel();
        assert( input->IsActive( rct ) && input->ContainsValidData( rct ) );
    
        m_frameDataHandler->HandleFrameData( ctx, input->GetActiveRenderChannel( rct ) );
    }
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
