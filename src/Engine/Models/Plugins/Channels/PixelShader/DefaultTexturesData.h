#pragma once

#include "Engine/Models/Interfaces/ITexturesData.h"

#include "Engine/Models/Plugins/Channels/PixelShader/DefaultTextureDescriptor.h"
#include "Engine/Models/Plugins/Channels/PixelShader/DefaultAnimationDescriptor.h"


namespace bv { namespace model {

class DefaultTexturesData : public ITexturesData
{
private:

    std::vector< ITextureDescriptor * >     m_textureDescriptors;
    std::vector< IAnimationDescriptor * >   m_animationDescriptors;

public:

            DefaultTexturesData ();
            ~DefaultTexturesData();

    virtual const std::vector< ITextureDescriptor * > &     GetTextures     () const override;
    virtual const std::vector< IAnimationDescriptor * > &   GetAnimations   () const override;

    ITextureDescriptor *                                    GetTexture          ( unsigned int idx );
    IAnimationDescriptor *                                  GetAnimation        ( unsigned int idx );

    void                                                    SetTextureParams    ( unsigned int idx, TextureWrappingMode wrapModeX, TextureWrappingMode wrapModeY, TextureFilteringMode filteringMode, const glm::vec4 & borderColor );
    void                                                    SetTexture          ( unsigned int idx, DefaultTextureDescriptor * textureDesc );
    void                                                    AddTexture          ( DefaultTextureDescriptor * textureDesc );

    void                                                    SetAnimationParams  ( unsigned int idx, TextureWrappingMode wrapModeX, TextureWrappingMode wrapModeY, TextureFilteringMode filteringMode, const glm::vec4 & borderColor );
    void                                                    SetAnimation        ( unsigned int idx, DefaultAnimationDescriptor * animationDesc );
    void                                                    AddAnimation        ( DefaultAnimationDescriptor * animationDesc );

    void                                                    SetAnimationFrame   ( unsigned int idx, unsigned int frameNum );

};

} //model
} //bv
