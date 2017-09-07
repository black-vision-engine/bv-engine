#pragma once

#include "Assets/Asset.h"
#include "Assets/AssetDescriptor.h"

#include <map>
#include <mutex>


namespace bv {


class AssetCache
{
private:

    std::map< std::string, AssetConstPtr >      m_assets;
    mutable std::recursive_mutex                m_lock;

private:

    AssetConstPtr                   Find                ( const std::string & key ) const;

public:

                                    AssetCache  ();
                                    ~AssetCache ();

    // **************************
    // Adds entry to cache if doesn't exist.
    bool                            Add     ( const std::string & key, const AssetConstPtr & resource );
    bool                            Add     ( const AssetDescConstPtr & descriptor, const AssetConstPtr & resource );

    // Removes asset with given key.
    bool                            Remove  ( const std::string & key );

    // **************************
    // Adds entry to cache if doesn't exist or update if exists.
    void                            Update  ( const std::string & key, const AssetConstPtr& resource );

    AssetConstPtr                   Get     ( const std::string & key ) const;
    AssetConstPtr                   Get     ( const AssetDescConstPtr & descriptor ) const;

    // ***********************
    // These functions are not thread safe. You should Get asset and compare with nullptr.
    bool                            Exists  ( const std::string & key ) const;
    bool                            Exists  ( const AssetDescConstPtr & descriptor ) const;

};

} //bv
