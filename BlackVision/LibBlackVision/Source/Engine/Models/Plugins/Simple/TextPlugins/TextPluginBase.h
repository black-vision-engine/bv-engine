#pragma once

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/Descriptor/BasePluginDescriptor.h"
#include "Assets/Asset.h"
#include "Assets/Font/2D/TextAtlas.h"

#include "Engine/Models/Plugins/Channels/DefaultPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/DefaultVertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/PixelShader/DefaultTextureDescriptor.h"

#include "Assets/Font/TextHelper.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"

namespace bv { namespace model {

typedef ParamEnum< TextAlignmentType > ParamEnumTAT;
DEFINE_PTR_TYPE( ParamEnumTAT );

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
        static const std::string        USE_TEXT_BOX;

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

	ValueParamState< TextAlignmentType > m_alignment;

	ValueParamState< Float32 >      m_newLineSize;
	ValueParamState< Float32 >		m_spacing;

	ValueParamState< glm::vec2 >    m_textBox;
	ValueParamState< bool >			m_useTextBox;

	ValueParamState< bool >			m_shadowEnabled;
	ValueParamState< bool >         m_outlineEnabled;
	ValueParamState< bool >         m_glowEnabled;

	ValueParamState< Int32 >        m_alignCharacter;

    ParamTransformPtr               m_outlineTxParam;
    ParamTransformPtr               m_glowTxParam;


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

    virtual bool                                SetPrevPlugin               ( IPluginPtr plugin ) override;

    virtual Float32                             BuildVACForText             ( const std::wstring & text, bool useKernin, bool useBoxg );

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