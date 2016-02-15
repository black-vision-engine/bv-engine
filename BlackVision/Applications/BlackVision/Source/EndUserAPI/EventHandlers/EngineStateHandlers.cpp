#include "EngineStateHandlers.h"

#include "../../BVAppLogic.h"
#include "EventHandlerHelpers.h"

namespace bv
{

// ***********************
//
EngineStateHandlers::EngineStateHandlers( BVAppLogic* logic )
    :   m_appLogic( logic )
{}

// ***********************
//
EngineStateHandlers::~EngineStateHandlers()
{}

// ***********************
//
void EngineStateHandlers::EngineStateHandler( IEventPtr evt )
{
    if( evt->GetEventType() != EngineStateEvent::Type() )
        return;

    bv::EngineStateEventPtr stateEvent = std::static_pointer_cast<bv::EngineStateEvent>( evt );
    std::string& filePath = stateEvent->FilePath;
    unsigned int numFrames = stateEvent->NumFrames;
    float requestedFPS = stateEvent->FPS;
    auto command = stateEvent->RenderingCommand;

    auto& renderMode = m_appLogic->GetRenderMode();
    if( command == EngineStateEvent::Command::ScreenShot )
        renderMode.MakeScreenShot( filePath );
    else if( command == EngineStateEvent::Command::RenderOffscreen )
        renderMode.SetRenderToFileMode( filePath, requestedFPS, numFrames );
    else
    {
        SendSimpleErrorResponse( command, stateEvent->EventID, stateEvent->SocketID, "Unknown command" );
        return;
    }

    SendSimpleResponse( command, stateEvent->EventID, stateEvent->SocketID, true );
}


} //bv
