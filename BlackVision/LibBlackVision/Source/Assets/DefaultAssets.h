#pragma once

#include <cassert>

#include "AssetDescriptor.h"
#include "Assets/Asset.h"

#include "Assets/Texture/TextureAssetDescriptor.h"
#include "Assets/Texture/AnimationAssetDescriptor.h"
#include "Assets/Font/FontAssetDescriptor.h"
#include "Assets/VideoStream/VideoStreamAssetDescriptor.h"

#include "Engine/Types/EnumsUtils.h"
#include "ProjectManager.h"


namespace bv
{

class DefaultAssets
{
public:

    struct Config
    {
        std::string     texture2DPath;

        std::string     fontName;
        UInt32          fontSize;

        std::string     animation2DDir;
        std::string     animationFilter;

        std::string     videoStreamPath;
        TextureFormat   videoStreamFormat;
    };

private:

    Config              m_config;                

private:

                        DefaultAssets           ();

public:
    
    template< typename AssetDescType >
    inline std::shared_ptr< const AssetDescType >   GetDefaultDesc   ();
    
    static DefaultAssets &                          Instance    ();

};

} // bv

#include "DefaultAssets.inl"
