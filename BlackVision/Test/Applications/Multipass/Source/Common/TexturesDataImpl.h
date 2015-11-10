#pragma once

#include "Engine/Models/Interfaces/ITexturesData.h"

namespace bv {

class TexturesDataImpl : public ITexturesData {
private:

    std::vector< ITextureDescriptorPtr >     m_textures;
    std::vector< IAnimationDescriptorPtr >   m_animations;

public:

    ~TexturesDataImpl   ();

    virtual const std::vector< ITextureDescriptorPtr > &     GetTextures         () const override;
    virtual const std::vector< IAnimationDescriptorPtr > &   GetAnimations       () const override;

    bool                                                    AddTextureFromFile  ( const std::string & textureFile, const std::string & textureName );

};

DEFINE_PTR_TYPE(TexturesDataImpl)
DEFINE_CONST_PTR_TYPE(TexturesDataImpl)

} // bv
