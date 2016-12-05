#pragma once

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Assets/Asset.h"
#include "Assets/Font/2D/TextAtlas.h"

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/PixelShader/DefaultTextureDescriptor.h"

namespace bv { namespace model {

// ***************************** DESCRIPTOR **********************************
class TextPluginBaseDesc : public BasePluginDescriptor
{
public:

    TextPluginBaseDesc                                          ( const std::string & uid, const std::string & defaultName, const std::string & abbrv );

    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
};


class TextPluginBase : public BasePlugin
{
public:
    struct PARAM
    {
        static const std::string        ALPHA;
        static const std::string        FONT_SIZE;
        static const std::string        BLUR_SIZE;
        static const std::string        GLOW_COLOR;
        static const std::string        OUTLINE_SIZE;
        static const std::string        OUTLINE_COLOR;
        static const std::string        SPACING;
        static const std::string        ALIGNEMENT;
        static const std::string        ALIGN_CHARACTER;
    };

protected:

    DefaultPixelShaderChannelPtr    m_psc;
    DefaultVertexShaderChannelPtr   m_vsc;

    VertexAttributesChannelPtr      m_vaChannel;

    SizeType                        m_fontSize;
    SizeType                        m_blurSize;
    SizeType                        m_outlineSize;

    TextAtlasConstPtr               m_atlas;

    ParamFloatPtr                   m_spacingParam;
    ParamIntPtr                     m_alignmentParam;
    ValueIntPtr                     m_alignCharacter;

    bool                                        LoadResource                ( AssetDescConstPtr assetDescr, const std::string & name );
    bool                                        LoadAtlas                   ( const FontAssetDescConstPtr & fontAssetDesc, const std::string & name );

public:

    virtual void                                Update                      ( TimeType t ) override;

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