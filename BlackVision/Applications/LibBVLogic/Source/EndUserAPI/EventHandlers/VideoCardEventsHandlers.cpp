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

        switch( command )
        {
            case VideoCardEvent::Command::EnableOutput:
            {
                videoCardManager->SetVideoOutput( true );
                break;
            }
            case VideoCardEvent::Command::DisableOutput:
            {
                videoCardManager->SetVideoOutput( false );
                break;
            }
            case VideoCardEvent::Command::EnableKey:
            {
                videoCardManager->SetKey( true );
                break;
            }
            case VideoCardEvent::Command::DisableKey:
            {
                videoCardManager->SetKey( false );
                break;
            }
            case VideoCardEvent::Command::SetReferenceMode:
            {
                auto card = videoCardManager->GetVideoCard( videocardID );
                result = card->SetReferenceMode( videoEvent->Mode );

                break;
            }
            case VideoCardEvent::Command::SetReferenceOffsetH:
            {
                auto card = videoCardManager->GetVideoCard( videocardID );
                result = card->SetReferenceH( videoEvent->ChannelID, videoEvent->Number );

                break;
            }
            case VideoCardEvent::Command::SetReferenceOffsetV:
            {
                auto card = videoCardManager->GetVideoCard( videocardID );
                result = card->SetReferenceV( videoEvent->ChannelID, videoEvent->Number );

                break;
            }
            default:
            {
                SendSimpleErrorResponse( command, videoEvent->EventID, videoEvent->SocketID, "Unknown command" );
                return;
            }
        }

        if( result.IsValid() )
            SendSimpleResponse( command, videoEvent->EventID, videoEvent->SocketID, true );
        else
            SendSimpleErrorResponse( command, videoEvent->EventID, videoEvent->SocketID, result.GetErrorReason().c_str() );
    }
}

} //bv
