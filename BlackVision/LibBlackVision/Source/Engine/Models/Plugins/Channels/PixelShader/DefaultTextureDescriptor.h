#pragma once

#include "Engine/Models/Interfaces/ITextureDescriptor.h"
#include "Assets/Texture/TextureAsset.h"
#include "Assets/Texture/TextureAssetDescriptor.h"

#include "Engine/Models/Plugins/Channels/PixelShader/DefaultTextureParams.h"


namespace bv { namespace model {

class DefaultTextureDescriptor : public ITextureDescriptor
{
private:

    TextureAssetConstPtr m_texResource;

    DefaultTextureParams    m_params;
    mutable bool            m_bitsChanged;
    DataBuffer::Semantic    m_semantic;

public:

    DefaultTextureDescriptor        ();
    DefaultTextureDescriptor        ( TextureAssetConstPtr texResource, const std::string & name, TextureWrappingMode wmx, TextureWrappingMode wmy, TextureFilteringMode fm, const glm::vec4 & bc, DataBuffer::Semantic semantic );
    ~DefaultTextureDescriptor       ();

    virtual uintptr_t               GetUID          () const override;

    virtual MemoryChunkConstPtr     GetBits         () const override;

    virtual bool                    BitsChanged     () const override;
    virtual void                    ResetBitsChanged() const override;

    virtual const std::string       GetName         () const override;
    virtual SizeType				GetWidth        () const override;
    virtual SizeType				GetHeight       () const override;
    virtual TextureFormat           GetFormat       () const override;
    virtual TextureWrappingMode     GetWrappingModeX() const override;
    virtual TextureWrappingMode     GetWrappingModeY() const override;
    virtual TextureFilteringMode    GetFilteringMode() const override;
    virtual glm::vec4               BorderColor     () const override;
    virtual DataBuffer::Semantic    GetSemantic     () const override;

    void                            SetBits         ( TextureAssetConstPtr texResource );

    void                            SetBitsChanged  ( bool bitsChanged ) const;

    void                            SetName         ( const std::string & name );
    void                            SetWidth        ( SizeType w );
    void                            SetHeight       ( SizeType h );
    void                            SetFormat       ( TextureFormat fmt );
    void                            SetWrappingModeX( TextureWrappingMode wm );
    void                            SetWrappingModeY( TextureWrappingMode wm );
    void                            SetFilteringMode( TextureFilteringMode fm );
    void                            SetBorderColor  ( const glm::vec4 & bc );
    void                            SetSemantic     ( DataBuffer::Semantic semantic );

    static void                     SetDefaults     ( DefaultTextureDescriptor * desc );

	static DefaultTextureDescriptor * LoadTexture   ( const TextureAssetDescConstPtr & textureResDesc, const std::string & name );

};

} //model
} //bv
