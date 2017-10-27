#include "stdafxBVApp.h"

#include "NodeLogicHandlers.h"

#include "Engine/Editors/BVProjectEditor.h"
#include "BVAppLogic.h"
#include "UseLoggerBVAppModule.h"
#include "Engine/Events/EventHandlerHelpers.h"
#include "Engine/Models/NodeLogics/NodeLogicFactory.h"
#include "Engine/Models/BasicNode.h"

#include "Serialization/Json/JsonSerializeObject.h"
#include "Serialization/Json/JsonDeserializeObject.h"
#include "Serialization/BV/BVDeserializeContext.h"

#include "Engine/Models/NodeLogics/Counter/Counter.h"

namespace bv
{

// ***********************
//
NodeLogicHandlers::NodeLogicHandlers( BVAppLogic* logic )
    : m_appLogic( logic )
{}

// ***********************
//
NodeLogicHandlers::~NodeLogicHandlers()
{}



// ***********************
//
void NodeLogicHandlers::WidgetHandler       ( bv::IEventPtr evt )
{
	if( evt->GetEventType() != bv::NodeLogicEvent::Type() )
        return;

	bv::NodeLogicEventPtr widgetEvent = std::static_pointer_cast<bv::NodeLogicEvent>( evt );        
    auto editor = m_appLogic->GetBVProject()->GetProjectEditor();
    
    auto root = m_appLogic->GetBVProject()->GetModelSceneRoot();
        
    std::string& nodePath = widgetEvent->NodeName;
    std::string& sceneName = widgetEvent->SceneName;
    auto& action = *widgetEvent->Action;
    NodeLogicEvent::Command command = widgetEvent->WidgetCommand;

    assert( widgetEvent->Action != nullptr );
    if( widgetEvent->Action == nullptr )
    {
        SendSimpleErrorResponse( command, widgetEvent->EventID, widgetEvent->SocketID, "Wrong request" );
        return;
    }


    auto node = m_appLogic->GetBVProject()->GetProjectEditor()->GetNode( sceneName, nodePath );
    auto basicNode = std::static_pointer_cast< bv::model::BasicNode >( node );
    if( basicNode == nullptr )
    {
        SendSimpleErrorResponse( command, widgetEvent->EventID, widgetEvent->SocketID, "Node not found" );
        return;
    }


    SimpleJsonSerializeObject ser;

    auto context = static_cast< BVDeserializeContext* >( action.GetDeserializeContext() );
    context->SetSceneName( sceneName );
    context->SetNodePath( nodePath );

		
    if( command == NodeLogicEvent::Command::AddNodeLogic )
    {
        context->SetSceneTimeline( std::static_pointer_cast< model::OffsetTimeEvaluator >( model::TimelineManager::GetInstance()->GetTimeEvaluator( sceneName ) ) );

        action.EnterChild( "logic" );
        
        auto logic = GetNodeLogicFactory()->CreateLogic( action, basicNode );
        
        action.ExitChild();  // nodeLogic

        if( logic == nullptr)
        {
            SendSimpleErrorResponse( command, widgetEvent->EventID, widgetEvent->SocketID, "Logic could not be created" );
            return;
        }

        editor->SetLogic( basicNode, logic, true );

        SendSimpleResponse( command, widgetEvent->EventID, widgetEvent->SocketID, true );
    }
    else if( command == NodeLogicEvent::Command::DeleteNodeLogic )
    {
        editor->RemoveLogic( basicNode, true );
    }
    else if ( command == NodeLogicEvent::Command::LogicAction )
    {
        model::INodeLogic* logic = basicNode->GetLogic().get();
        if( logic == nullptr )
        {
            LOG_MESSAGE( SeverityLevel::warning ) << "Error NodeLogicEvent node [" + nodePath + "] , logic [] not found";
            SendSimpleErrorResponse( command, widgetEvent->EventID, widgetEvent->SocketID, "NodeLogic not found" );
            return;
        }

        bool result = logic->HandleEvent( action, ser, editor );

        PrepareResponseTemplate( ser, command, widgetEvent->EventID, result );
        SendResponse( ser, widgetEvent->SocketID, widgetEvent->EventID );
    }
    else
        SendSimpleErrorResponse( command, widgetEvent->EventID, widgetEvent->SocketID, "Unknown command" );
}



} //bv
