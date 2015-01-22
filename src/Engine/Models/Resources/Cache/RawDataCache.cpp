#include "RawDataCache.h"

namespace bv
{

// ******************************
//
MemoryChunkConstPtr		RawDataCache::Get( const std::string & key ) const
{
	return Find( key );
}

// ******************************
//
bool					RawDataCache::Add( const std::string & key, MemoryChunkConstPtr data )
{
	if( Exists( key ) )
		return false;
	else
	{
		Update( key, data );
		return true;
	}
}

// ******************************
//
void 					RawDataCache::Update	( const std::string & key, MemoryChunkConstPtr data )
{
	m_data[ key ] = data;
}

// ******************************
//
RawDataCache *			RawDataCache::GetInstance()
{
	static RawDataCache * instance = new RawDataCache();
	return instance;
}

// ******************************
//
MemoryChunkConstPtr		RawDataCache::Find( const std::string & key ) const
{
	auto it = m_data.find( key );
	if( it != m_data.end() )
		return it->second;
	else
		return nullptr;
}

// ******************************
//
RawDataCache::RawDataCache()
{}

// ******************************
//
RawDataCache::~RawDataCache()
{}


} // bv
