#include "stdafxBVApp.h"

#include "VideoCardEventsHandlers.h"

#include "BVAppLogic.h"
#include "VideoCardManager.h"
#include "Services/BVServiceProvider.h"


namespace bv {

// ***********************
//
VideoCardEventsHandlers::VideoCardEventsHandlers    ( BVAppLogic * appLogic )
    :   m_appLogic( appLogic )
{
}

// ***********************
//
VideoCardEventsHandlers::~VideoCardEventsHandlers   ()
{
}

// ***********************
//
void		VideoCardEventsHandlers::EventHandler   ( IEventPtr evt )
{
    if( evt->GetEventType() == VideoCardEvent::Type() )
    {
		auto videoEvent = std::static_pointer_cast< VideoCardEvent >( evt );
        auto videocardID = videoEvent->VideoCardID;
        auto command = videoEvent->VideoCommand;

        auto videoCardManager = BVServiceProvider::GetInstance().GetVideoCardManager();

        ReturnResult result = Result::Success();

        if( command == VideoCardEvent::Command::EnableOutput )
        {
            videoCardManager->SetVideoOutput( true );
        }
        else if( command == VideoCardEvent::Command::DisableOutput )
        {
            videoCardManager->SetVideoOutput( false );
        }
        else if( command == VideoCardEvent::Command::EnableKey )
        {
            videoCardManager->SetKey( true );
        }
        else if( command == VideoCardEvent::Command::DisableKey )
        {
            videoCardManager->SetKey( false );
        }
        else if( command == VideoCardEvent::Command::SetReferenceMode )
        {
            auto card = videoCardManager->GetVideoCard( videocardID );
            result = card->SetReferenceMode( videoEvent->Mode );
        }
        else if( command == VideoCardEvent::Command::SetReferenceOffsetH )
        {
            auto card = videoCardManager->GetVideoCard( videocardID );
            result = card->SetReferenceH( videoEvent->ChannelID, videoEvent->Number );
        }
        else if( command == VideoCardEvent::Command::SetReferenceOffsetV )
        {
            auto card = videoCardManager->GetVideoCard( videocardID );
            result = card->SetReferenceV( videoEvent->ChannelID, videoEvent->Number );
        }
        else
        {
            SendSimpleErrorResponse( command, videoEvent->EventID, videoEvent->SocketID, "Unknown command" );
            return;
        }

        if( result.IsValid() )
            SendSimpleResponse( command, videoEvent->EventID, videoEvent->SocketID, true );
        else
            SendSimpleErrorResponse( command, videoEvent->EventID, videoEvent->SocketID, result.GetErrorReason().c_str() );
    }
}

} //bv
