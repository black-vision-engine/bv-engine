#include "EventHandlerHelpers.h"
#include "../../BVAppLogic.h"


#include "../../UseLoggerBVAppModule.h"

namespace bv
{

// ***********************
//
bv::model::IModelNodePtr GetRootNode( BVAppLogic* appLogic, const std::string& sceneName )
{
    auto scene = appLogic->GetBVProject()->GetScene( sceneName );
    if( scene == nullptr )
    {
        LOG_MESSAGE( SeverityLevel::error ) << "Scene [" + sceneName +" ] not found";
        return nullptr;
    }

    auto root = scene->GetRootNode();
    return root;
}

// ***********************
//
bv::model::IModelNodePtr GetNode( BVAppLogic* appLogic, const std::string& sceneName, const std::string& nodeName )
{
    auto root = GetRootNode( appLogic, sceneName );
    if( root == nullptr ) return nullptr;

    auto node = root->GetNode( nodeName );
    if( node == nullptr )
    {
        if( root->GetName() == nodeName )
            node = root;
        else
        {
            LOG_MESSAGE( SeverityLevel::error ) << "Node [" + nodeName + "] not found";
            return nullptr;
        }
    }
    return node;
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
void SendOnSceneStructureResponse( int socketID, const std::string & cmd, const std::string & msgKey, const Json::Value & msgVal )
{
    LOG_MESSAGE( SeverityLevel::info ) << cmd << " OK";

    Json::Value scenes;

    scenes[ "cmd" ] = cmd;
    scenes[ msgKey ] = msgVal;

    std::string S = scenes.toStyledString();

    wstring WS = wstring( S.begin(), S.end() );

    ResponseEventPtr responseEvent = std::make_shared<ResponseEvent>();
    responseEvent->Response = WS;
    responseEvent->SocketID = socketID;
    GetDefaultEventManager().QueueResponse( responseEvent );
}



} //bv
