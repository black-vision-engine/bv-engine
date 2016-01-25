#include "AssetCache.h"


namespace bv
{

// ******************************
//
AssetCache::AssetCache()
{
}

// ******************************
//
AssetCache::~AssetCache()
{
}

// ******************************
//
bool AssetCache::Add( const std::string& key, const AssetConstPtr& resource )
{
	if( Find( key ) )
		return false;
	else
	{
		Update( key, resource );
		return true;
	}
}

// ******************************
//
bool AssetCache::Add( const AssetDescConstPtr& descriptor, const AssetConstPtr& resource )
{
	return Add( descriptor->GetKey(), resource );
}


// ******************************
//
void AssetCache::Update( const std::string& key, const AssetConstPtr& resource )
{
	m_assets[ key ] = resource;
}
// ******************************
//
bool AssetCache::Exists	( const std::string& key ) const
{
	return Find( key ) != nullptr;
}

// ******************************
//
bool AssetCache::Exists( const AssetDescConstPtr& descriptor ) const
{
	return Exists( descriptor->GetKey() );
}

// ******************************
//
AssetConstPtr	AssetCache::Get( const std::string& key ) const
{
	return Find( key );
}

// ******************************
//
AssetConstPtr	AssetCache::Get( const AssetDescConstPtr& descriptor ) const
{
	auto asset = Find( descriptor->GetKey() );
	return asset;
}

// ******************************
//
AssetConstPtr	AssetCache::Find( const std::string& key ) const
{
	auto it = m_assets.find( key );
	if( it != m_assets.end() )
		return it->second;
	else
		return nullptr;
}

} //bv
