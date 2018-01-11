#pragma once

#include <map>

#include "AssetDescriptor.h"
#include "Assets/Asset.h"
#include "Assets/AssetLoader.h"
#include "Assets/AssetCache.h"

#include "AsyncLoading/LoadBarrier.h"

#include <mutex>


namespace bv
{


/**@brief Assets managment
@ingroup Assets*/
class AssetManager
{
    FRIEND_TEST_ACCESSOR( AssetManager );
private:

    std::map< std::string, AssetLoaderConstPtr >	m_loaders;
    AssetCache										m_assetCache;
    LoadBarrier                                     m_loadBarrier;

    std::mutex                                      m_lock;

public:

    AssetDescConstPtr				CreateDesc		( const IDeserializer & deserializer );
    AssetConstPtr					LoadAsset		( const AssetDescConstPtr & desc );
    bool							RegisterLoader	( const std::string & assetDescUID, const AssetLoaderConstPtr & loader );
    bool							UnregisterLoader( const std::string & assetDescUID );

    void                            AddToCache      ( AssetDescConstPtr & desc, AssetConstPtr asset );
    bool                            RemoveFromCache ( const std::string & assetKey );
    AssetConstPtr                   GetFromCache    ( const std::string & assetKey );

    ThumbnailConstPtr		        LoadThumbnail	( const AssetDescConstPtr & desc ) const;

    static AssetManager &			GetInstance		();

public:

    explicit						AssetManager();
                                    ~AssetManager();

private:

    void							RegisterBasicLoaders();

    AssetConstPtr                   LoadAssetImpl   ( const AssetDescConstPtr & desc );
};

// ***********************
//
template< typename AssetType >
std::shared_ptr< const AssetType > LoadTypedAsset		( const AssetDescConstPtr & desc )
{
    if( desc->GetUID() == GetAssetDescUID<AssetType>() )
    {
        auto asset = AssetManager::GetInstance().LoadAsset( desc );
        return std::static_pointer_cast<const AssetType>( asset );
    }
    return nullptr;
}



} // bv