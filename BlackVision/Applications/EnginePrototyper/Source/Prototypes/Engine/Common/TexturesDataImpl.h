#pragma once

#include "Engine/Models/Interfaces/ITexturesData.h"

namespace bv {

class TexturesDataImpl : public ITexturesData {
private:

    std::vector< ITextureDescriptor * >     m_textures;
    std::vector< IAnimationDescriptor * >   m_animations;

public:

    virtual const std::vector< ITextureDescriptor * > &     GetTextures         () const override;
    virtual const std::vector< IAnimationDescriptor * > &   GetAnimations       () const override;

    bool                                                    AddTextureFromFile  ( const std::string & textureFile );

};

DEFINE_PTR_TYPE(TexturesDataImpl)
DEFINE_CONST_PTR_TYPE(TexturesDataImpl)

} // bv
