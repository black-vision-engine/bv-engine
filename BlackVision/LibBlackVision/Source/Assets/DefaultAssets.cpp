#include "stdafx.h"

#include "DefaultAssets.h"




#include "Memory/MemoryLeaks.h"



namespace bv
{

// *********************************
//
                    DefaultAssets::DefaultAssets        ()
{
    m_config.texture2DPath = "file:/Assets/DefaultAssets/textures/2D/default";
    m_config.texture2DTransparentPath = "file:/Assets/DefaultAssets/textures/2D/defaultTransparent.png";
    InitTex2DFallback( Tex2DFallback::Checker );

    m_config.animation2DDir = "seq:/Assets/DefaultAssets/animations/2D";
    m_config.animationFilter = ".*";

    m_config.fontName = "file:/Assets/DefaultAssets/fonts/default.ttf";
    m_config.fontSize = 24;

    m_config.videoStreamPath = "Assets/DefaultAssets/videostream/default";
    m_config.videoStreamFormat = TextureFormat::F_A8R8G8B8;
}

// ***********************
//
void                DefaultAssets::InitTex2DFallback    ( Tex2DFallback fallback )
{
    switch( fallback )
    {
    case Tex2DFallback::None:
        m_config.texture2DFallbackPath = "";
        break;
    case Tex2DFallback::Checker:
        m_config.texture2DFallbackPath = m_config.texture2DPath;
        break;
    case Tex2DFallback::Transparent:
        m_config.texture2DFallbackPath = m_config.texture2DTransparentPath;
        break;
    default:
        m_config.texture2DFallbackPath = m_config.texture2DPath;
        break;
    }
}

                    // *********************************
//
DefaultAssets &     DefaultAssets::Instance             ()
{
    static DefaultAssets instance;
    return instance;
}

} // bv