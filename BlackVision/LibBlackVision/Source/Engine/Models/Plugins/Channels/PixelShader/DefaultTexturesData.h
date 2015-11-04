#pragma once

#include "Engine/Models/Interfaces/ITexturesData.h"

#include "Engine/Models/Plugins/Channels/PixelShader/DefaultTextureDescriptor.h"
#include "Engine/Models/Plugins/Channels/PixelShader/DefaultAnimationDescriptor.h"


namespace bv { namespace model {

class DefaultTexturesData : public ITexturesData
{
private:

    std::vector< ITextureDescriptorPtr >     m_textureDescriptors;
    std::vector< IAnimationDescriptorPtr >   m_animationDescriptors;

public:

            DefaultTexturesData ();
            ~DefaultTexturesData();

    virtual const std::vector< ITextureDescriptorPtr > &     GetTextures     () const override;
    virtual const std::vector< IAnimationDescriptorPtr > &   GetAnimations   () const override;

	//NOTE: not used anywhere
    ITextureDescriptorPtr                                   GetTexture          ( unsigned int idx );
	IAnimationDescriptorPtr                                 GetAnimation        ( unsigned int idx );
    
	ITextureDescriptorPtr                                   GetTexture          ( const std::string & name );

    void                                                    SetTextureParams    ( unsigned int idx, TextureWrappingMode wrapModeX, TextureWrappingMode wrapModeY, TextureFilteringMode filteringMode, const glm::vec4 & borderColor );
	void                                                    SetTexture          ( unsigned int idx, ITextureDescriptorPtr textureDesc );
    void                                                    AddTexture          ( ITextureDescriptorPtr textureDesc );

    void                                                    SetAnimationParams  ( unsigned int idx, TextureWrappingMode wrapModeX, TextureWrappingMode wrapModeY, TextureFilteringMode filteringMode, const glm::vec4 & borderColor );
    void                                                    SetAnimation        ( unsigned int idx, IAnimationDescriptorPtr animationDesc );
    void                                                    AddAnimation        ( IAnimationDescriptorPtr animationDesc );

    bool                                                    SetAnimationFrame   ( unsigned int idx, unsigned int frameNum );

	void													UpdateResourceModels();

	void													ClearAll			();
};

DEFINE_PTR_TYPE(DefaultTexturesData)
DEFINE_CONST_PTR_TYPE(DefaultTexturesData)

} //model
} //bv
