#include "AssetManager.h"
#include "Assets.h"
//#include "Serialization/Json/JsonDeserializeObject.h"

#include <memory>

#include <cassert>	//Delete after implementing function CreateDesc

namespace bv
{

// ***********************
//
AssetDescConstPtr AssetManager::CreateDesc( ISerializer& deserializer )
{
	//JsonDeserializeObject deserializeObject;
 //   deserializeObject.Load( jsonString );

    bool success = deserializer.EnterChild( "asset" );
	if( !success )
		return nullptr;

    std::string assetUID  = deserializer.GetAttribute( "uid" );
    //deserializeObject.ExitChild();

	auto it = m_loaders.find( assetUID );

	if( it != m_loaders.end() )
		return it->second->CreateDescriptor( deserializer );

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
}



} // bv