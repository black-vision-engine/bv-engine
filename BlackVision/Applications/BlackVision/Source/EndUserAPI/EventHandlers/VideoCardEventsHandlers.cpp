#include "VideoCardEventsHandlers.h"

#include "BVAppLogic.h"


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
        auto command = videoEvent->VideoCommand;

        auto videoCardManager = m_appLogic->GetVideoCardManager();

        if( command == VideoCardEvent::Command::EnableOutput )
        {
            videoCardManager->Enable();
        }
        else  if( command == VideoCardEvent::Command::DisableOutput )
        {
            videoCardManager->Disable();
        }
        else if( command == VideoCardEvent::Command::EnableKey )
        {
            videoCardManager->SetKey( true );
        }
        else if( command == VideoCardEvent::Command::DisableKey )
        {
            videoCardManager->SetKey( false );
        }
    }
}

} //bv
