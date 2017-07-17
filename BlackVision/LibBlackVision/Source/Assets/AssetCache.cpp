#include "stdafx.h"

#include "AssetCache.h"


namespace bv {


// ******************************
//
AssetCache::AssetCache()
{}

// ******************************
//
AssetCache::~AssetCache()
{}

// ******************************
//
bool        AssetCache::Add         ( const std::string& key, const AssetConstPtr& resource )
{
    std::lock_guard< std::recursive_mutex > guard( m_lock );

    if( !Find( key ) )
    {
        Update( key, resource );
        return true;
    }
    return false;
}

// ******************************
//
bool        AssetCache::Add         ( const AssetDescConstPtr& descriptor, const AssetConstPtr& resource )
{
    std::lock_guard< std::recursive_mutex > guard( m_lock );

    return Add( descriptor->GetKey(), resource );
}

// ******************************
//
bool        AssetCache::Remove      ( const std::string & key )
{
    std::lock_guard< std::recursive_mutex > guard( m_lock );

    if( Find( key ) )
    {
        m_assets.erase( key );
        return true;
    }
    return false;
}

// ******************************
//
void        AssetCache::Update      ( const std::string& key, const AssetConstPtr& resource )
{
    std::lock_guard< std::recursive_mutex > guard( m_lock );

    m_assets[ key ] = resource;
}

// ******************************
//
bool        AssetCache::Exists      ( const std::string& key ) const
{
    std::lock_guard< std::recursive_mutex > guard( m_lock );
    return Find( key ) != nullptr;
}

// ******************************
//
bool        AssetCache::Exists      ( const AssetDescConstPtr& descriptor ) const
{
    return Exists( descriptor->GetKey() );
}

// ******************************
//
AssetConstPtr   AssetCache::Get     ( const std::string& key ) const
{
    std::lock_guard< std::recursive_mutex > guard( m_lock );

    return Find( key );
}

// ******************************
//
AssetConstPtr   AssetCache::Get     ( const AssetDescConstPtr& descriptor ) const
{
    std::lock_guard< std::recursive_mutex > guard( m_lock );

    auto asset = Find( descriptor->GetKey() );
    return asset;
}

// ******************************
//
AssetConstPtr   AssetCache::Find    ( const std::string& key ) const
{
    auto it = m_assets.find( key );
    if( it != m_assets.end() )
    {
        return it->second;
    }
    
    return nullptr;
}

} //bv
