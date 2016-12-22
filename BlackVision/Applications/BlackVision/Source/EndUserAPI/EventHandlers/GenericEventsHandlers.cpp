#include "stdafxBVApp.h"

#include "GenericEventsHandlers.h"

#include "BVAppLogic.h"
#include "Engine/Models/BVProjectEditor.h"
#include "Engine/Events/EventHandlerHelpers.h"


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

        JsonSerializeObject responseJSON;

        if( command == "SetWeightCenter" )
        {
            SetWeightCenterHandler( command, responseJSON, request, genericEvent->EventID );
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
glm::vec3       GetBBPoint          ( model::BasicNodeConstPtr & node, const std::string & alignX, const std::string & alignY, const std::string & alignZ )
{
    if( node != nullptr )
    {
        auto boundingVolume = node->GetBoundingVolume();
        auto bb = boundingVolume->GetBoundingBox();

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

    TimeType keyTime = SerializationHelper::String2T( request->GetAttribute( "Time" ), TimeType( 0.0 ) );

    auto node = m_appLogic->GetBVProject()->GetProjectEditor()->GetNode( sceneName, nodeName );
    if( node == nullptr )
    {
        ErrorResponseTemplate( ser, command, eventID, "Node not found." );
        return;
    }

    auto basicNode = std::static_pointer_cast< const model::BasicNode >( node );
    glm::vec3 bbPoint = GetBBPoint( basicNode, alignX, alignY, alignZ );

    auto transformParam = basicNode->GetFinalizePlugin()->GetParamTransform();
    transformParam->SetCenter( bbPoint, keyTime );

    PrepareResponseTemplate( ser, command, eventID, true );
}


}   // bv


