#pragma once

#include <vector>

#include "Engine/Models/Interfaces/IAnimationDescriptor.h"
#include "Assets/Asset.h"
#include "Assets/Texture/AnimationAssetDescriptor.h"
#include "Assets/Texture/TextureAsset.h"

#include "Engine/Models/Plugins/Channels/PixelShader/DefaultTextureParams.h"


namespace bv { namespace model {

class DefaultAnimationDescriptor;
DEFINE_PTR_TYPE(DefaultAnimationDescriptor)
DEFINE_CONST_PTR_TYPE(DefaultAnimationDescriptor)

class DefaultAnimationDescriptor : public IAnimationDescriptor
{
private:

	std::vector< TextureAssetConstPtr >	m_frames;
    mutable std::vector< bool >				m_frameBiteChanged;

    DefaultTextureParams        m_params;

    UInt32						m_curFrame;
    UInt32						m_lastFrame;

public:

    DefaultAnimationDescriptor      ();
    DefaultAnimationDescriptor      ( const std::string & name, UInt32 w, UInt32 h, TextureFormat fmt, TextureWrappingMode wmx, TextureWrappingMode wmy, TextureFilteringMode fm, const glm::vec4 & bc );
    ~DefaultAnimationDescriptor     ();

    virtual unsigned int            NumTextures         () const override;
    virtual MemoryChunkConstPtr     GetBits             ( unsigned int idx ) const override;

    virtual bool                    BitsChanged         () const override;
    virtual bool                    BitsChanged         ( UInt32 frameNum ) const override;
    virtual void                    ResetBitsChanged    ( UInt32 frameNum ) const override;

    virtual UInt32					CurrentFrame        () const override;
    virtual UInt32					PreviousFrame       () const override;

    virtual const std::string       GetName             () const override;

    virtual UInt32	                GetWidth            ( UInt32 level = 0 ) const override;
    virtual UInt32	                GetHeight           ( UInt32 level = 0 ) const override;
	virtual UInt32	                GetDepth            ( UInt32 level = 0 ) const override;
    
    virtual TextureFormat           GetFormat           () const override;
	
	virtual SamplerStateModelPtr	GetSamplerState		() const override;

 //   virtual TextureWrappingMode     GetWrappingModeX    () const override;
 //   virtual TextureWrappingMode     GetWrappingModeY    () const override;
	//virtual TextureWrappingMode     GetWrappingModeZ	() const override;

 //   virtual TextureFilteringMode    GetFilteringMode    () const override;
 //   
 //   virtual glm::vec4               BorderColor         () const override;

    void                            SetBits             ( UInt32 idx, TextureAssetConstPtr handle );
    void                            AddBits             ( TextureAssetConstPtr handle );

    void                            SetBitsChanged      ( UInt32 frameNum, bool bitsChanged ) const;
    void                            SetCurrentFrame     ( UInt32 frameNum );

    void                            SetName             ( const std::string & name );

    void                            SetWidth            ( UInt32 w );
    void                            SetHeight           ( UInt32 h );

    void                            SetFormat           ( TextureFormat fmt );

	void							SetSamplerState		( SamplerStateModelPtr samplerState );

    //void                            SetWrappingModeX    ( TextureWrappingMode wm );
    //void                            SetWrappingModeY    ( TextureWrappingMode wm );

    //void                            SetFilteringMode    ( TextureFilteringMode fm );

    //void                            SetBorderColor      ( const glm::vec4 & bc );

    static DefaultAnimationDescriptorPtr LoadAnimation   ( const AnimationAssetDescConstPtr & frames, const std::string & name );

};

} //model
} //bv
