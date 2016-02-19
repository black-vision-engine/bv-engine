#pragma once

#include <map>
#include <set>

#include "Engine/Models/Interfaces/ITextureDescriptor.h"
#include "Engine/Graphics/Resources/Textures/Texture.h"


namespace bv { 

//FIXME: AssetTracker tracks only textures (engine). There is no need to track anything more for now, but it should be extended to all kind of assets.
class AssetTracker
{

private:

    std::map< TextureConstPtr, UInt32 >     m_registeredAssetsMap;
    std::set< TextureConstPtr >             m_unregisteredAssets;

private:

                                            AssetTracker        ();

public:

    void                                    RegisterAsset       ( TextureConstPtr asset );
    void                                    UnregisterAsset     ( TextureConstPtr asset );
    
    void                                    RegisterAsset       ( ITextureDescriptorConstPtr asset );
    void                                    UnregisterAsset     ( ITextureDescriptorConstPtr asset );

    std::vector< TextureConstPtr >          GetUnusedAssets     ();

    static AssetTracker &                   Instance            ();

};

} //bv
