#pragma once

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Assets/Asset.h"
#include "Assets/Font/2D/TextAtlas.h"

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/PixelShader/DefaultTextureDescriptor.h"

namespace bv { namespace model {

class TextPluginBase : public BasePlugin< IPlugin >
{
protected:

    DefaultPixelShaderChannelPtr    m_psc;
    DefaultVertexShaderChannelPtr   m_vsc;

    VertexAttributesChannelPtr      m_vaChannel;

    SizeType                        m_fontSize;
    SizeType                        m_blurSize;
    SizeType                        m_outlineSize;

    TextAtlasConstPtr               m_atlas;

    ParamFloatPtr                   m_spacingParam;
    ParamFloatPtr                   m_alignmentParam;


    bool                                        LoadResource                ( AssetDescConstPtr assetDescr, const std::string & name );
    bool                                        LoadAtlas                   ( const FontAssetDescConstPtr & fontAssetDesc, const std::string & name );

public:

    virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel  () const override;
    virtual IPixelShaderChannelPtr              GetPixelShaderChannel       () const override;
    virtual IVertexShaderChannelConstPtr        GetVertexShaderChannel      () const override;

    virtual void                                SetPrevPlugin               ( IPluginPtr plugin ) override;

    explicit                                    TextPluginBase              (   const std::string & name,
                                                                                const std::string & uid,
                                                                                IPluginPtr prev,
                                                                                DefaultPluginParamValModelPtr model );
    virtual                                     ~TextPluginBase             ();

private:

    DefaultTextureDescriptorPtr                 LoadTexture                 (   DefaultTexturesDataPtr txData,
                                                                                TextureAssetConstPtr res,
                                                                                const std::string & name,
                                                                                TextureWrappingMode hWrappingMode,
                                                                                TextureWrappingMode vWrappingMode,
                                                                                TextureFilteringMode txFilteringMode,
                                                                                const glm::vec4 & bColor,
                                                                                DataBuffer::Semantic semantic );

};

} // model
} // bv