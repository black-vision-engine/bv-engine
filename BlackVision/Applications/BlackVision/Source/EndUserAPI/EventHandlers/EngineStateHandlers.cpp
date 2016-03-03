#include "EngineStateHandlers.h"

#include "BVAppLogic.h"
#include "EventHandlerHelpers.h"
#include "ProjectManager.h"
#include "Engine/Models/BVProjectEditor.h"

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

    bv::EngineStateEventPtr stateEvent  = std::static_pointer_cast<bv::EngineStateEvent>( evt );
    std::string& filePath               = stateEvent->FilePath;
    unsigned int numFrames              = stateEvent->NumFrames;
    float requestedFPS                  = stateEvent->FPS;
    auto command                        = stateEvent->RenderingCommand;

    // Converts to path inside project manager.
    std::string path = ProjectManager::GetInstance()->ToAbsPath( filePath ).Str();

    auto& renderMode = m_appLogic->GetRenderMode();
    if( command == EngineStateEvent::Command::ScreenShot )
    {
        renderMode.MakeScreenShot( path );
    }
    else if( command == EngineStateEvent::Command::RenderOffscreen )
    {
        renderMode.SetRenderToFileMode( path, requestedFPS, numFrames );
    }
    else if( command == EngineStateEvent::Command::CloseApplication )
    {
        m_appLogic->ChangeState( BVAppState::BVS_CLOSING );
    }
    else
    {
        SendSimpleErrorResponse( command, stateEvent->EventID, stateEvent->SocketID, "Unknown command" );
        return;
    }

    SendSimpleResponse( command, stateEvent->EventID, stateEvent->SocketID, true );
}

// ***********************
//
void    EngineStateHandlers::MouseInteraction         ( IEventPtr evt )
{
    if( evt->GetEventType() != MouseEvent::Type() )
        return;
    
    bv::MouseEventPtr mouseEvent  = std::static_pointer_cast<bv::MouseEvent>( evt );
    auto command        = mouseEvent->MouseCommand;
    //auto mouseX         = mouseEvent->MouseX;
    //auto mouseY         = mouseEvent->MouseY;

    auto editor = m_appLogic->GetBVProject()->GetProjectEditor();

    if( command == MouseEvent::Command::MouseDown )
    {
        auto node = editor->FindIntersectingNode( glm::vec3( 0.0, 0.0, 0.0 ), glm::vec3( 0.0, 0.0, -1.0 ) );

        if( node == nullptr )
        {
            SendSimpleErrorResponse( command, mouseEvent->EventID, mouseEvent->SocketID, "No intersection" );
            return;
        }

        bool result = editor->SelectNode( std::static_pointer_cast< model::BasicNode >( node ) );
        
        JsonSerializeObject ser;
        PrepareResponseTemplate( ser, command, mouseEvent->SocketID, result );
        ser.SetAttribute( "ClickedNode", node->GetName() );
    }
    else if( command == MouseEvent::Command::MouseUp )
    {

    }
    else if( command == MouseEvent::Command::MouseMove )
    {

    }
    else
    {
        SendSimpleErrorResponse( command, mouseEvent->EventID, mouseEvent->SocketID, "Unknown command" );
        return;
    }

}

} //bv
