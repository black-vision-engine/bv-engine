#include "stdafxBVApp.h"

#include "GenericEventsHandlers.h"

#include "BVAppLogic.h"
#include "Engine/Editors/BVProjectEditor.h"
#include "Engine/Events/EventHandlerHelpers.h"

#include "Engine/Models/Updaters/UpdatersHelpers.h"
#include "Application/ApplicationContext.h"


namespace bv
{

// ***********************
//
GenericEventsHandlers::GenericEventsHandlers( BVAppLogic* appLogic )
    : m_appLogic( appLogic )
{}

// ***********************
//
GenericEventsHandlers::~GenericEventsHandlers()
{}

// ***********************
//
void    GenericEventsHandlers::EventHandler             ( bv::IEventPtr evt )
{
    if( evt->GetEventType() == bv::GenericEvent::Type() )
    {
        bv::GenericEventPtr genericEvent = std::static_pointer_cast< GenericEvent >( evt );

        std::string & command = genericEvent->CommandName;
        auto request = genericEvent->Request;

        SimpleJsonSerializeObject responseJSON;

        if( command == "SetWeightCenter" )
        {
            SetWeightCenterHandler( command, responseJSON, request, genericEvent->EventID );
        }
        else if( command == "AdjustToScreen" )
        {
            SetNodeScaleHandler( command, responseJSON, request, genericEvent->EventID );
        }
        else
        {
            SendSimpleErrorResponse( command, genericEvent->EventID, genericEvent->SocketID, "Unknown command" );
            return;
        }


        SendResponse( responseJSON, genericEvent->SocketID, genericEvent->EventID );
    }
}


// ***********************
//
glm::vec3       GetBBPoint          ( model::BasicNodeConstPtr & node, const std::string & alignX, const std::string & alignY, const std::string & alignZ, TimeType time )
{
    if( node != nullptr )
    {
        auto boundingVolume = node->GetBoundingVolume();
        auto bb = boundingVolume->GetBoundingBox();

        auto transformParam = node->GetFinalizePlugin()->GetParamTransform();

        glm::vec3 BBPoint;

        if( alignX == "Left" )
        {
            BBPoint.x = bb->xmin;
        }
        else if( alignX == "Right" )
        {
            BBPoint.x = bb->xmax;
        }
        else if( alignX == "Center" )
        {
            BBPoint.x = ( bb->xmax + bb->xmin ) / 2;
        }
        else
        {
            auto center = transformParam->GetTransform().GetCenter( time );
            BBPoint.x = center.x;
        }

        if( alignY == "Bottom" )
        {
            BBPoint.y = bb->ymin;
        }
        else if( alignY == "Top" )
        {
            BBPoint.y = bb->ymax;
        }
        else if( alignY == "Center" )
        {
            BBPoint.y = ( bb->ymax + bb->ymin ) / 2;
        }
        else
        {
            auto center = transformParam->GetTransform().GetCenter( time );
            BBPoint.y = center.y;
        }

        if( alignZ == "Back" )
        {
            BBPoint.z = bb->zmin;
        }
        else if( alignZ == "Front" )
        {
            BBPoint.z = bb->zmax;
        }
        else if( alignZ == "Center" )
        {
            BBPoint.z = ( bb->zmax + bb->zmin ) / 2;
        }
        else
        {
            auto center = transformParam->GetTransform().GetCenter( time );
            BBPoint.z = center.z;
        }

        return BBPoint;
    }
    return glm::vec3( 0.0, 0.0, 0.0 );
}


// ***********************
//
void    GenericEventsHandlers::SetWeightCenterHandler   ( const std::string & command, JsonSerializeObject & ser, IDeserializer * request, int eventID )
{
    std::string nodeName = request->GetAttribute( SerializationHelper::NODE_NAME_STRING );
    std::string sceneName = request->GetAttribute( SerializationHelper::SCENE_NAME_STRING );

    std::string alignX = request->GetAttribute( "AlignX" );
    std::string alignY = request->GetAttribute( "AlignY" );
    std::string alignZ = request->GetAttribute( "AlignZ" );

    TimeType keyTime = Convert::String2T( request->GetAttribute( "Time" ), TimeType( 0.0 ) );

    auto node = m_appLogic->GetBVProject()->GetProjectEditor()->GetNode( sceneName, nodeName );
    if( node == nullptr )
    {
        ErrorResponseTemplate( ser, command, eventID, "Node not found." );
        return;
    }

    auto basicNode = std::static_pointer_cast< const model::BasicNode >( node );
    glm::vec3 bbPoint = GetBBPoint( basicNode, alignX, alignY, alignZ, keyTime );

    auto transformParam = basicNode->GetFinalizePlugin()->GetParamTransform();
    transformParam->SetCenter( bbPoint, keyTime );

    PrepareResponseTemplate( ser, command, eventID, true );
}

// ***********************
//
void    GenericEventsHandlers::SetNodeScaleHandler      ( const std::string & command, JsonSerializeObject & ser, IDeserializer * request, int eventID )
{
    std::string nodeName = request->GetAttribute( SerializationHelper::NODE_NAME_STRING );
    std::string sceneName = request->GetAttribute( SerializationHelper::SCENE_NAME_STRING );

    TimeType keyTime = Convert::String2T( request->GetAttribute( "Time" ), TimeType( 0.0 ) );

    auto scene = m_appLogic->GetBVProject()->GetModelScene( sceneName );
    auto node = m_appLogic->GetBVProject()->GetProjectEditor()->GetNode( sceneName, nodeName );
    if( scene == nullptr || node == nullptr )
    {
        ErrorResponseTemplate( ser, command, eventID, "Node not found." );
        return;
    }

    auto basicNode = std::static_pointer_cast< const model::BasicNode >( node );

    auto boundingVolume = basicNode->GetBoundingVolume();
    auto bb = boundingVolume->GetBoundingBox();

    auto transformParam = basicNode->GetFinalizePlugin()->GetParamTransform();
    glm::vec3 scale;

    glm::vec3 center = bb->Center();

    Float32 screenWidth = (Float32)ApplicationContext::Instance().GetWidth();
    Float32 screenHeight = (Float32)ApplicationContext::Instance().GetHeight();
    Float32 aspect = screenWidth / screenHeight;


    Camera tempCamera;
    auto& currentCam = scene->GetCamerasLogic().GetCurrentCamera();
    UpdatersHelpers::UpdateCamera( &tempCamera, currentCam );

    if( tempCamera.IsPerspective() )
    {
        glm::vec3 cameraPos = tempCamera.GetPosition();
        float dist = glm::length( center - cameraPos );

        Float32 fovY = glm::radians( tempCamera.GetFOV() );
        Float32 d = static_cast< Float32 >( 1 / glm::tan( fovY / 2.0 ) );

        scale.y = 2 * dist / d / bb->Height();
        scale.x = aspect * 2 * dist / d / bb->Width();
        scale.z = 1.0f;
    }
    else
    {
        scale.x = 2 * aspect / bb->Height();
        scale.y = 2.0f / bb->Width();
        scale.z = 1.0f;
    }

    transformParam->SetScale( scale, keyTime );
    PrepareResponseTemplate( ser, command, eventID, true );
}


}   // bv


