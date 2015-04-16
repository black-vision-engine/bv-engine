#pragma once

#include "DataTypes/Hash.h"
#include "Memory/MemoryChunk.h"

#include <set>

namespace bv 
{

class DHRawDataCache
{
public:

	MemoryChunkConstPtr				Get				( const Hash & key ) const;

	bool							Add				( const Hash & key, const MemoryChunkConstPtr & memory, bool rewriteIfExists = false );

	static DHRawDataCache &			GetInstance		();

private:

	void							Initialize		();
	void							ScanCacheDir	();

	MemoryChunkConstPtr				Load			( const Hash & key ) const;


	DHRawDataCache		();
	~DHRawDataCache		();

	std::set< Hash >				m_entries;

};


} // bv