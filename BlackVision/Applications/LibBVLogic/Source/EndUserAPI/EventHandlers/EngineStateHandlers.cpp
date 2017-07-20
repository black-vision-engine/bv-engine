#include "stdafxBVApp.h"

#include "EngineStateHandlers.h"

#include "BVAppLogic.h"
#include "ProjectManager.h"
#include "Engine/Editors/BVProjectEditor.h"
#include "Application/ApplicationContext.h"

#include "Engine/Models/ModelState.h"

#include "Engine/Events/EventHandlerHelpers.h"
#include "Engine/Models/Updaters/UpdatersHelpers.h"

#include "BVConfig.h"


namespace bv
{

// ***********************
//
EngineStateHandlers::EngineStateHandlers( BVAppLogic* logic )
    :   m_appLogic( logic )
    //,   m_lockWarning( 360 )
{
    m_enableLockQueue = DefaultConfig.EnableLockingQueue();
}

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
	auto gain							= stateEvent->Gain;

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
    else if( command == EngineStateEvent::Command::LockEventQueue )
    {
        if( m_enableLockQueue )
            GetDefaultEventManager().LockEvents( numFrames );
    }
	else if( command == EngineStateEvent::Command::SetGain )
	{
		m_appLogic->SetGain( gain );
	}
    else if( command == EngineStateEvent::Command::OutputCommand )
    {
        HandleOutputEvent( stateEvent );
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
    bool autoSelect     = mouseEvent->AutoSelect;

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


        auto & scenes = m_appLogic->GetBVProject()->GetModelScenes();
        std::pair< model::BasicNodePtr, Float32 > intersectedNode = std::make_pair< model::BasicNodePtr, Float32 >( nullptr, std::numeric_limits< float >::infinity() );
        std::string nodeScene;

        for( auto & scene : scenes )
        {
            glm::vec3 rayDirection;
            glm::vec3 cameraPos;
        
            Camera tempCamera;
            auto& currentCam = scene->GetCamerasLogic().GetCurrentCamera();

            UpdatersHelpers::UpdateCamera( &tempCamera, currentCam );

            if( tempCamera.IsPerspective() )
            {
                Float32 fovY = glm::radians( tempCamera.GetFOV() );
                Float32 d = static_cast< Float32 >( 1 / glm::tan( fovY / 2.0 ) );

                rayDirection = glm::normalize( glm::vec3( 0.0, 0.0, -1.0 ) * d + screenSpaceVec );
                rayDirection = glm::vec3( glm::inverse( tempCamera.GetViewMatrix() ) * glm::vec4( rayDirection, 0.0 ) );
                cameraPos = tempCamera.GetPosition();
            }
            else
            {
                rayDirection = tempCamera.GetDirection();

                screenSpaceVec = screenSpaceVec * glm::vec3( tempCamera.GetViewportWidth(), tempCamera.GetViewportHeight(), 0.0f );
                screenSpaceVec = glm::vec3( glm::inverse( tempCamera.GetViewMatrix() ) * glm::vec4( screenSpaceVec, 0.0 ) );

                cameraPos = tempCamera.GetPosition() + screenSpaceVec;
            }

            auto sceneIntersection = editor->FindIntersectingNode( scene, cameraPos, rayDirection );

            if( intersectedNode.first == nullptr && intersectedNode.second != std::numeric_limits< float >::infinity() )
                intersectedNode = sceneIntersection;
            else
            {
                if( sceneIntersection.first != nullptr && sceneIntersection.second < intersectedNode.second )
                {
                    nodeScene = scene->GetName();
                    intersectedNode = sceneIntersection;
                }
            }
        }

        auto node = intersectedNode.first;

        if( autoSelect )
            editor->UnselectNodes();

        if( node == nullptr )
        {
            SendSimpleErrorResponse( command, mouseEvent->EventID, mouseEvent->SocketID, "No intersection" );
            return;
        }

        bool result = true;
        
        if( autoSelect )
            result = editor->SelectNode( std::static_pointer_cast< model::BasicNode >( node ), mouseEvent->AutoSelectColor );
        
        std::string nodePath = model::ModelState::GetInstance().BuildIndexPath( node.get() );
        //std::string nodeScene = ModelState::GetInstance().QueryNodeScene( node.get() );

        JsonSerializeObject ser;
        PrepareResponseTemplate( ser, command, mouseEvent->EventID, result );
        ser.SetAttribute( "Scene", nodeScene );
        ser.SetAttribute( "ClickedNode", nodePath );

        SendResponse( ser, mouseEvent->SocketID, mouseEvent->EventID );
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

// ***********************
//
void    EngineStateHandlers::ConfigManagment          ( IEventPtr evt )
{
    if( evt->GetEventType() != ConfigEvent::Type() )
        return;
    
    bv::ConfigEventPtr configEvent  = std::static_pointer_cast<bv::ConfigEvent>( evt );
    auto command        = configEvent->ConfigCommand;
    auto& key           = configEvent->Key;
    auto& value         = configEvent->Value;


    JsonSerializeObject ser;
    PrepareResponseTemplate( ser, command, configEvent->SocketID, true );

    if( command == ConfigEvent::Command::ReadValue )
    {
        auto result = DefaultConfig.PropertyValue( key );
        ser.SetAttribute( "Value", result );
		ser.SetAttribute( "Key", key );
    }
    else if( command == ConfigEvent::Command::SetValue )
    {
        BVConfig::Instance().SetPropertyValue( key, value );
    }
    else if( command == ConfigEvent::Command::SaveConfig )
    {
        assert( false );
        //FIXME
        //bool result = ConfigManager::SaveXMLConfig();
        //SendSimpleResponse( command, configEvent->EventID, configEvent->SocketID, result );
        return;
    }
    else
    {
        SendSimpleErrorResponse( command, configEvent->EventID, configEvent->SocketID, "Unknown command" );
        return;
    }

    SendResponse( ser, configEvent->SocketID, configEvent->EventID );
}

// ***********************
//
void    EngineStateHandlers::UndoRedoEvent            ( IEventPtr evt )
{
    if( evt->GetEventType() != UndoRedoEvent::Type() )
        return;

    bv::UndoRedoEventPtr undoEvent  = std::static_pointer_cast< bv::UndoRedoEvent >( evt );
    auto command        = undoEvent->UndoCommand;
    auto & scene        = undoEvent->SceneName;
    auto numSteps       = undoEvent->NumSteps;
    auto buffSize       = undoEvent->Size;


    auto editor = m_appLogic->GetBVProject()->GetProjectEditor();

    if( command == UndoRedoEvent::Command::Undo )
    {
        bool result = editor->Undo( scene, numSteps );
        SendSimpleResponse( command, undoEvent->EventID, undoEvent->SocketID, result );
    }
    else if( command == UndoRedoEvent::Command::Redo )
    {
        bool result = editor->Redo( scene, numSteps );
        SendSimpleResponse( command, undoEvent->EventID, undoEvent->SocketID, result );
    }
    else if( command == UndoRedoEvent::Command::SetOperationsBufferSize )
    {
        auto modelScene = editor->GetModelScene( scene );
        if( modelScene )
        {
            modelScene->GetHistory().SetHistoryLength( buffSize );
            SendSimpleResponse( command, undoEvent->EventID, undoEvent->SocketID, true );
        }
        else
        {
            SendSimpleErrorResponse( command, undoEvent->EventID, undoEvent->SocketID, "Scene not found" );
        }
    }
    else
    {
        SendSimpleErrorResponse( command, undoEvent->EventID, undoEvent->SocketID, "Unknown command" );
    }

}

// ***********************
//
void    EngineStateHandlers::HandleOutputEvent       ( EngineStateEventPtr evt )
{
    std::string action = evt->Request->GetAttribute( "Action" );

    if( action == "StartAVFileRendering" )
    {
        auto fileName = evt->Request->GetAttribute( "OutputFileName" );
        if( !fileName.empty() )
        {
            auto & renderMode = m_appLogic->GetRenderMode();
            renderMode.StartToAVFileRendering( fileName );
        }
    }
    else if( action == "StopAVFileRendering" )
    {
        auto & renderMode = m_appLogic->GetRenderMode();
        renderMode.StopToAVFileRendering();
    }
}

} //bv
