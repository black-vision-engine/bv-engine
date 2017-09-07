#include "stdafx.h"

#include "AssetManager.h"
#include "Assets.h"
#include "Serialization/IDeserializer.h"

#include "Engine/Events/EventManager.h"
#include "Engine/Events/Events.h"

#include "Assets/Cache/RawDataCache.h"
#include "Assets/Cache/HardDriveRawDataCache.h"

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
    std::unique_lock< std::mutex > guard( m_lock );

    auto asset = m_assetCache.Get( desc );
    if( !asset )
    {
        auto result = m_loadBarrier.RequestAsset( desc );
        
        guard.unlock();

        WaitingAsset* assetWait = result.first;
        bool needWait = result.second;

        if( needWait )
            asset = m_loadBarrier.WaitUntilLoaded( assetWait );
        else
            asset = LoadAssetImpl( desc );
    }

    return asset;
}

// ***********************
//
AssetConstPtr       AssetManager::LoadAssetImpl ( const AssetDescConstPtr & desc )
{
    auto it = m_loaders.find( desc->GetUID() );

    if( it != m_loaders.end() )
    {
        auto asset = it->second->LoadAsset( desc );
        if( asset != nullptr )
        {
            if( m_assetCache.Add( desc, asset ) )
            {
                GetDefaultEventManager().TriggerEvent( std::make_shared< AssetTrackerInternalEvent >( AssetTrackerInternalEvent::Command::RegisterAsset, desc->GetKey() ) );
            }

            m_loadBarrier.LoadingCompleted( desc, asset );

            return asset;
        }
    }

    // Loading failed but we must wake all waiting threads.
    m_loadBarrier.LoadingCompleted( desc, nullptr );
    return nullptr;
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
    static AssetManager instance;
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
    RegisterLoader( FontAssetDesc::UID(),		        std::make_shared< Font2DLoader >() );
    RegisterLoader( FontAsset3DDesc::UID(),             std::make_shared< Font3DLoader >() );
    RegisterLoader( AnimationAssetDesc::UID(),	        std::make_shared< AnimationLoader >() );
    RegisterLoader( AVAssetDesc::UID(),	                std::make_shared< AVAssetLoader >() );
    RegisterLoader( MeshAssetDesc::UID(),	            std::make_shared< MeshLoader >() );
    RegisterLoader( DataArrayAssetDesc::UID(),	std::make_shared< DataArrayLoader >() );
    RegisterLoader( SVGAssetDescriptor::UID(),          std::make_shared< SVGLoader >() );

    RawDataCache::GetInstance();
}




} // bv