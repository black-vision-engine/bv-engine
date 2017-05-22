#include "stdafx.h"

#include "OutputInstance.h"

#include "Engine/Graphics/Effects/Logic/Components/RenderedChannelsData.h"
#include "Engine/Graphics/Effects/Logic/OutputRendering/Impl/FrameDataHandlers/FrameDataHandler.h"


namespace bv { 

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
void    OutputInstance::ProcessFrameData                ( RenderContext * ctx, RenderedChannelsData * input )
{
    if( IsEnabled() )
    {
        SyncHandlerState();

        auto rct = m_state.GetActiveRenderChannelType();
        assert( input->IsActive( rct ) && input->ContainsValidData( rct ) );
 
        auto renderChannel = input->GetRenderChannel( rct );

        m_frameDataHandler->HandleFrameData( m_state, ctx,  renderChannel );
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
OutputState &  OutputInstance::AccessOutputState       ()
{
    return m_state;
}

// *********************************
//
void            OutputInstance::SyncHandlerState        ()
{
    auto state      = m_frameDataHandler->GetInternalFSEState();
    
    auto mappingVal = state->GetValueAt( 0 ); assert( mappingVal->GetName() == "channelMapping" );
    auto maskVal    = state->GetValueAt( 1 ); assert( maskVal->GetName() == "channelMask" );

    auto mapping    = m_state.GetChannelMapping();
    auto mask       = m_state.GetChannelMask();

    QueryTypedValue< ValueIntPtr >( mappingVal )->SetValue( mapping );
    QueryTypedValue< ValueVec4Ptr >( maskVal )->SetValue( mask );
}


} //bv
