#include "stdafx.h"
#include "TexturePluginBase.h"

#include "Engine/Models/Plugins/Channels/PixelShader/GPUTextureDescriptor.h"
#include "Engine/Models/Plugins/Channels/PixelShader/DefaultTextureDescriptor.h"
#include "Engine/Models/Plugins/Channels/PixelShader/ResourceStateModel.h"

#include "Engine/Models/Plugins/Channels/Geometry/HelperVertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/HelperVertexShaderChannel.h"
#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"

#include "Assets/Input/TextureInputAssetDesc.h"
#include "Assets/Input/VideoInput/VideoInputTextureAssetDesc.h"

#include "Assets/DefaultAssets.h"




namespace bv {
namespace model
{


// *************************************
// 
TexturePluginBase::TexturePluginBase            ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin( name, uid, prev, model )
    , m_psc( nullptr )
{
    m_psc = DefaultPixelShaderChannel::Create( model->GetPixelShaderChannelModel() );
}


// *************************************
// 
bool                            TexturePluginBase::LoadResource  ( AssetDescConstPtr assetDescr )
{
    if( IsSupportedDescriptor( assetDescr ) )
    {
        SamplerStateModelPtr newSamplerStateModel = CreateSamplerReplacment();
        
        auto txDesc = LoadTexture( assetDescr, GetTextureName(), newSamplerStateModel );

        // If texture doesn't exists, read fallback texture. 
        if( txDesc == nullptr )
        {
            LoadFallbackTexture( GetTextureName(), newSamplerStateModel );
            return false;
        }
        else
        {
            ReplaceTexture( assetDescr, txDesc );
            return true;
        }
    }

    return false;
}

// ***********************
//
ITextureDescriptorPtr               TexturePluginBase::LoadTexture                  ( const AssetDescConstPtr & assetDesc, const std::string & name, SamplerStateModelPtr samplerState )
{
    return LoadTexture( assetDesc, name, samplerState, DataBuffer::Semantic::S_TEXTURE_STATIC );
}


// ***********************
//
ITextureDescriptorPtr               TexturePluginBase::LoadTexture                  ( const AssetDescConstPtr & assetDesc, const std::string & name, SamplerStateModelPtr samplerState, DataBuffer::Semantic semantic )
{
    ITextureDescriptorPtr resultDesc = nullptr;

    if( assetDesc )
    {
        resultDesc = LoadMemoryTexture( assetDesc, name, samplerState, semantic );
        resultDesc = resultDesc ? resultDesc : LoadGPUTexture( assetDesc, name, samplerState );
    }

    return resultDesc;
}

// ***********************
//
ITextureDescriptorPtr               TexturePluginBase::LoadMemoryTexture            ( const AssetDescConstPtr & assetDesc, const std::string & name, SamplerStateModelPtr samplerState, DataBuffer::Semantic semantic )
{
    // Normal memory texture loaded from disc.
    auto txAssetDesc = QueryTypedDesc< TextureAssetDescConstPtr >( assetDesc );
    if( txAssetDesc )
    {
        auto txDesc = DefaultTextureDescriptor::LoadTexture( txAssetDesc, name );

        if( txDesc )
        {
            txDesc->SetSamplerState( samplerState );
            txDesc->SetSemantic( semantic );
        }

        return txDesc;
    }

    return nullptr;
}

// ***********************
//
ITextureDescriptorPtr               TexturePluginBase::LoadGPUTexture               ( const AssetDescConstPtr & assetDesc, const std::string & name, SamplerStateModelPtr samplerState )
{
    // GPU texture from input slot.
    if( IsTextureSlotAssetDesc( assetDesc->GetUID() ) )
    {
        auto txInputAssetDesc = std::static_pointer_cast< const TextureInputAssetDesc >( assetDesc );
        auto inTexDesc = GPUTextureDescriptor::LoadTexture( txInputAssetDesc, name );

        if( inTexDesc )
        {
            inTexDesc->SetSamplerState( samplerState );
        }

        return inTexDesc;
    }

    return nullptr;
}

// ***********************
//
void                                TexturePluginBase::LoadFallbackTexture          ( const std::string & name, SamplerStateModelPtr samplerState, UInt32 texIdx )
{
    auto txAssetDescr = DefaultAssets::Instance().GetFallbackDesc< TextureAssetDesc >();
    auto txDesc = DefaultTextureDescriptor::LoadTexture( txAssetDescr, name );

    if( txDesc )
    {
        txDesc->SetSamplerState( samplerState );
        txDesc->SetSemantic( DataBuffer::Semantic::S_TEXTURE_STATIC );

        ReplaceTexture( txAssetDescr, txDesc, texIdx );
    }
    else
    {
        LOG_MESSAGE( SeverityLevel::error ) << "Failed to load fallback texture.";
    }
}

// ***********************
//
void                                TexturePluginBase::ReplaceTexture               ( const AssetDescConstPtr & assetDesc, ITextureDescriptorPtr texDesc, UInt32 texIdx )
{
    auto txData = m_psc->GetTexturesDataImpl();

    txData->SetTexture( texIdx, texDesc );
    SetAsset( texIdx, LAsset( texDesc->GetName(), assetDesc, texDesc->GetSamplerState() ) );

    HelperPixelShaderChannel::SetTexturesDataUpdate( m_psc );
}

// ***********************
/// Checks if this descriptor type can be loaded by this plugin.
bool                                TexturePluginBase::IsSupportedDescriptor        ( const AssetDescConstPtr & assetDesc )
{
    auto & uid = assetDesc->GetUID();

    if( uid == TextureAssetDesc::UID() )
        return true;

    if( IsTextureSlotAssetDesc( uid ) )
        return true;

    return false;
}

// ***********************
/// Creates sampler for new loaded asset. Function tries to use previous sampler to avoid changing parameters.
SamplerStateModelPtr                TexturePluginBase::CreateSamplerReplacment      ( UInt32 texIdx ) const
{
    auto txData = m_psc->GetTexturesDataImpl();
    auto replacedTex = txData->GetTexture( texIdx );

    return replacedTex != nullptr ? replacedTex->GetSamplerState() : SamplerStateModel::Create( m_pluginParamValModel->GetTimeEvaluator() );

}

// *************************************
// 
IPixelShaderChannelPtr              TexturePluginBase::GetPixelShaderChannel        () const
{
    return m_psc;
}


} // model
} // bv

