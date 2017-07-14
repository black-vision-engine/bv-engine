#pragma once

#include "Assets/AssetDescriptor.h"

#include <string>


namespace bv
{


// ***********************
// Asynchronous asset loading request info.
struct LoadAssetRequest
{
    std::string             SceneName;
    std::string             NodePath;
    std::string             PluginName;
    AssetDescConstPtr       Descriptor;
    Int32                   RequestID;


// ***********************
//
    explicit LoadAssetRequest()
        :   Descriptor( nullptr )
        ,   RequestID( -1 )
    {}

    explicit LoadAssetRequest( const std::string & sceneName, const std::string & nodePath, const std::string & pluginName, AssetDescConstPtr desc, Int32 requestId )
        :   SceneName( sceneName )
        ,   NodePath( nodePath )
        ,   PluginName( pluginName )
        ,   Descriptor( desc )
        ,   RequestID( requestId )
    {}
};


}	// bv
