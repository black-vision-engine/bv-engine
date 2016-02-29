#include "NodeLogicHandlers.h"

#include "Engine/Models/BVProjectEditor.h"
#include "BVAppLogic.h"
#include "UseLoggerBVAppModule.h"
#include "EventHandlerHelpers.h"
#include "Widgets/NodeLogicFactory.h"
#include "Engine/Models/BasicNode.h"

#include "Serialization/Json/JsonSerializeObject.h"
#include "Serialization/Json/JsonDeserializeObject.h"
#include "Serialization/BV/BVDeserializeContext.h"

#include "Widgets/Scroller/ScrollerEvents.h"
#include "Widgets/Counter/Counter.h"

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


    JsonSerializeObject ser;

    auto context = static_cast< BVDeserializeContext* >( action.GetDeserializeContext() );
    context->SetSceneName( sceneName );
    context->SetNodePath( nodePath );

		
    if( command == NodeLogicEvent::Command::AddNodeLogic )
    {
        context->SetSceneTimeline( std::static_pointer_cast< model::OffsetTimeEvaluator >( TimelineManager::GetInstance()->GetTimeEvaluator( sceneName ) ) );

        action.EnterChild( "logic" );
        
        auto logic = GetNodeLogicFactory()->CreateLogic( action, basicNode );
        
        action.ExitChild();  // nodeLogic

        if( logic == nullptr)
        {
            SendSimpleErrorResponse( command, widgetEvent->EventID, widgetEvent->SocketID, "Logic could not be created" );
            return;
        }

        basicNode->SetLogic( logic );

        SendSimpleResponse( command, widgetEvent->EventID, widgetEvent->SocketID, true );
    }
    else if( command == NodeLogicEvent::Command::DeleteNodeLogic )
    {
        basicNode->RemoveLogic();
    }
    else if ( command == NodeLogicEvent::Command::LogicAction )
    {
        INodeLogic* logic = basicNode->GetLogic().get();
        if( logic == nullptr )
        {
            LOG_MESSAGE( SeverityLevel::warning ) << "Error NodeLogicEvent node [" + nodePath + "] , logic [] not found";
            SendSimpleErrorResponse( command, widgetEvent->EventID, widgetEvent->SocketID, "NodeLogic not found" );
        }

        bool result = logic->HandleEvent( action, ser, editor );

        PrepareResponseTemplate( ser, command, widgetEvent->SocketID, result );
        SendResponse( ser, widgetEvent->SocketID, widgetEvent->EventID );
    }
    else
        SendSimpleErrorResponse( command, widgetEvent->EventID, widgetEvent->SocketID, "Unknown command" );
}



} //bv
