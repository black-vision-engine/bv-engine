#include "stdafxBVApp.h"

#include "AssetHandlers.h"

#include "BVAppLogic.h"
#include "Engine/Models/BVProjectEditor.h"
#include "Engine/Events/EventHandlerHelpers.h"


namespace bv
{

// ***********************
//
AssetHandlers::AssetHandlers( BVAppLogic* appLogic )
    :   m_appLogic( appLogic )
{
}

// ***********************
//
AssetHandlers::~AssetHandlers()
{
}

// ***********************
//
void		AssetHandlers::CacheHandler			( bv::IEventPtr evt )
{
    if( evt->GetEventType() == bv::AssetEvent::Type() )
    {
        bv::AssetEventPtr assetEvent = std::static_pointer_cast< bv::AssetEvent >( evt );
        AssetEvent::Command command = assetEvent->AssetCommand;
        
        auto assetTracker = m_appLogic->GetBVProject()->GetAssetTracker();

        if( command == AssetEvent::Command::ClearCache )
        {
            assetTracker->ClearCache();
            SendSimpleResponse( command, assetEvent->EventID, assetEvent->SocketID, true );
        }
        else
            SendSimpleErrorResponse( command, assetEvent->EventID, assetEvent->SocketID, "Unknown command" );
    }
}

// *********************************
//
void        AssetHandlers::LoadAsset            ( bv::IEventPtr eventPtr )
{
    if( eventPtr->GetEventType() == LoadAssetEvent::Type() )
    {
        LoadAssetEventPtr eventLoadAsset = std::static_pointer_cast< LoadAssetEvent >( eventPtr );

        std::string & nodeName = eventLoadAsset->NodeName;
        std::string & pluginName = eventLoadAsset->PluginName;
        std::string & sceneName = eventLoadAsset->SceneName;
        auto & assetData = *eventLoadAsset->AssetData;

        assert( eventLoadAsset->AssetData != nullptr );
        if( eventLoadAsset->AssetData == nullptr )
        {
            SendSimpleErrorResponse( LoadAssetEvent::Command::LoadAsset, eventLoadAsset->EventID, eventLoadAsset->SocketID, "Wrong request" );
            return;
        }

        auto projectEditor = m_appLogic->GetBVProject()->GetProjectEditor();
        bool result = projectEditor->LoadAsset( sceneName, nodeName, pluginName, assetData );

        SendSimpleResponse( LoadAssetEvent::Command::LoadAsset, eventLoadAsset->EventID, eventLoadAsset->SocketID, result );
    }
}

} //bv
