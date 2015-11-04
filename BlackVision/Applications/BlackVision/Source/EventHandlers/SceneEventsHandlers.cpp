#include "SceneEventsHandlers.h"

#include "Engine/Models/BVSceneEditor.h"
#include "Engine/Models/ModelNodeEditor.h"
#include "../BVAppLogic.h"
#include "../UseLogger.h"
#include "ProjectManager.h"
#include "Tools/IncludeJSON.h"

#include <limits>
#undef max

namespace bv
{

// ***********************
//
SceneEventsHandlers::SceneEventsHandlers( BVAppLogic* logic )
    : m_appLogic( logic )
{}

SceneEventsHandlers::~SceneEventsHandlers()
{}

// ***********************
//
void SceneEventsHandlers::SceneStructure      ( bv::IEventPtr evt )
{
    if( evt->GetEventType() != bv::SceneStructureEvent::Type() )
        return;
    bv::SceneStructureEventPtr structureEvent = std::static_pointer_cast<bv::SceneStructureEvent>( evt );

    std::string& nodeName = structureEvent->NodeName;
    std::string& newNodeName = structureEvent->NewNodeName;
    std::string& pluginName = structureEvent->PluginName;
    auto command = structureEvent->SceneCommand;

    auto root = m_appLogic->GetBVScene()->GetModelSceneRoot();
    auto node = root->GetNode( nodeName );
    if( node == nullptr )
    {
        if( root->GetName() == nodeName )
            node = root;
        else
        {
            LOG_MESSAGE( SeverityLevel::error ) << "SceneStructureEvent() node ["+ nodeName +"] not found";
            return;
        }
    }

    if( command == SceneStructureEvent::Command::AddNode )
    {
        auto newNode = model::BasicNode::Create( newNodeName, m_appLogic->GetTimelineManager()->GetRootTimeline() );
		newNode->AddPlugin( "DEFAULT_TRANSFORM", "transform", m_appLogic->GetTimelineManager()->GetRootTimeline() ); 

		m_appLogic->GetBVScene()->GetSceneEditor()->AddChildNode( node, newNode );
    }
    else if( command == SceneStructureEvent::Command::RemoveNode )
    {
        auto parentNodeName = nodeName.substr( 0, nodeName.find_last_of("/") );
        auto childNode = nodeName.substr( nodeName.find_last_of("/") + 1 );
        auto parentNode = root->GetNode( parentNodeName );
			
        m_appLogic->GetBVScene()->GetSceneEditor()->DeleteChildNode( parentNode, childNode );
    }
    else if( command == SceneStructureEvent::Command::AttachPlugin )
    {
        bv::model::BasicNodePtr basicNode = std::static_pointer_cast< bv::model::BasicNode >( node );
        
        unsigned int endIndex = std::numeric_limits<unsigned int>::max();
        basicNode->GetModelNodeEditor()->AttachPlugin( endIndex );
    }
    else if( command == SceneStructureEvent::Command::DetachPlugin )
    {
        bv::model::BasicNodePtr basicNode = std::static_pointer_cast< bv::model::BasicNode >( node );
        basicNode->GetModelNodeEditor()->DetachPlugin( pluginName );
    }
    else if( command == SceneStructureEvent::Command::SetNodeVisible )
        node->SetVisible( true );
    else if( command == SceneStructureEvent::Command::SetNodeInvisible )
        node->SetVisible( false );

}


// *********************************
//
Json::Value ToJSONArray( const PathVec & v )
{
    Json::Value root;

    for( auto pn : v )
    {
        root.append( pn.Str() );
    }

    return root;
}

// *********************************
//
Json::Value ToJSONArray( const StringVector & v )
{
    Json::Value root;

    for( auto s : v )
    {
        root.append( s );
    }

    return root;
}
// *********************************
//
Json::Value Str2Json( const std::string & data )
{
    return Json::Value( data );
}

// *********************************
//
Json::Value GetRequestParamValue( std::string& request )
{
    return Str2Json( std::string( request.begin(), request.end() ) );
}

// *********************************
//
void SendOnSceneStructureResponse( const std::string & cmd, const std::string & msgKey, const Json::Value & msgVal )
{
    //Log::A( "OK", cmd );

    Json::Value scenes;

    scenes[ "cmd" ] = cmd;
    scenes[ msgKey ] = msgVal;

    std::string S = scenes.toStyledString();

    wstring WS = wstring( S.begin(), S.end() );

    //ResponseMsg msg;
    //msg.msg     = WS;
    //msg.sock_id = evtStructure->sock_id;
    //SocketWrapper::AddMsg( msg );
}

// ***********************
//
void SceneEventsHandlers::ProjectStructure    ( bv::IEventPtr evt )
{
    if( evt->GetEventType() != bv::ProjectStructureEvent::Type() )
        return;
    bv::ProjectStructureEventPtr projectEvent = std::static_pointer_cast<bv::ProjectStructureEvent>( evt );

    auto pm = ProjectManager::GetInstance();

    std::string& request = projectEvent->Request;
    auto command = projectEvent->ProjectCommand;

        if( command == ProjectStructureEvent::Command::ListProjectNames )
        {
            auto pns = pm->ListProjectsNames();

            auto pList = ToJSONArray( pns );

            SendOnSceneStructureResponse( "LIST_PROJECTS_NAMES", "list", pList );
        }
        else if( command == ProjectStructureEvent::Command::NewProject )
        {
            auto name = GetRequestParamValue( request )[ "projectName" ].asString();

            pm->AddNewProject( name );

            SendOnSceneStructureResponse( "NEW_PROJECT", "status", "OK" );
        }
        else if( command == ProjectStructureEvent::Command::ListScenes )
        {
            auto name = GetRequestParamValue( request )[ "projectName" ].asString();
            auto sns = pm->ListScenesNames( name );

            auto pList = ToJSONArray( sns );

            SendOnSceneStructureResponse( "LIST_SCENES", "list", pList );
        }
        else if( command == ProjectStructureEvent::Command::ListAssetsPaths )
        {
            auto projName = GetRequestParamValue( request )[ "projectName" ].asString();
            auto catName = GetRequestParamValue( request )[ "categoryName" ].asString();

            auto sns = pm->ListAssetsPaths( projName, catName );

            auto pList = ToJSONArray( sns );

            SendOnSceneStructureResponse( "LIST_ASSETS_PATHS", "list", pList );
        }
        else if( command == ProjectStructureEvent::Command::ListCategoriesNames )
        {
            auto sns = pm->ListCategoriesNames();

            auto pList = ToJSONArray( sns );

            SendOnSceneStructureResponse( "LIST_CATEGORIES_NAMES", "list", pList );
        }
        else if( command == ProjectStructureEvent::Command::SetCurrentProject )
        {
            auto projName = GetRequestParamValue( request )[ "projectName" ].asString();

            pm->SetCurrentProject( projName );

            SendOnSceneStructureResponse( "SET_CURRENT_PROJECT", "status", "OK" );
        }
        else if( command == ProjectStructureEvent::Command::ListProjects )
        {
            auto pns = pm->ListProjectsNames();

            Json::Value list;

            for( auto p : pns )
            {
                auto scenesCount = pm->ListScenesNames( p ).size();

                Json::Value entry;
                entry[ "name" ] = p.Str();
                entry[ "scenes_count" ] = scenesCount;

                list.append( entry );
            }

            SendOnSceneStructureResponse( "LIST_PROJECTS", "list", list );
        }
}


} //bv
