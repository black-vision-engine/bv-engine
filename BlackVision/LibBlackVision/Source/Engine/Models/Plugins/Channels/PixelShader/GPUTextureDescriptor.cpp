#include "stdafx.h"

#include "GPUTextureDescriptor.h"

#include "Assets/AssetManager.h"
#include "Assets/Input/ITextureInputAsset.h"

#include "Application/ApplicationContext.h"


namespace bv {
namespace model
{



// **************************
//
GPUTextureDescriptor::GPUTextureDescriptor        ( ITextureInputAssetConstPtr texResource, const std::string & name )
    : m_texResource( texResource )
    , m_updateID( ApplicationContext::Instance().GetUpdateCounter() + 1 )
{
    auto format = texResource->GetFormat();
    auto width = texResource->GetWidth();
    auto height = texResource->GetHeight();

    // We set these values but never use them in future. Moreover when texture changes
    // these values won't be updated, so don't use these values and don't expose it to ouside world.
    m_params.SetWidth( width );
    m_params.SetHeight( height );
    m_params.SetDepth( texResource->GetDepth() );
    m_params.SetFormat( format );

    SetName( name );
}

// **************************
//
GPUTextureDescriptor::~GPUTextureDescriptor                     ()
{}

// **************************
//
uintptr_t               GPUTextureDescriptor::GetUID            () const
{
    return ( uintptr_t )m_texResource.get();
}

// **************************
//
UInt32                  GPUTextureDescriptor::GetNumLevels      () const
{
    if( m_texResource )
    {
        m_texResource->GetNumMipMapLevels();
    }
    return 0;
}

// **************************
//
MemoryChunkConstPtr     GPUTextureDescriptor::GetBits           ( UInt32 ) const
{
    return nullptr;
}

// **************************
//
MemoryChunkVector       GPUTextureDescriptor::GetBits           () const
{
    return MemoryChunkVector();
}

// **************************
//
UInt64                  GPUTextureDescriptor::GetUpdateID       () const
{
    // Update ID in m_texResource changes if someone changes content in input slot.
    // m_updateID is this descriptor creation time which is independent from asset creation
    // and multiple plugins can have different value here. We choose last update that has been made.
    return std::max( m_texResource->GetUpdateID(), m_updateID );
}

// **************************
//
const std::string       GPUTextureDescriptor::GetName           () const
{
    return m_params.GetName();
}

// **************************
//
UInt32                  GPUTextureDescriptor::GetWidth          ( UInt32 level ) const
{
    return m_texResource->GetWidth( level );
}

// **************************
//
UInt32                  GPUTextureDescriptor::GetHeight         ( UInt32 level ) const
{
    return m_texResource->GetHeight( level );
}

// **************************
//
UInt32                  GPUTextureDescriptor::GetDepth          ( UInt32 level ) const
{
    return m_texResource->GetDepth( level );
}

// **************************
//
TextureFormat           GPUTextureDescriptor::GetFormat         () const
{
    return m_texResource->GetFormat();
}

// **************************
//
DataBuffer::Semantic    GPUTextureDescriptor::GetSemantic     () const
{
    return m_texResource->GetSemantic();
}

// **************************
//
SamplerStateModelPtr    GPUTextureDescriptor::GetSamplerState     () const
{
    return m_params.GetSamplerState();
}

// **************************
//
void                    GPUTextureDescriptor::SetName           ( const std::string & name )
{
    m_params.SetName( name );
}

// **************************
//
void                    GPUTextureDescriptor::SetSamplerState   ( SamplerStateModelPtr samplerState )
{
    m_params.SetSamplerState( samplerState );
}

// ***********************
//
GPUTextureDescriptorPtr GPUTextureDescriptor::LoadTexture       ( const TextureInputAssetDescConstPtr & inputTexDesc, const std::string & name )
{
    if( !inputTexDesc )
        return nullptr;

    // We don't use LoadTypedAsset functionality here. Descriptor from parameters can be
    // one of subclasses either. We use only interface to original asset to provide separation between engine and model.
    auto asset = AssetManager::GetInstance().LoadAsset( inputTexDesc );
    auto inputAsset = std::static_pointer_cast< const ITextureInputAsset >( asset );

    if( inputAsset == nullptr )
    {
        return nullptr;
    }

    auto texDesc = std::make_shared< GPUTextureDescriptor >( inputAsset, name );
    texDesc->SetName( name );

    return texDesc;
}


} //model
} //bv