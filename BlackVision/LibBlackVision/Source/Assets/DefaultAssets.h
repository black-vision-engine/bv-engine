#pragma once

#include <cassert>

#include "AssetDescriptor.h"
#include "Assets/Asset.h"

#include "Assets/Texture/TextureAssetDescriptor.h"
#include "Assets/Texture/AnimationAssetDescriptor.h"
#include "Assets/Font/2D/FontAssetDescriptor.h"
#include "Assets/AVStream/AVAssetDescriptor.h"
#include "Assets/DataArray/DataArrayAssetDescriptor.h"
#include "Assets/Input/VideoInput/VideoInputAssetDesc.h"

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
        std::string     texture2DTransparentPath;
        std::string     texture2DFallbackPath;

        std::string     fontName;
        UInt32          fontSize;

        std::string     animation2DDir;
        std::string     animationFilter;

        std::string     videoStreamPath;
        TextureFormat   videoStreamFormat;
    };

    enum class Tex2DFallback
    {
        None,
        Checker,
        Transparent
    };

private:

    Config              m_config;                

private:

                        DefaultAssets           ();

public:
    
    template< typename AssetDescType >
    inline std::shared_ptr< const AssetDescType >   GetDefaultDesc   ();

    template< typename AssetDescType >
    inline std::shared_ptr< const AssetDescType >   GetFallbackDesc  ();

public:

    void                InitTex2DFallback       ( Tex2DFallback fallback );



    static DefaultAssets &                          Instance    ();

};

} // bv

#include "DefaultAssets.inl"
