#include "NodeLogicHandlers.h"

#include "Engine/Models/BVProjectEditor.h"
#include "../../BVAppLogic.h"
#include "../../UseLoggerBVAppModule.h"
#include "EventHandlerHelpers.h"
#include "../../Widgets/NodeLogicFactory.h"
#include "Engine/Models/BasicNode.h"

#include "Serialization/Json/JsonSerializeObject.h"
#include "Serialization/Json/JsonDeserializeObject.h"
#include "Serialization/BV/BVDeserializeContext.h"

#include "Widgets/Crawler/CrawlerEvents.h"
#include "Widgets/Counter/Counter.h"

namespace bv
{

// ***********************
//
NodeLogicHandlers::NodeLogicHandlers( BVAppLogic* logic )
    : m_appLogic( logic )
{
}

// ***********************
//
NodeLogicHandlers::~NodeLogicHandlers()
{
}





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
    std::string& action = widgetEvent->Action;
    NodeLogicEvent::Command command = widgetEvent->WidgetCommand;

    auto node = m_appLogic->GetBVProject()->GetProjectEditor()->GetNode( sceneName, nodePath );
    auto basicNode = std::static_pointer_cast< bv::model::BasicNode >( node );
    if( basicNode == nullptr )
    {
        SendSimpleErrorResponse( command, widgetEvent->EventID, widgetEvent->SocketID, "Node not found" );
        return;
    }



    JsonSerializeObject ser;
    JsonDeserializeObject deser;
    deser.Load( action );

    auto context = static_cast< BVDeserializeContext* >( deser.GetDeserializeContext() );
    context->SetEditor( editor );
    context->SetSceneName( sceneName );

		
    if( command == NodeLogicEvent::Command::AddNodeLogic )
    {
        context->SetSceneTimeline( std::static_pointer_cast< model::OffsetTimeEvaluator >( TimelineManager::GetInstance()->GetTimeEvaluator( sceneName ) ) );

        deser.EnterChild( "logic" );
        
        auto logic = GetNodeLogicFactory()->CreateLogic( deser, basicNode.get() );
        
        deser.ExitChild();  // nodeLogic

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

        bool result = logic->HandleEvent( deser, ser );

        PrepareResponseTemplate( ser, command, widgetEvent->SocketID, result );
        SendResponse( ser, widgetEvent->SocketID, widgetEvent->EventID );
    }
    else
        SendSimpleErrorResponse( command, widgetEvent->EventID, widgetEvent->SocketID, "Unknown command" );
}

// ***********************
//
void NodeLogicHandlers::OnNodeAppearing     ( IEventPtr evt )
{

}

// ***********************
//
void NodeLogicHandlers::OnNodeLeaving       ( IEventPtr evt )
{

}


// *********************************
//
namespace 
{
    const std::wstring* examples2 = new std::wstring[20];
   

    int examplesIndex=0;


	const static std::wstring examples[] = 
	{
		L"Jasiu kup kie³basê !!",
		L"wielojêzyczny projekt internetortej treœci. Funkcjonuje wykorzystuj¹c",
		L"Wikipedia powsta³a 15 stycznia ertów i nieistniej¹cej ju¿ Nupedii. ",
		L"iostrzane. Wikipedia jest jedn¹], a wiele stron uruchomi³o jej mirrory lub forki.",
		L"Wspó³za³o¿yciel Wikipedii Jimmyia wielojêzycznej",
		L"wolnej encyklopedii o najwy¿szyw³asnym jêzyku”[8].",
		L"Kontrowersje budzi wiarygodnoœæeœci artyku³ów ",
		L"i brak weryfikacji kompetencji .",
		L"Z drugiej",
		L"strony mo¿liwoœæ swobodnej dyst Ÿród³em informacji",
		L"Jasiu kup kie³basê !!",
	};

   

   

	auto exampleSize = sizeof( examples ) / sizeof( std::wstring );
}


// ***********************
//
void NodeLogicHandlers::OnNoMoreNodes       ( IEventPtr evt )
{
	auto typedEvent = std::static_pointer_cast< nodelogic::NoMoreNodesCrawlerEvent >( evt );
	// Remove code below. Only for testing.
	auto n = typedEvent->GetCrawler()->GetNonActiveNode();
	if( n )
	{

		auto textNode = n->GetChild( "Text" );
		if( textNode )
		{
			auto pl = textNode->GetPlugin( "text" );

			if( pl )
			{
				//model::DefaultTextPlugin::SetText( pl, examples[ i ] );

                SetParameter( pl->GetParameter( "text" ), 0.0, L"nowa wiadomoœæ "+to_wstring(examplesIndex) );
                examplesIndex=(examplesIndex+1)%20;

				typedEvent->GetCrawler()->EnqueueNode( n );
			}
		}
	}
}

} //bv
