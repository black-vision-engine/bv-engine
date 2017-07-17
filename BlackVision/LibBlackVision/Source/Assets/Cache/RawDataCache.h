#pragma once

#include "DataTypes/Hash.h"
#include "Memory/MemoryChunk.h"

#include <map>
#include <mutex>


namespace bv
{

// ******************************
// Implementation of simple cache of MemoryChunk. Key must be class of bv::Hash.
class RawDataCache
{
public:

    MemoryChunkConstPtr     Get     ( const Hash & key ) const;

    // ******************************
    // Adds entry to cache. If doesn't exist return false else returns true and adds entry
    bool                    Add     ( const Hash & key, MemoryChunkConstPtr data, bool addToHardDriveCache = false );

    // ******************************
    // Removes entry from cache.
    bool                    Remove  ( const Hash & key );

    // ******************************
    // Updates entry in cache. If doesn't exist adds it.
    void                    Update  ( const Hash & key, MemoryChunkConstPtr data, bool addToHardDriveCache = false );

    // ******************************
    // 
    bool                    Exists  ( const Hash & key );

    // ******************************
    // Returns instance of the class singleton. 
    static RawDataCache &   GetInstance();

private:
    RawDataCache();
    ~RawDataCache();

    MemoryChunkConstPtr     Find( const Hash & key ) const;

    std::map< Hash, MemoryChunkConstPtr >   m_data;
    mutable std::recursive_mutex            m_lock;

};

} // bv
