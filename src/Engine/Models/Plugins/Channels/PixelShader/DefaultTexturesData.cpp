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
    for( auto tx : m_textureDescriptors )
    {
        delete tx;
    }

    for( auto anim : m_animationDescriptors )
    {
        delete anim;
    }
}

// ******************************
//
const std::vector< ITextureDescriptor * > &     DefaultTexturesData::GetTextures     () const
{
    return m_textureDescriptors;
}

// ******************************
//
const std::vector< IAnimationDescriptor * > &   DefaultTexturesData::GetAnimations   () const
{
    return m_animationDescriptors;
}

// ******************************
//
void                                            DefaultTexturesData::SetTexture      ( unsigned int idx, DefaultTextureDescriptor * textureDesc )
{
    assert( idx < m_textureDescriptors.size() );

    delete m_textureDescriptors[ idx ];
    m_textureDescriptors[ idx ] = textureDesc;
}

// ******************************
//
void                                            DefaultTexturesData::AddTexture      ( DefaultTextureDescriptor * textureDesc )
{
    m_textureDescriptors.push_back( textureDesc );
}

// ******************************
//
void                                            DefaultTexturesData::SetAnimation    ( unsigned int idx, DefaultAnimationDescriptor * animationDesc )
{
    assert( idx < m_animationDescriptors.size() );

    delete m_animationDescriptors[ idx ];
    m_animationDescriptors[ idx ] = animationDesc;
}

// ******************************
//
void                                            DefaultTexturesData::AddAnimation    ( DefaultAnimationDescriptor * animationDesc )
{
    m_animationDescriptors.push_back( animationDesc );
}

} //model
} //bv
