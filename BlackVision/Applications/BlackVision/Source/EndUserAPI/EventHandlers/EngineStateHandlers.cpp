#include "EngineStateHandlers.h"

#include "BVAppLogic.h"
#include "EventHandlerHelpers.h"
#include "ProjectManager.h"
#include "Engine/Models/BVProjectEditor.h"
#include "Application/ApplicationContext.h"

#include "Engine/Models/ModelState.h"

#include "BVConfig.h"

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
    auto mouseX         = mouseEvent->MouseX;
    auto mouseY         = mouseEvent->MouseY;

    auto editor = m_appLogic->GetBVProject()->GetProjectEditor();

    if( command == MouseEvent::Command::MouseDown )
    {
        // Fixme: move this code in different place in future, when camera will be
        // handle properly.

        assert( mouseX >= 0 );
        assert( mouseY >= 0 );

        Float32 screenWidth = (Float32)ApplicationContext::Instance().GetWidth();
        Float32 screenHeight = (Float32)ApplicationContext::Instance().GetHeight();

        screenHeight /= 2;
        screenWidth /= 2;

        Float32 normMouseX = ( mouseX - screenWidth ) / screenWidth;
        Float32 normMouseY = ( screenHeight - mouseY ) / screenHeight;

        Float32 aspect = screenWidth / screenHeight;
        glm::vec3 screenSpaceVec( normMouseX * aspect, normMouseY, 0.0f );

        glm::vec3 rayDirection;
        glm::vec3 cameraPos;
        
        if( DefaultConfig.IsCameraPerspactive() )
        {
            Float32 fovY = glm::radians( DefaultConfig.FOV() );
            Float32 d = static_cast< Float32 >( 1 / glm::tan( fovY / 2.0 ) );

            rayDirection = glm::normalize( glm::vec3( 0.0, 0.0, -1.0 ) * d + screenSpaceVec );
            cameraPos = DefaultConfig.CameraPosition();
        }
        else
        {
            rayDirection = glm::vec3( 0.0, 0.0, -1.0 );
            cameraPos = DefaultConfig.CameraPosition() + screenSpaceVec;
        }

        auto node = editor->FindIntersectingNode( cameraPos, rayDirection );
        editor->UnselectNodes();

        if( node == nullptr )
        {
            SendSimpleErrorResponse( command, mouseEvent->EventID, mouseEvent->SocketID, "No intersection" );
            return;
        }

        bool result = editor->SelectNode( std::static_pointer_cast< model::BasicNode >( node ) );
        
        std::string nodePath = ModelState::GetInstance().QueryNodePath( node.get() );
        std::string nodeScene = ModelState::GetInstance().QueryNodeScene( node.get() );

        JsonSerializeObject ser;
        PrepareResponseTemplate( ser, command, mouseEvent->SocketID, result );
        ser.SetAttribute( "Scene", nodeScene );
        ser.SetAttribute( "ClickedNode", nodePath );

        SendResponse( ser, mouseEvent->SocketID, mouseEvent->SocketID );
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
