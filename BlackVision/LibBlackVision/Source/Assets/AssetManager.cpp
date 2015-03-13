#include "AssetManager.h"
#include "Assets.h"

#include <memory>

namespace bv
{

// ***********************
//
AssetConstPtr AssetManager::LoadAsset( const AssetDescConstPtr & desc ) const
{
	auto it = m_loaders.find( desc->GetUID() );

	if( it != m_loaders.end() )
	{
		auto asset = it->second->LoadAsset( desc );
		if( asset != nullptr )
		{
			return asset;
		}
	}

	return nullptr;
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