#include "stdafxBVApp.h"

#include "VideoCardEventsHandlers.h"

#include "BVAppLogic.h"
#include "VideoCardManager.h"
#include "Services/BVServiceProvider.h"

#include "VideoCardManagerUtils.h"


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
                auto expCard = GetVideoCard( videoCardManager, videocardID );
                if( expCard.IsValid() )
                    result = expCard.GetVal()->SetReferenceMode( videoEvent->Mode );
                else
                    result = expCard.GetErrorReason();

                break;
            }
            case VideoCardEvent::Command::SetReferenceOffsetH:
            {
                auto expCard = GetVideoCard( videoCardManager, videocardID );
                if( expCard.IsValid() )
                    result = expCard.GetVal()->SetReferenceH( videoEvent->Number );
                else
                    result = expCard.GetErrorReason();

                break;
            }
            case VideoCardEvent::Command::SetReferenceOffsetV:
            {
                auto expCard = GetVideoCard( videoCardManager, videocardID );
                if( expCard.IsValid() )
                    result = expCard.GetVal()->SetReferenceV( videoEvent->Number );
                else
                    result = expCard.GetErrorReason();

                break;
            }
            case VideoCardEvent::Command::GetReferenceSignalInfo:
            {
                GetReferenceSignalInfoHandler( videoCardManager, videoEvent );
                return;
            }
            case VideoCardEvent::Command::IsLocked:
            {
                IsLockedHandler( videoCardManager, videoEvent );
                return;
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

// ***********************
//
void        VideoCardEventsHandlers::GetReferenceSignalInfoHandler      ( videocards::VideoCardManager * videoCardManager, VideoCardEventPtr videocardEvent )
{
    auto expCard = GetVideoCard( videoCardManager, videocardEvent->VideoCardID );
    if( !expCard.IsValid() )
    {
        SendSimpleErrorResponse( VideoCardEvent::Command::GetReferenceSignalInfo, videocardEvent->EventID, videocardEvent->SocketID, expCard.GetErrorReason().c_str() );
        return;
    }

    auto card = expCard.GetVal();
    auto referenceSignalInfo = card->GetReferenceInfo();

    JsonSerializeObject responseJSON( BVSerializeContext::CreateContextFromEmptiness() );

    responseJSON.EnterChild( "referenceSignal" );

    responseJSON.SetAttribute( "referenceMode", Convert::T2String( referenceSignalInfo.Mode ) );
    responseJSON.SetAttribute( "referenceH", Convert::T2String( referenceSignalInfo.ReferenceH ) );
    responseJSON.SetAttribute( "referenceV", Convert::T2String( referenceSignalInfo.ReferenceV ) );

    responseJSON.ExitChild();

    PrepareResponseTemplate( responseJSON, VideoCardEvent::Command::GetReferenceSignalInfo, videocardEvent->EventID, true );
    SendResponse( responseJSON, videocardEvent->SocketID, videocardEvent->EventID );
}

// ***********************
//
void        VideoCardEventsHandlers::IsLockedHandler                ( videocards::VideoCardManager * videoCardManager, VideoCardEventPtr videocardEvent )
{
    auto expCard = GetVideoCard( videoCardManager, videocardEvent->VideoCardID );
    if( !expCard.IsValid() )
    {
        SendSimpleErrorResponse( VideoCardEvent::Command::IsLocked, videocardEvent->EventID, videocardEvent->SocketID, expCard.GetErrorReason().c_str() );
        return;
    }

    auto card = expCard.GetVal();

    JsonSerializeObject responseJSON( BVSerializeContext::CreateContextFromEmptiness() );

    responseJSON.SetAttribute( "isLocked", Convert::T2String( card->IsLocked() ) );

    PrepareResponseTemplate( responseJSON, VideoCardEvent::Command::IsLocked, videocardEvent->EventID, true );
    SendResponse( responseJSON, videocardEvent->SocketID, videocardEvent->EventID );
}

// ***********************
//
Expected< videocards::IVideoCardPtr >       VideoCardEventsHandlers::GetVideoCard       ( videocards::VideoCardManager * videoCardManager, UInt32 videoCardID )
{
    auto card = videoCardManager->GetVideoCard( videoCardID );
    if( card )
        return card;
    else
        return "VideoCard [" + Convert::T2String( videoCardID ) + "] doesn't exist.";
}

} //bv
