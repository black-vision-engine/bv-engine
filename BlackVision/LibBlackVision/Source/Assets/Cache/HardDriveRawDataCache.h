#pragma once

#include "DataTypes/Hash.h"
#include "Memory/MemoryChunk.h"

#include <set>

namespace bv 
{

class HardDriveRawDataCache
{
public:

	MemoryChunkConstPtr				Get				( const Hash & key ) const;

	bool							Add				( const Hash & key, const MemoryChunkConstPtr & memory, bool rewriteIfExists = false );

	bool							Exists			( const Hash & key ) const;

	static HardDriveRawDataCache &			GetInstance		();

private:

	void							Initialize		();
	void							ScanCacheDir	();

	MemoryChunkConstPtr				Load			( const Hash & key ) const;


	HardDriveRawDataCache		();
	~HardDriveRawDataCache		();

	std::set< Hash >				m_entries;

};


} // bv