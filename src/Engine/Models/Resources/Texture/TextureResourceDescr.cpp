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
IAnimationResourceDescrConstPtr   QueryAnimationResourceDescr   ( IPluginResourceDescrConstPtr resDescr )
{
    if ( resDescr->GetResourceType() == PluginResourceType::PRT_ANIMATION )
    {
        return std::static_pointer_cast< const IAnimationResourceDescr >( resDescr );
    }

    return nullptr;
}

} //model
} //bv
