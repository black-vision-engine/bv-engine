#pragma once

#include "Engine/Models/Interfaces/ITexturesData.h"


namespace bv { namespace model {

class DefaultTexturesData : public ITexturesData
{
private:

    std::vector< ITextureDescriptor * >     m_textureDescritpors;
    std::vector< IAnimationDescriptor * >   m_animationDescritpors;

public:

            DefaultTexturesData ();
            ~DefaultTexturesData();

    virtual const std::vector< ITextureDescriptor * > &     GetTextures     () const override;
    virtual const std::vector< IAnimationDescriptor * > &   GetAnimations   () const override;

    void                                                    AddTexture      ( ITextureDescriptor * textureDesc );
    void                                                    AddAnimation    ( IAnimationDescriptor * animationDesc );

};

} //model
} //bv
