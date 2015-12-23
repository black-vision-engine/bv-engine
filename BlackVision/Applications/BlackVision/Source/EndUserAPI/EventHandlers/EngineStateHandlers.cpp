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
void EngineStateHandlers::RenderingModeEvent( IEventPtr evt )
{
    if( evt->GetEventType() != RenderingModeEvent::Type() )
        return;

    bv::RenderingModeEventPtr modeEvent = std::static_pointer_cast<bv::RenderingModeEvent>( evt );
    std::string& filePath = modeEvent->FilePath;
    unsigned int numFrames = modeEvent->NumFrames;
    float requestedFPS = modeEvent->FPS;
    auto command = modeEvent->RenderingCommand;

    auto& renderMode = m_appLogic->GetRenderMode();
    if( command == RenderingModeEvent::Command::ScreenShot )
        renderMode.MakeScreenShot( filePath );
    else if( command == RenderingModeEvent::Command::RenderOffscreen )
        renderMode.SetRenderToFileMode( filePath, requestedFPS, numFrames );
    else
    {
        SendSimpleErrorResponse( command, modeEvent->EventID, modeEvent->SocketID, "Unknown command" );
        return;
    }

    SendSimpleResponse( command, modeEvent->EventID, modeEvent->SocketID, true );
}


} //bv
