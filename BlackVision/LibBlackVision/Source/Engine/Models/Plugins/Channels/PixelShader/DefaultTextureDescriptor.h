#pragma once

#include "Engine/Models/Interfaces/ITextureDescriptor.h"
#include "Assets/Texture/TextureAsset.h"
#include "Assets/Texture/TextureAssetDescriptor.h"

#include "Engine/Models/Plugins/Channels/PixelShader/DefaultTextureParams.h"


namespace bv { namespace model {

class DefaultTextureDescriptor : public ITextureDescriptor
{
private:

    TextureAssetConstPtr	m_texResource;

    DefaultTextureParams    m_params;
    DataBuffer::Semantic    m_semantic;

	mutable UInt32			m_updateID;

public:

    DefaultTextureDescriptor        ();
    DefaultTextureDescriptor        ( TextureAssetConstPtr texResource, const std::string & name, TextureWrappingMode wmx, TextureWrappingMode wmy, TextureFilteringMode fm, const glm::vec4 & bc, DataBuffer::Semantic semantic );
    ~DefaultTextureDescriptor       ();

    virtual uintptr_t               GetUID          () const override;

	virtual UInt32				    GetNumLevels    () const override;

    virtual MemoryChunkConstPtr     GetBits         (  UInt32 level ) const override;
	virtual MemoryChunkVector		GetBits         () const override;

	virtual UInt32                  GetUpdateID     () const override;

    virtual const std::string       GetName         () const override;

    virtual UInt32				    GetWidth        ( UInt32 level = 0 ) const override;
    virtual UInt32				    GetHeight       ( UInt32 level = 0 ) const override;
	virtual UInt32				    GetDepth		( UInt32 level = 0 ) const override;
    
    virtual TextureFormat           GetFormat       () const override;\

    virtual TextureWrappingMode     GetWrappingModeX() const override;
    virtual TextureWrappingMode     GetWrappingModeY() const override;
	virtual TextureWrappingMode     GetWrappingModeZ() const override;
    
    virtual TextureFilteringMode    GetFilteringMode() const override;
    
    virtual glm::vec4               BorderColor     () const override;
    
    virtual DataBuffer::Semantic    GetSemantic     () const override;

    void                            SetBits         ( TextureAssetConstPtr texResource );

    void                            SetName         ( const std::string & name );

    void                            SetWidth        ( UInt32 w );
    void                            SetHeight       ( UInt32 h );
	void                            SetDepth        ( UInt32 z );
    
    void                            SetFormat       ( TextureFormat fmt );
    
    void                            SetWrappingModeX( TextureWrappingMode wm );
    void                            SetWrappingModeY( TextureWrappingMode wm );
	void                            SetWrappingModeZ( TextureWrappingMode wm );
    
    void                            SetFilteringMode( TextureFilteringMode fm );
    
    void                            SetBorderColor  ( const glm::vec4 & bc );
    
    void                            SetSemantic     ( DataBuffer::Semantic semantic );

    static void                     SetDefaults     ( DefaultTextureDescriptor * desc );

	static DefaultTextureDescriptor * LoadTexture   ( const TextureAssetDescConstPtr & textureResDesc, const std::string & name );

};

} //model
} //bv
