#include "DefaultTexturesData.h"

namespace bv { namespace model {

// ******************************
//
DefaultTexturesData::DefaultTexturesData ()
{
}

// ******************************
//
DefaultTexturesData::~DefaultTexturesData()
{
}

// ******************************
//
const std::vector< ITextureDescriptor * > &     DefaultTexturesData::GetTextures     () const
{
    return m_textureDescritpors;
}

// ******************************
//
const std::vector< IAnimationDescriptor * > &   DefaultTexturesData::GetAnimations   () const
{
    return m_animationDescritpors;
}

} //model
} //bv
