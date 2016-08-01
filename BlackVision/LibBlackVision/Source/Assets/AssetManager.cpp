#include "stdafx.h"

#include "AssetManager.h"
#include "Assets.h"
#include "Serialization/IDeserializer.h"

#include <memory>



#include "Memory/MemoryLeaks.h"



namespace bv
{

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
AssetConstPtr AssetManager::GetFromCache    ( AssetDescConstPtr& desc )
{
    return m_assetCache.Get( desc );
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
    AssetManager::GetInstance().RegisterLoader( TextureAssetDesc::UID(),	std::make_shared< TextureLoader >() );
    AssetManager::GetInstance().RegisterLoader( FontAssetDesc::UID(),		std::make_shared< FontLoader >() );
    AssetManager::GetInstance().RegisterLoader( AnimationAssetDesc::UID(),	std::make_shared< AnimationLoader >() );
    AssetManager::GetInstance().RegisterLoader( AVAssetDesc::UID(),	std::make_shared< AVAssetLoader >() );
    AssetManager::GetInstance().RegisterLoader( MeshAssetDesc::UID(),	std::make_shared< MeshLoader >() );
    AssetManager::GetInstance().RegisterLoader( DataArrayAssetDescriptor::UID(),	std::make_shared< DataArrayLoader >() );
    AssetManager::GetInstance().RegisterLoader( SVGAssetDescriptor::UID(),  std::make_shared< SVGLoader >() );
}



} // bv