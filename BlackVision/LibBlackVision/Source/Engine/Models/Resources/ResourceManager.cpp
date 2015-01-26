#include "ResourceManager.h"

namespace bv
{

// ***********************
//
model::IResourceNEWConstPtr ResourceManager::LoadResource( const ResourceDescConstPtr & desc ) const
{
	auto it = m_loaders.find( desc->GetUID() );
	for( auto k : m_loaders )
		if( desc->GetUID() == k.first )
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
		auto ptr = it->second;
		m_loaders.erase( it );
		delete ptr;
		return true;
	}
}

// ***********************
//
ResourceManager & ResourceManager::GetInstance()
{
	static auto instance = ResourceManager();
	return instance;
}

// ***********************
//
ResourceManager::ResourceManager()
{}

// ***********************
//
ResourceManager::~ResourceManager()
{
	for( auto it : m_loaders )
		delete it.second;
}

} // bv