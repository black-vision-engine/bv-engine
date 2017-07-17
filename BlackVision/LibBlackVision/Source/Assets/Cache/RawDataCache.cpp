#include "stdafx.h"

#include "RawDataCache.h"
#include "HardDriveRawDataCache.h"


namespace bv {


// ******************************
//
MemoryChunkConstPtr     RawDataCache::Get( const Hash & key ) const
{
    std::lock_guard< std::recursive_mutex > guard( m_lock );
    return Find( key );
}

// ******************************
//
bool                    RawDataCache::Add( const Hash & key, MemoryChunkConstPtr data, bool addToHardDriveCache )
{
    std::lock_guard< std::recursive_mutex > guard( m_lock );

    if( Exists( key ) )
    {
        return false;
    }
    else
    {
        Update( key, data, addToHardDriveCache );
        return true;
    }
}

// ******************************
//
bool                    RawDataCache::Remove    ( const Hash & key )
{
    std::lock_guard< std::recursive_mutex > guard( m_lock );

    if ( Exists( key ) )
    {
        m_data.erase( key );
    }

    return false;
}

// ******************************
//
void                    RawDataCache::Update    ( const Hash & key, MemoryChunkConstPtr data, bool addToHardDriveCache )
{
    std::lock_guard< std::recursive_mutex > guard( m_lock );

    m_data[ key ] = data;

    // FIXME: This is thread safe only as long as no one except RawDataCache calls Exists function. 
    if( addToHardDriveCache && !m_hardDriveCache.Exists( key ) )
    {
        m_hardDriveCache.Add( key, data, true );
    }
}

// ******************************
//
RawDataCache &          RawDataCache::GetInstance()
{
    static RawDataCache instance;
    return instance;
}

// ******************************
//
MemoryChunkConstPtr     RawDataCache::Find( const Hash & key ) const
{
    auto it = m_data.find( key );
    if( it != m_data.end() )
    {
        return it->second;
    }
    else
    {
        return m_hardDriveCache.Get( key );
    }
}

// ******************************
//
bool                    RawDataCache::Exists( const Hash & key )
{
    std::lock_guard< std::recursive_mutex > guard( m_lock );

    return ( m_data.find( key ) != m_data.end() ) || m_hardDriveCache.Exists( key );
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
