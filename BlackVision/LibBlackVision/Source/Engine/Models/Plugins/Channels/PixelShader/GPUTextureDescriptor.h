#pragma once

#include "Engine/Models/Interfaces/ITextureDescriptor.h"
#include "Assets/Input/TextureInputAssetDesc.h"
#include "Assets/Input/ITextureInputAsset.h"

#include "Engine/Models/Plugins/Channels/PixelShader/DefaultTextureParams.h"

#include "Engine/Events/Events.h"


namespace bv {
namespace model
{



class GPUTextureDescriptor;
DEFINE_PTR_TYPE( GPUTextureDescriptor )
DEFINE_CONST_PTR_TYPE( GPUTextureDescriptor )


// ***********************
//
class GPUTextureDescriptor : public ITextureDescriptor
{
private:

    ITextureInputAssetConstPtr      m_texResource;
    DefaultTextureParams            m_params;

public:

    GPUTextureDescriptor        ();
    GPUTextureDescriptor        ( ITextureInputAssetConstPtr texResource, const std::string & name );
    ~GPUTextureDescriptor       ();

    virtual uintptr_t                   GetUID          () const override;

    virtual UInt32                      GetNumLevels    () const override;

    virtual MemoryChunkConstPtr         GetBits         ( UInt32 level ) const override;
    virtual MemoryChunkVector           GetBits         () const override;

    virtual UInt64                      GetUpdateID     () const override;

    virtual const std::string           GetName         () const override;

    virtual UInt32                      GetWidth        ( UInt32 level = 0 ) const override;
    virtual UInt32                      GetHeight       ( UInt32 level = 0 ) const override;
    virtual UInt32                      GetDepth        ( UInt32 level = 0 ) const override;
    virtual TextureFormat               GetFormat       () const override;
    virtual SamplerStateModelPtr        GetSamplerState () const override;
    virtual DataBuffer::Semantic        GetSemantic     () const override;

    void                                SetName         ( const std::string & name );

    void                                SetSamplerState ( SamplerStateModelPtr samplerState );

};

} //model
} //bv
