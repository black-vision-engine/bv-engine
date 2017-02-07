#include "stdafx.h"

#include "OutputInstance.h"

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Impl/FrameDataHandlers/FrameDataHandler.h"


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
bool    OutputInstance::IsEnabled                       () const
{
    return m_isEnabled;
}

// *********************************
//
void    OutputInstance::Enable                          ()
{
    m_isEnabled = true;
}

// *********************************
//
void    OutputInstance::Disable                         ()
{
    m_isEnabled = false;
}

// *********************************
//
NOutputState &  OutputInstance::AccessOutputState       ()
{
    return m_state;
}

// *********************************
//
void            OutputInstance::SyncHandlerState        ()
{
}

} //nrl
} //bv
