#pragma once

#include "DataTypes/Hash.h"
#include "Memory/MemoryChunk.h"

#include <set>
#include <mutex>



namespace bv 
{

class HardDriveRawDataCache
{
public:

	MemoryChunkConstPtr				Get				( const Hash & key ) const;

	bool							Add				( const Hash & key, const MemoryChunkConstPtr & memory, bool rewriteIfExists = false );

    // Function isn't thread safe.
	bool							Exists			( const Hash & key ) const;

	static HardDriveRawDataCache &			GetInstance		();

private:

	void							Initialize		();
	void							ScanCacheDir	();

	MemoryChunkConstPtr				Load			( const Hash & key ) const;


	HardDriveRawDataCache		();
	~HardDriveRawDataCache		();

	std::set< Hash >				m_entries;
    mutable std::recursive_mutex    m_lock;
};


} // bv