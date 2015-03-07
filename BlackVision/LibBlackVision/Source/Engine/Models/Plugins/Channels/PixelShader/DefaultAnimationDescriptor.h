#pragma once

#include <vector>

#include "Engine/Models/Interfaces/IAnimationDescriptor.h"
#include "Engine/Models/Resources/IResource.h"
#include "Engine/Models/Resources/Texture/AnimationResourceDescriptor.h"
#include "Engine/Models/Resources/Texture/TextureResource.h"

#include "Engine/Models/Plugins/Channels/PixelShader/DefaultTextureParams.h"


namespace bv { namespace model {

class DefaultAnimationDescriptor : public IAnimationDescriptor
{
private:

	std::vector< TextureResourceConstPtr >	m_frames;
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
    virtual SizeType	            GetWidth            () const override;
    virtual SizeType	            GetHeight           () const override;
    virtual TextureFormat           GetFormat           () const override;
    virtual TextureWrappingMode     GetWrappingModeX    () const override;
    virtual TextureWrappingMode     GetWrappingModeY    () const override;
    virtual TextureFilteringMode    GetFilteringMode    () const override;
    virtual glm::vec4               BorderColor         () const override;

    void                            SetBits             ( UInt32 idx, TextureResourceConstPtr handle );
    void                            AddBits             ( TextureResourceConstPtr handle );

    void                            SetBitsChanged      ( UInt32 frameNum, bool bitsChanged ) const;
    void                            SetCurrentFrame     ( UInt32 frameNum );

    void                            SetName             ( const std::string & name );
    void                            SetWidth            ( UInt32 w );
    void                            SetHeight           ( UInt32 h );
    void                            SetFormat           ( TextureFormat fmt );
    void                            SetWrappingModeX    ( TextureWrappingMode wm );
    void                            SetWrappingModeY    ( TextureWrappingMode wm );
    void                            SetFilteringMode    ( TextureFilteringMode fm );
    void                            SetBorderColor      ( const glm::vec4 & bc );

    static DefaultAnimationDescriptor * LoadAnimation   ( const AnimationResourceDescConstPtr & frames, const std::string & name );

};

} //model
} //bv
