#include "ResourceManager.h"

namespace bv
{

// ***********************
//
model::IResourceNEWConstPtr ResourceManager::LoadResource( const ResourceDescConstPtr & desc ) const
{
	for( auto k : m_loaders )
		if( desc->Query( k.first ) )
			return k.second->LoadResource( desc );
	
	return nullptr;
}

// ***********************
//
bool ResourceManager::RegisterLoader( const std::string & resDescUID, model::IResourceLoader * loader )
{
	auto it = m_loaders.find( resDescUID );

	if( it != m_loaders.end() )
		return false;
	else
	{
		m_loaders[ resDescUID ] = loader;
		return true;
	}
}

// ***********************
//
bool ResourceManager::UnregisterLoader( const std::string & resDescUID )
{
	auto it = m_loaders.find( resDescUID );

	if( it == m_loaders.end() )
		return false;
	else
	{
		m_loaders.erase( it ); // FIXME: mem leak. Should the manager take ownership? Probably it should.
		return true;
	}
}

// ***********************
//
ResourceManager & ResourceManager::GetInstance()
{
	static auto instance = new ResourceManager();
	return *instance;
}

// ***********************
//
ResourceManager::ResourceManager()
{
	// FIXME: mem leak. Should the manager take ownership? Probably it should.
}


} // bv