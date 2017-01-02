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
        static const std::string        NEW_LINE_SIZE;
        static const std::string        TEXT_BOX;

        static const std::string        SHADOW_ENABLED;
        static const std::string        SHADOW_COLOR;
        static const std::string        SHADOW_TX;

        static const std::string        OUTLINE_ENABLED;
        static const std::string        OUTLINE_COLOR;
        static const std::string        OUTLINE_TX;

        static const std::string        GLOW_ENABLED;
        static const std::string        GLOW_STRENGTH;
        static const std::string        GLOW_COLOR;

        static const std::string        SPACING;
        static const std::string        ALIGNEMENT;
        static const std::string        ALIGN_CHARACTER;
        static const std::string        FIRST_TEXT_CC;
        static const std::string        FIRST_TEXT_OUT_CC;
        static const std::string        FIRST_TEXT_SH_CC;
        static const std::string        FIRST_TEXT_GLOW_CC;
    };

protected:

    DefaultPixelShaderChannelPtr    m_psc;
    DefaultVertexShaderChannelPtr   m_vsc;

    VertexAttributesChannelPtr      m_vaChannel;

    SizeType                        m_fontSize;
    SizeType                        m_blurSize;
    SizeType                        m_glowBlurSize;
    SizeType                        m_outlineSize;

    TextAtlasConstPtr               m_atlas;

    ValueFloatPtr                   m_newLineSize;
    ParamFloatPtr                   m_spacingParam;
    ParamIntPtr                     m_alignmentParam;
    ValueVec2Ptr                    m_box;

    ParamTransformPtr               m_outlineTxParam;
    ParamTransformPtr               m_glowTxParam;

    ValueBoolPtr                    m_shadowEnabled;
    ValueBoolPtr                    m_outlineEnabled;
    ValueBoolPtr                    m_glowEnabled;

    ValueIntPtr                     m_alignCharacter;
    ValueIntPtr                     m_firstTextCC;
    ValueIntPtr                     m_firstTextOutCC;
    ValueIntPtr                     m_firstTextGlowCC;
    ValueIntPtr                     m_firstTextShCC;

    bool                                        LoadResource                ( AssetDescConstPtr assetDescr, const std::string & name );
    bool                                        LoadAtlas                   ( const FontAssetDescConstPtr & fontAssetDesc, const std::string & name );

public:

    virtual void                                Update                      ( TimeType t ) override;

    virtual IVertexAttributesChannelConstPtr    GetVertexAttributesChannel  () const override;
    virtual IPixelShaderChannelPtr              GetPixelShaderChannel       () const override;
    virtual IVertexShaderChannelConstPtr        GetVertexShaderChannel      () const override;

    virtual void                                SetPrevPlugin               ( IPluginPtr plugin ) override;

    virtual Float32                             BuildVACForText             ( const std::wstring & text, bool useKerning );

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