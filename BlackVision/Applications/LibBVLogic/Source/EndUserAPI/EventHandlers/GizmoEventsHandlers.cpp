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
    default:
        break;
    }

    SendSimpleResponse( command, gizmoEvent->EventID, gizmoEvent->SocketID, result );
}


}	// bv

