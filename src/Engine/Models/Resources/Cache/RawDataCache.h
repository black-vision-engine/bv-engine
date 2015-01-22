#pragma once

#include "Core/MemoryChunk.h"

#include <string>
#include <map>

namespace bv
{

// ******************************
// Implementation of simple cache of MemoryChunk. Key can be every string. Mostly hash.
class RawDataCache
{
public:

	MemoryChunkConstPtr		Get		( const std::string & key ) const;

	// ******************************
	// Adds entry to cache. If doesn't exist return false else returns true and adds entry
	bool					Add		( const std::string & key, MemoryChunkConstPtr data );

	// ******************************
	// Updates entry in cache. If doesn't exist adds it.
	void					Update	( const std::string & key, MemoryChunkConstPtr data );

	// ******************************
	// 
	bool					Exists	( const std::string & key );

	// ******************************
	// Returns instance of the class singleton. 
	static RawDataCache *	GetInstance();

private:
	RawDataCache();
	~RawDataCache();

	MemoryChunkConstPtr		Find( const std::string & key ) const;

	std::map< std::string, MemoryChunkConstPtr > m_data;
};

} // bv
