#pragma once

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/PixelShader/ResourceStateModel.h"

#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Engine/Models/Plugins/Plugin.h"

#include "Assets/Input/TextureInputAssetDesc.h"



namespace bv {
namespace model
{





/// ***********************
/// Implementation of texture or input slots loading functionality.
/// Even if you don't won't standard loading function you can use this base class as assets helpers.
class TexturePluginBase : public BasePlugin
{
protected:

    DefaultPixelShaderChannelPtr    m_psc;

public:

    explicit                                    TexturePluginBase           ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model );
    virtual                                     ~TexturePluginBase          () {}



    virtual bool                                LoadResource                ( AssetDescConstPtr assetDescr ) override;
    virtual std::string                         GetTextureName              ( UInt32 idx = 0 ) const = 0;

    virtual IPixelShaderChannelPtr              GetPixelShaderChannel       () const override;

    virtual bool                                IsSupportedDescriptor       ( const AssetDescConstPtr & assetDesc );

protected:

    SamplerStateModelPtr                        CreateSamplerReplacment     ( UInt32 texIdx = 0 ) const;

    void                                        ReplaceTexture              ( const AssetDescConstPtr & assetDesc, ITextureDescriptorPtr texDesc, UInt32 texIdx = 0 );
    void                                        LoadFallbackTexture         ( const std::string & name, SamplerStateModelPtr samplerState, UInt32 texIdx = 0 );

    static ITextureDescriptorPtr                LoadTexture                 ( const AssetDescConstPtr & assetDesc, const std::string & name, SamplerStateModelPtr samplerState, DataBuffer::Semantic semantic );
    static ITextureDescriptorPtr                LoadTexture                 ( const AssetDescConstPtr & assetDesc, const std::string & name, SamplerStateModelPtr samplerState );

    static ITextureDescriptorPtr                LoadMemoryTexture           ( const AssetDescConstPtr & assetDesc, const std::string & name, SamplerStateModelPtr samplerState, DataBuffer::Semantic semantic );
    static ITextureDescriptorPtr                LoadGPUTexture              ( const AssetDescConstPtr & assetDesc, const std::string & name, SamplerStateModelPtr samplerState );

protected:

    static ConnectedComponentPtr                RewriteComponent            ( IConnectedComponentConstPtr prevComponent );
    bool                                        GenerateDefaultUVs          ( VertexAttributesChannelPtr attribsChannel, IVertexAttributesChannelConstPtr prevChannel );

    VertexAttributesChannelPtr                  InitAttributesChannelWithUVs        ( VertexAttributesChannelPtr curChannel );
};

} // model
} // bv
