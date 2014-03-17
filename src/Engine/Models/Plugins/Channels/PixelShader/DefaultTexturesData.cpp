#include "DefaultTexturesData.h"

#include "Engine/Models/Plugins/Channels/PixelShader/DefaultTextureDescriptor.h"
#include "Engine/Models/Plugins/Channels/PixelShader/DefaultAnimationDescriptor.h"


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
    for( auto tx : m_textureDescritpors )
    {
        delete tx;
    }

    for( auto anim : m_animationDescritpors )
    {
        delete anim;
    }
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

// ******************************
//
void                                            DefaultTexturesData::AddTexture      ( ITextureDescriptor * textureDesc )
{
    m_textureDescritpors.push_back( textureDesc );
}

// ******************************
//
void                                            DefaultTexturesData::AddAnimation    ( IAnimationDescriptor * animationDesc )
{
    m_animationDescritpors.push_back( animationDesc );
}

} //model
} //bv
