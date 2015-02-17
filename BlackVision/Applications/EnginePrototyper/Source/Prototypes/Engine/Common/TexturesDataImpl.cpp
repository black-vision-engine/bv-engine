#include "TexturesDataImpl.h"

namespace bv {

// **************************
//
const std::vector< ITextureDescriptor * > &     TexturesDataImpl::GetTextures       () const
{
    return m_textures;
}

// **************************
//
const std::vector< IAnimationDescriptor * > &   TexturesDataImpl::GetAnimations     () const
{
    return m_animations;
}

// **************************
//
bool                                            TexturesDataImpl::AddTextureFromFile( const std::string & textureFile )
{
    { textureFile; }
    return false;
}

} // bv
