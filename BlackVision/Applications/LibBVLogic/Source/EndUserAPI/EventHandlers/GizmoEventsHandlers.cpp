#include "stdafxBVApp.h"
#include "GizmoEventsHandlers.h"


#include "Engine/Editors/BVProjectEditor.h"
#include "BVAppLogic.h"




namespace bv
{

// ***********************
//
GizmoHandlers::GizmoHandlers( BVAppLogic * logic )
    : m_appLogic( logic )
{}

// ***********************
//
GizmoHandlers::~GizmoHandlers()
{}


// ***********************
//
void            GizmoHandlers::GizmoHandler     ( IEventPtr evt )
{
    if( evt->GetEventType() != GizmoEvent::Type() )
        return;

    GizmoEventPtr gizmoEvent = std::static_pointer_cast< GizmoEvent >( evt );

    GizmoEvent::Command command = gizmoEvent->CommandName;

    model::GizmoType gizmoType  = gizmoEvent->Address.GizmoType;
    std::string & sceneName     = gizmoEvent->Address.SceneName;
    std::string & nodeName      = gizmoEvent->Address.NodeName;
    std::string & elementName   = gizmoEvent->Address.ElementName;
    std::string & functionName  = gizmoEvent->Address.FunctionalityName;

    auto editor = m_appLogic->GetBVProject()->GetProjectEditor();

    bool result = false;

    switch( command )
    {
    case GizmoEvent::Command::CreateGizmo:
        result = editor->CreateGizmo( sceneName, nodeName, gizmoType, elementName, functionName );
        break;
    case GizmoEvent::Command::RemoveGizmo:
        result = editor->DeleteGizmo( sceneName, nodeName, functionName );
        break;
    case GizmoEvent::Command::ListGizmos:
        ListGizmos( gizmoEvent, sceneName, nodeName, gizmoType, elementName );
        return;
    default:
        break;
    }

    SendSimpleResponse( command, gizmoEvent->EventID, gizmoEvent->SocketID, result );
}

// ***********************
//
void        GizmoHandlers::ListGizmos           ( GizmoEventPtr gizmoEvt, const std::string & sceneName, const std::string nodeName, model::GizmoType type, const std::string & elementName )
{
    assert( !"Implement in future" );
    elementName;

    auto editor = m_appLogic->GetBVProject()->GetProjectEditor();
    auto node = std::static_pointer_cast< model::BasicNode >( editor->GetNode( sceneName, nodeName ) );

    if( node )
    {
        switch( type )
        {
        case model::GizmoType::Scene:
            break;
        case model::GizmoType::Node:
            break;
        case model::GizmoType::Logic:
            break;
        case model::GizmoType::Plugin:
            break;
        case model::GizmoType::Effect:
            break;
        default:
            break;
        }
    }
    else
    {
        SendSimpleErrorResponse( GizmoEvent::Command::ListGizmos, gizmoEvt->EventID, gizmoEvt->SocketID, "Node not found" );
    }

}


}	// bv

