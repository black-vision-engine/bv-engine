#include "stdafx.h"
#include "VideoInputHandler.h"

#include "Engine/Events/EventManager.h"

#include "Engine/Events/InnerEvents/InputSlots/FirstSlotRefEvent.h"
#include "Engine/Events/InnerEvents/InputSlots/AllSlotRefsRemovedEvent.h"


namespace bv
{

// ***********************
//
VideoInputHandler::VideoInputHandler( videocards::VideoCardManager * videoCardMan, InputSlotsPtr slots )
    :   m_videoCardManager( videoCardMan )
    ,   m_inputSlots( std::make_shared< VideoInputSlots >( slots ) )
    ,   m_audioRenderer( nullptr )
    ,   m_renderer( nullptr )
{
    assert( videoCardMan );
    assert( slots );
}

// ***********************
//
VideoInputHandler::~VideoInputHandler()
{
    // Call as not virtual function. Virtuals in destructor not allowed.
    VideoInputHandler::UnregisterInputs();
}

// ***********************
//
void            VideoInputHandler::UnregisterInputs ()
{
    if( m_renderer && m_audioRenderer )
    {
        RenderContext ctx;
        ctx.SetRenderer( m_renderer );
        ctx.SetAudio( m_audioRenderer );

        m_inputSlots->UnregisterAllChannels( &ctx );
    }
}

// ***********************
//
void            VideoInputHandler::ProcessInputs    ( RenderContext * )
{
    auto inputFrameData = m_videoCardManager->QueryVideoInput();
    ProcessFrameData( inputFrameData );
}

// ***********************
//
void            VideoInputHandler::RegisterInputs   ( RenderContext * ctx, InputSlotsPtr inputSlots )
{
    m_renderer = ctx->GetRenderer();
    m_audioRenderer = ctx->GetAudio();

    auto inputDescs = m_videoCardManager->GetInputChannelsDescs();
    RegisterInputs( inputDescs );

    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &VideoInputHandler::OnAllSlotReferencesRemoved ), AllSlotRefsRemovedEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &VideoInputHandler::OnFirstSlotReference ), FirstSlotRefEvent::Type() );
}

// ***********************
//
void            VideoInputHandler::RegisterInputs   ( const videocards::InputChannelsDescsVec & channelsDesc )
{
    for( auto & desc : channelsDesc )
    {
        // Don't check result. Error message is logged internally.
        m_inputSlots->RegisterVideoInputChannel( desc );

        // Channel will be enabled on first reference to input slot.
        DisableChannel( desc.GetCardID(), desc.GetInputID() );
    }

    GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &VideoInputHandler::OnAllSlotReferencesRemoved ), AllSlotRefsRemovedEvent::Type() );
    GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &VideoInputHandler::OnFirstSlotReference ), FirstSlotRefEvent::Type() );
}

// ***********************
//
void            VideoInputHandler::ProcessFrameData ( videocards::VideoInputFrameData & frameData )
{
    for( auto & singleFrame : frameData.Frames )
    {
        // Note that it's fully legal to get nullptr here. Video input could not
        // provide any frame. In this case input slots remains with previous content.
        if( singleFrame.FrameData )
        {
            m_inputSlots->UpdateVideoInput( singleFrame.InputID, singleFrame.FrameData );
        }
    }
}


// ========================================================================= //
// Slot references optimization.
// ========================================================================= //


// ***********************
//
void            VideoInputHandler::OnAllSlotReferencesRemoved       ( IEventPtr evt )
{
    if( evt->GetEventType() == AllSlotRefsRemovedEvent::Type() )
    {
        auto typedEvent = std::static_pointer_cast< AllSlotRefsRemovedEvent >( evt );

        auto channelDesc = m_inputSlots->GetVideoCardFromSlot( typedEvent->Index );

        if( channelDesc.IsValid() )
            DisableChannel( channelDesc.GetVal().GetCardID(), channelDesc.GetVal().GetInputID() );
    }
}

// ***********************
//
void            VideoInputHandler::OnFirstSlotReference             ( IEventPtr evt )
{
    if( evt->GetEventType() == FirstSlotRefEvent::Type() )
    {
        auto typedEvent = std::static_pointer_cast< FirstSlotRefEvent >( evt );

        auto channelDesc = m_inputSlots->GetVideoCardFromSlot( typedEvent->Index );

        if( channelDesc.IsValid() )
            EnableChannel( channelDesc.GetVal().GetCardID(), channelDesc.GetVal().GetInputID() );
    }
}

// ***********************
//
void            VideoInputHandler::EnableChannel                    ( videocards::VideoCardID cardID, videocards::VideoInputID inputID )
{
    auto videoCard = FindVideoCard( cardID );
    videoCard->SetVideoInput( inputID, true );
}

// ***********************
//
void            VideoInputHandler::DisableChannel                   ( videocards::VideoCardID cardID, videocards::VideoInputID inputID )
{
    auto videoCard = FindVideoCard( cardID );
    videoCard->SetVideoInput( inputID, false );
}

// ***********************
//
videocards::IVideoCardPtr       VideoInputHandler::FindVideoCard    ( videocards::VideoCardID cardID ) const
{
    int cardIdx = 0;
    while( auto videoCard = m_videoCardManager->GetVideoCard( cardIdx ) )
    {
        if( videoCard->GetVideoCardID() == cardID )
            return videoCard;

        cardIdx++;
    }

    return nullptr;
}


}	// bv


