#pragma once

#include "Assets/AssetDescriptor.h"

#include "Engine/Models/BasicNode.h"
#include "Engine/Models/SceneModel.h"
#include "Engine/Models/Plugins/Plugin.h"

#include <string>


namespace bv
{


// ***********************
// Asynchronous asset loading request info.
struct LoadAssetRequest
{
    model::SceneModelPtr    Scene;
    model::BasicNodePtr     Node;
    model::IPluginPtr       Plugin;

    AssetDescConstPtr       Descriptor;
    Int32                   RequestID;


// ***********************
//
    explicit LoadAssetRequest()
        :   Descriptor( nullptr )
        ,   RequestID( -1 )
    {}

    explicit LoadAssetRequest( model::SceneModelPtr scene, model::BasicNodePtr node, model::IPluginPtr plugin, AssetDescConstPtr desc, Int32 requestId )
        :   Scene( scene )
        ,   Node( node )
        ,   Plugin( plugin )
        ,   Descriptor( desc )
        ,   RequestID( requestId )
    {}
};


}	// bv
