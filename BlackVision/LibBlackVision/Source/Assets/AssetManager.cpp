#include "stdafx.h"

#include "AssetManager.h"
#include "Assets.h"
#include "Serialization/IDeserializer.h"

#include "Engine/Events/EventManager.h"
#include "Engine/Events/Events.h"

#include "Assets/Cache/RawDataCache.h"

#include <memory>


namespace bv {


// ***********************
//
AssetDescConstPtr AssetManager::CreateDesc( const IDeserializer& deserializer )
{
    bool success = deserializer.EnterChild( "asset" );
    if( !success )
        return nullptr;

    std::string assetUID  = deserializer.GetAttribute( "type" );

    auto it = m_loaders.find( assetUID );

    if( it != m_loaders.end() )
    {
        auto desc = it->second->CreateDescriptor( deserializer );
        deserializer.ExitChild();
        return desc;
    }

    deserializer.ExitChild();
    return nullptr;
}


// ***********************
//
AssetConstPtr AssetManager::LoadAsset( const AssetDescConstPtr & desc )
{
    if( m_assetCache.Exists( desc ) )
        return m_assetCache.Get( desc );
    else
    {
        auto it = m_loaders.find( desc->GetUID() );

        if( it != m_loaders.end() )
        {
            auto asset = it->second->LoadAsset( desc );
            if( asset != nullptr )
            {
                m_assetCache.Add( desc, asset );

                //TriggerEvent
                auto evt = std::make_shared< AssetTrackerInternalEvent >( AssetTrackerInternalEvent::Command::RegisterAsset );
                evt->AssetDesc = desc;
                GetDefaultEventManager().TriggerEvent( evt );

                return asset;
            }
        }

        return nullptr;
    }
}

// ***********************
//
ThumbnailConstPtr   AssetManager::LoadThumbnail	( const AssetDescConstPtr & desc ) const
{
    auto it = m_loaders.find( desc->GetUID() );

    if( it != m_loaders.end() )
    {
        return it->second->LoadThumbnail( desc );
    }
    else
    {
        return nullptr;
    }
}

// ***********************
//
bool AssetManager::RegisterLoader( const std::string & assetDescUID, const AssetLoaderConstPtr & loader )
{
    auto it = m_loaders.find( assetDescUID );

    if( it != m_loaders.end() )
    {
        return false;
    }
    else
    {
        m_loaders[ assetDescUID ] = loader;
        return true;
    }
}

// ***********************
//
bool AssetManager::UnregisterLoader( const std::string & assetDescUID )
{
    auto it = m_loaders.find( assetDescUID );

    if( it == m_loaders.end() )
        return false;
    else
    {
        m_loaders.erase( it );
        return true;
    }
}

// ***********************
//
void AssetManager::AddToCache               ( AssetDescConstPtr& desc, AssetConstPtr asset )
{
    m_assetCache.Add( desc, asset );
}

// ***********************
//
bool AssetManager::RemoveFromCache          ( const std::string & assetKey )
{
    auto assetKeyHash = Hash::FromString( assetKey );
    RawDataCache::GetInstance().Remove( assetKeyHash );

    return m_assetCache.Remove( assetKey );
}

// ***********************
//
AssetConstPtr AssetManager::GetFromCache    ( const std::string & assetKey )
{
    return m_assetCache.Get( assetKey );
}

// ***********************
//
AssetManager & AssetManager::GetInstance()
{
    static auto instance = AssetManager();
    return instance;
}

// ***********************
//
AssetManager::AssetManager()
{
    RegisterBasicLoaders();
}

// ***********************
//
AssetManager::~AssetManager()
{}

// ***********************
//
void AssetManager::RegisterBasicLoaders()
{
    RegisterLoader( TextureAssetDesc::UID(),	        std::make_shared< TextureLoader >() );
    RegisterLoader( FontAssetDesc::UID(),		        std::make_shared< FontLoader >() );
    RegisterLoader( AnimationAssetDesc::UID(),	        std::make_shared< AnimationLoader >() );
    RegisterLoader( AVAssetDesc::UID(),	                std::make_shared< AVAssetLoader >() );
    RegisterLoader( MeshAssetDesc::UID(),	            std::make_shared< MeshLoader >() );
    RegisterLoader( DataArrayAssetDescriptor::UID(),	std::make_shared< DataArrayLoader >() );
    RegisterLoader( SVGAssetDescriptor::UID(),          std::make_shared< SVGLoader >() );
}



} // bv