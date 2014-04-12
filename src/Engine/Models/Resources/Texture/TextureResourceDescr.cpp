#include "TextureResourceDescr.h"


namespace bv { namespace model {

// *******************************
//
TextureResourceDescr::TextureResourceDescr                  ( const std::string & textureFile, const std::string & name )
    : m_name( name )
    , m_textureFile( textureFile )
{
}

// *******************************
//
TextureResourceDescr::~TextureResourceDescr                 ()
{
}

// *******************************
//
const std::string & TextureResourceDescr::GetName           () const
{
    return m_name;
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

} //model
} //bv
