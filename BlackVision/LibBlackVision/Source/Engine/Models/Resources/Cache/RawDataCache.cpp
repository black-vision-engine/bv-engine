#include "RawDataCache.h"

namespace bv
{

// ******************************
//
MemoryChunkConstPtr		RawDataCache::Get( const Hash & key ) const
{
	return Find( key );
}

// ******************************
//
bool					RawDataCache::Add( const Hash & key, MemoryChunkConstPtr data )
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
void 					RawDataCache::Update	( const Hash & key, MemoryChunkConstPtr data )
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
MemoryChunkConstPtr		RawDataCache::Find( const Hash & key ) const
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
