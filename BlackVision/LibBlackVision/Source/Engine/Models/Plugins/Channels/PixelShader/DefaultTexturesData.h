#pragma once

#include "Engine/Models/Interfaces/ITexturesData.h"
#include "Engine/Events/Events.h"


namespace bv { namespace model {

class DefaultTexturesData : public ITexturesData
{
private:

    std::vector< ITextureDescriptorPtr >                    m_textureDescriptors;
    std::vector< IAnimationDescriptorPtr >                  m_animationDescriptors;
    std::vector< IFontDescriptorPtr >                       m_fontDescriptors;

public:

            DefaultTexturesData ();
            ~DefaultTexturesData();

    virtual const std::vector< ITextureDescriptorPtr > &    GetTextures     () const override;
    virtual const std::vector< IAnimationDescriptorPtr > &  GetAnimations   () const override;
    virtual const std::vector< IFontDescriptorPtr > &       GetFonts        () const override;

	//NOTE: not used anywhere
    ITextureDescriptorPtr                                   GetTexture          ( unsigned int idx );
	IAnimationDescriptorPtr                                 GetAnimation        ( unsigned int idx );
    
	ITextureDescriptorPtr                                   GetTexture          ( const std::string & name );

	void                                                    SetTexture          ( unsigned int idx, ITextureDescriptorPtr textureDesc );
    void                                                    AddTexture          ( ITextureDescriptorPtr textureDesc );

    void                                                    SetAnimation        ( unsigned int idx, IAnimationDescriptorPtr animationDesc );
    void                                                    AddAnimation        ( IAnimationDescriptorPtr animationDesc );

    bool                                                    SetAnimationFrame   ( unsigned int idx, unsigned int frameNum );

    void                                                    SetFont             ( unsigned int idx, IFontDescriptorPtr fontDesc );
    void                                                    AddFont             ( IFontDescriptorPtr fontDesc );

	void													UpdateResourceModels();

	void													ClearAll			();

private:

    void                                                    TriggerEvent        ( AssetTrackerInternalEvent::Command command, ITextureDescriptorPtr textureDesc );

};

DEFINE_PTR_TYPE(DefaultTexturesData)
DEFINE_CONST_PTR_TYPE(DefaultTexturesData)

} //model
} //bv
