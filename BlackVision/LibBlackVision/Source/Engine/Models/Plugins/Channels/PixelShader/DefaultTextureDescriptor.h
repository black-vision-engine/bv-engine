#pragma once

#include "Engine/Models/Interfaces/ITextureDescriptor.h"
#include "Assets/Texture/TextureAsset.h"
#include "Assets/Texture/TextureAssetDescriptor.h"

#include "Engine/Models/Plugins/Channels/PixelShader/DefaultTextureParams.h"

#include "Engine/Events/Events.h"


namespace bv { namespace model {

class DefaultTextureDescriptor;
DEFINE_PTR_TYPE(DefaultTextureDescriptor)
DEFINE_CONST_PTR_TYPE(DefaultTextureDescriptor)

class DefaultTextureDescriptor : public ITextureDescriptor
{
private:

    TextureAssetConstPtr    m_texResource;

    DefaultTextureParams    m_params;
    DataBuffer::Semantic    m_semantic;

    mutable UInt64          m_updateID;

public:

    DefaultTextureDescriptor        ();
    DefaultTextureDescriptor        ( TextureAssetConstPtr texResource, const std::string & name, DataBuffer::Semantic semantic );
    ~DefaultTextureDescriptor       ();

    virtual uintptr_t                   GetUID          () const override;

    virtual UInt32                      GetNumLevels    () const override;

    virtual MemoryChunkConstPtr         GetBits         (  UInt32 level ) const override;
    virtual MemoryChunkVector           GetBits         () const override;

    virtual UInt64                      GetUpdateID     () const override;

    virtual const std::string           GetName         () const override;

    virtual UInt32                      GetWidth        ( UInt32 level = 0 ) const override;
    virtual UInt32                      GetHeight       ( UInt32 level = 0 ) const override;
    virtual UInt32                      GetDepth        ( UInt32 level = 0 ) const override;
    virtual TextureFormat               GetFormat       () const override;
    virtual SamplerStateModelPtr        GetSamplerState () const override;
    virtual DataBuffer::Semantic        GetSemantic     () const override;

    void                                SetBits         ( TextureAssetConstPtr texResource );

    void                                SetName         ( const std::string & name );

    void                                SetWidth        ( UInt32 w );
    void                                SetHeight       ( UInt32 h );
    void                                SetDepth        ( UInt32 z );
    void                                SetFormat       ( TextureFormat fmt );
    void                                SetSemantic     ( DataBuffer::Semantic semantic );
    void                                SetSamplerState ( SamplerStateModelPtr samplerState );

    static void                         SetDefaults     ( DefaultTextureDescriptorPtr desc );

    static DefaultTextureDescriptorPtr  LoadTexture     ( const TextureAssetDescConstPtr & textureResDesc, const std::string & name );


private:

    static void                         TriggerEvent    ( AssetTrackerInternalEvent::Command command, DefaultTextureDescriptorPtr textureDesc );

};

} //model
} //bv
