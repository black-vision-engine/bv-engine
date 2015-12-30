#include "AssetTracker.h"

#include "Engine/Graphics/Resources/Textures/Texture2DCache.h"


namespace bv {

// *************************************
//
                        AssetTracker::AssetTracker          ()
{
}

// *************************************
//
void                    AssetTracker::RegisterAsset         ( TextureConstPtr asset )
{
    if( m_registeredAssetsMap.count( asset ) == 0 )
    {
        m_registeredAssetsMap[ asset ] = 0;
    }

    if( m_unregisteredAssets.count( asset ) )
    {
        m_unregisteredAssets.erase( asset );
    }

    m_registeredAssetsMap[ asset ] += 1;
}

// *************************************
//
void                    AssetTracker::UnregisterAsset       ( TextureConstPtr asset )
{
    if( m_registeredAssetsMap.empty() || m_registeredAssetsMap.count( asset ) == 0 ) return;

    if( m_registeredAssetsMap[ asset ] <= 2 ) // asset is registered twice (plugin & finalize plugin)
    {
        m_registeredAssetsMap.erase( asset );
        m_unregisteredAssets.insert( asset );
        return;
    }
    
    m_registeredAssetsMap[ asset ] -= 1;
}

// *************************************
//
void                    AssetTracker::RegisterAsset         ( ITextureDescriptorConstPtr asset )
{
    auto tex = GTexture2DCache.GetTexture( asset.get() );
    RegisterAsset( tex );
}

// *************************************
//
void                    AssetTracker::UnregisterAsset     ( ITextureDescriptorConstPtr asset )
{
    if( GTexture2DCache.IsRegistered( asset.get() ) )
    {
        auto tex = GTexture2DCache.GetTexture( asset.get() );
        UnregisterAsset( tex );
    }
}

// *************************************
//
std::vector< TextureConstPtr >      AssetTracker::GetUnusedAssets       ()
{
    std::vector< TextureConstPtr > ret( m_unregisteredAssets.begin(), m_unregisteredAssets.end() );
    m_unregisteredAssets.clear();
    return ret;
}

// *************************************
//
AssetTracker &       AssetTracker::Instance                 ()
{
    static AssetTracker instance;
    return instance;
}

} //bv
