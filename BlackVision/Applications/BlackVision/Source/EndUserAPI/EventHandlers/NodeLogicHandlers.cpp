#include "NodeLogicHandlers.h"

#include "Engine/Models/BVProjectEditor.h"
#include "../../BVAppLogic.h"
#include "../../UseLoggerBVAppModule.h"
#include "EventHandlerHelpers.h"
#include "Serialization/Json/JsonSerializeObject.h"
#include "Serialization/Json/JsonDeserializeObject.h"

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
    auto root = m_appLogic->GetBVProject()->GetModelSceneRoot();
        
    std::string& nodePath = widgetEvent->NodeName;
    std::string& sceneName = widgetEvent->SceneName;
    std::string& action = widgetEvent->Action;
    NodeLogicEvent::Command command = widgetEvent->WidgetCommand;

    auto node = m_appLogic->GetBVProject()->GetProjectEditor()->GetNode( sceneName, nodePath );
    if( node == nullptr )
    {
        SendSimpleErrorResponse( command, widgetEvent->EventID, widgetEvent->SocketID, "Node not found" );
    }

    BasicNodePtr basicNode = std::static_pointer_cast< bv::model::BasicNode >( node );
    INodeLogic* logic = basicNode->GetLogic().get();
	if( logic == nullptr )
	{
        LOG_MESSAGE( SeverityLevel::warning ) << "Error NodeLogicEvent node [" + nodePath + "] , logic [] not found";
        SendSimpleErrorResponse( command, widgetEvent->EventID, widgetEvent->SocketID, "NodeLogic not found" );
	}
		
    if( command == NodeLogicEvent::Command::AddNodeLogic )
    {

    }
    else if( command == NodeLogicEvent::Command::DeleteNodeLogic )
    {

    }
    else if ( command == NodeLogicEvent::Command::SetLogicParam )
    {
        JsonSerializeObject ser;
        JsonDeserializeObject deser;
        deser.Load( action );

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
		L"Jasiu kup kie�bas� !!",
		L"wieloj�zyczny projekt internetortej tre�ci. Funkcjonuje wykorzystuj�c",
		L"Wikipedia powsta�a 15 stycznia ert�w i nieistniej�cej ju� Nupedii. ",
		L"iostrzane. Wikipedia jest jedn�], a wiele stron uruchomi�o jej mirrory lub forki.",
		L"Wsp�za�o�yciel Wikipedii Jimmyia wieloj�zycznej",
		L"wolnej encyklopedii o najwy�szyw�asnym j�zyku�[8].",
		L"Kontrowersje budzi wiarygodno��e�ci artyku��w ",
		L"i brak weryfikacji kompetencji .",
		L"Z drugiej",
		L"strony mo�liwo�� swobodnej dyst �r�d�em informacji",
		L"Jasiu kup kie�bas� !!",
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

                SetParameter( pl->GetParameter( "text" ), 0.0, L"nowa wiadomo�� "+to_wstring(examplesIndex) );
                examplesIndex=(examplesIndex+1)%20;

				typedEvent->GetCrawler()->EnqueueNode( n );
			}
		}
	}
}

} //bv
