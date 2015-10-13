#pragma once

#include "Engine/Models/Interfaces/ITextureDescriptor.h"
#include "Assets/VideoStream/VideoStreamAsset.h"
#include "Assets/VideoStream/VideoStreamAssetDescriptor.h"

#include "Engine/Models/Plugins/Channels/PixelShader/DefaultTextureParams.h"


namespace bv { namespace model {

class DefaultVideoStreamDescriptor : public ITextureDescriptor
{
private:

    MemoryChunkConstPtr		m_data;

    DefaultTextureParams    m_params;
    mutable bool            m_bitsChanged;
    DataBuffer::Semantic    m_semantic;

public:

	DefaultVideoStreamDescriptor        ( const std::string & name, MemoryChunkConstPtr data, UInt32 width, UInt32 height, TextureFormat format, DataBuffer::Semantic semantic );
    ~DefaultVideoStreamDescriptor       ();

    virtual uintptr_t               GetUID          () const override;

    virtual MemoryChunkConstPtr     GetBits         (  UInt32 level ) const override;
	virtual MemoryChunkVector		GetBits         () const override;

    virtual bool                    BitsChanged     () const override;
    virtual void                    ResetBitsChanged() const override;

    virtual const std::string       GetName         () const override;

    virtual UInt32				    GetWidth        ( UInt32 level = 0 ) const override;
    virtual UInt32				    GetHeight       ( UInt32 level = 0 ) const override;
    
    virtual TextureFormat           GetFormat       () const override;\
    
	virtual DataBuffer::Semantic    GetSemantic     () const override;

	virtual UInt32				    GetNumLevels    () const override;

	virtual UInt32				    GetDepth		( UInt32 level = 0 ) const override;

    virtual TextureWrappingMode     GetWrappingModeX() const override;
    virtual TextureWrappingMode     GetWrappingModeY() const override;
	virtual TextureWrappingMode     GetWrappingModeZ() const override;
    
    virtual TextureFilteringMode    GetFilteringMode() const override;
    
    virtual glm::vec4               BorderColor     () const override;


    void                            SetBits         ( MemoryChunkConstPtr data );

    void                            SetName         ( const std::string & name );

    void                            SetWidth        ( UInt32 w );
    void                            SetHeight       ( UInt32 h );
    
    void                            SetFormat       ( TextureFormat fmt );

    void                            SetSemantic     ( DataBuffer::Semantic semantic );

};

} //model
} //bv
