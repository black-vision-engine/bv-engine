#include "AssetManager.h"

namespace bv
{

// ***********************
//
model::AssetConstPtr AssetManager::LoadAsset( const AssetDescConstPtr & desc ) const
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
bool AssetManager::RegisterLoader( const std::string & assetDescUID, model::AssetLoader * loader )
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
		auto ptr = it->second;
		m_loaders.erase( it );
		delete ptr;
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
{}

// ***********************
//
AssetManager::~AssetManager()
{
	for( auto it : m_loaders )
		delete it.second;
}

} // bv