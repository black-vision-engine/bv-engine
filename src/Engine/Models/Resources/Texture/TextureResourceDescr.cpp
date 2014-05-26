#include "TextureResourceDescr.h"


namespace bv { namespace model {

// *******************************
//
TextureResourceDescr::TextureResourceDescr                  ( const std::string & textureFile )
    : m_textureFile( textureFile )
{
}

// *******************************
//
TextureResourceDescr::~TextureResourceDescr                 ()
{
}

// *******************************
//
PluginResourceType  TextureResourceDescr::GetResourceType   () const
{
    return PluginResourceType::PRT_TEXTURE;
}

// *******************************
//
const std::string & TextureResourceDescr::GetTextureFile    () const
{
    return m_textureFile;
}

// *******************************
//
const IAnimationResourceDescr *   QueryAnimationResourceDescr   ( const IPluginResourceDescr * resDescr )
{
    if ( resDescr->GetResourceType() == PluginResourceType::PRT_ANIMATION )
    {
        return static_cast< const IAnimationResourceDescr * >( resDescr );
    }

    return nullptr;
}

} //model
} //bv
