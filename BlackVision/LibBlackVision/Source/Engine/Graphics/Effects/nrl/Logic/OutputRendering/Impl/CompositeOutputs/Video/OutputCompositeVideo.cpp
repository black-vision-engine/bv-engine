#include "stdafx.h"

#include "OutputCompositeVideo.h"

#include "Engine/Graphics/Effects/nrl/Logic/Components/RenderedChannelsData.h"
#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Impl/FrameDataHandlers/FrameDataHandler.h"

// FIXME: make sure that after each frame all VideoInputChannels are invalidated (so that no cached textures are used next frame)

namespace bv { namespace nrl {

// *********************************
//
OutputCompositeVideo::OutputCompositeVideo                          ( unsigned int width, unsigned int height, FrameDataHandler * handler )
    : m_state( width, height )
    , m_frameDataHandler( handler )
    , m_isEnabled( false )
{
}

// *********************************
//
OutputCompositeVideo::~OutputCompositeVideo                         ()
{
    delete m_frameDataHandler;
}

// *********************************
//
void    OutputCompositeVideo::ProcessFrameData                ( NRenderContext * ctx, RenderedChannelsData * input )
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
bool    OutputCompositeVideo::IsEnabled                       () const
{
    return m_isEnabled;
}

// *********************************
//
void    OutputCompositeVideo::Enable                          ()
{
    m_isEnabled = true;
}

// *********************************
//
void    OutputCompositeVideo::Disable                         ()
{
    m_isEnabled = false;
}

// *********************************
//
NOutputState &  OutputCompositeVideo::AccessOutputState       ()
{
    return m_state;
}

// *********************************
//
void            OutputCompositeVideo::SyncHandlerState        ()
{
    auto state      = m_frameDataHandler->GetInternalFSEState();
    
    auto mappingVal = state->GetValueAt( 0 ); assert( mappingVal->GetName() == "channelMapping" );
    auto maskVal    = state->GetValueAt( 1 ); assert( maskVal->GetName() == "channelMask" );

    auto mapping    = m_state.GetChannelMapping();
    auto mask       = m_state.GetChannelMask();

    QueryTypedValue< ValueIntPtr >( mappingVal )->SetValue( mapping );
    QueryTypedValue< ValueVec4Ptr >( maskVal )->SetValue( mask );
}

} //nrl
} //bv

