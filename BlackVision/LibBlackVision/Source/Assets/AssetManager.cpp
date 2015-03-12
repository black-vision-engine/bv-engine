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
		auto res = it->second->LoadAsset( desc );
		if( res != nullptr )
		{
			return res;
		}
	}

	return nullptr;
}

// ***********************
//
bool AssetManager::RegisterLoader( const std::string & resDescUID, model::AssetLoader * loader )
{
	auto it = m_loaders.find( resDescUID );

	if( it != m_loaders.end() )
	{
		return false;
	}
	else
	{
		m_loaders[ resDescUID ] = loader;
		return true;
	}
}

// ***********************
//
bool AssetManager::UnregisterLoader( const std::string & resDescUID )
{
	auto it = m_loaders.find( resDescUID );

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