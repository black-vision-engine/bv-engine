#include "stdafx.h"

#include "TextPluginBase.h"

#include "Engine/Models/Plugins/Channels/PixelShader/DefaultTextureDescriptor.h"
#include "Engine/Models/Plugins/Channels/PixelShader/DefaultFontDescriptor.h"
#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"

#include "Assets/DefaultAssets.h"
#include "Assets/Font/2D/FontAssetDescriptor.h"
#include "Assets/Font/FontLoader.h"
#include "Assets/Font/Text.h"

#include "Application/ApplicationContext.h"

#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {

// ************************************************************************* DESCRIPTOR *************************************************************************

const std::string TextPluginBase::PARAM::ALPHA              = "alpha";
const std::string TextPluginBase::PARAM::FONT_SIZE          = "fontSize";
const std::string TextPluginBase::PARAM::NEW_LINE_SIZE      = "newLineSize";
const std::string TextPluginBase::PARAM::TEXT_BOX           = "textBox";
const std::string TextPluginBase::PARAM::USE_TEXT_BOX       = "useTextBox";

const std::string TextPluginBase::PARAM::SHADOW_ENABLED     = "shadowEnabled";
const std::string TextPluginBase::PARAM::SHADOW_COLOR       = "shadowColor";
const std::string TextPluginBase::PARAM::SHADOW_TX          = "shadowTx";

const std::string TextPluginBase::PARAM::OUTLINE_ENABLED    = "outlineEnabled";
const std::string TextPluginBase::PARAM::OUTLINE_TX         = "outlineTx";
const std::string TextPluginBase::PARAM::OUTLINE_COLOR      = "outlineColor";

const std::string TextPluginBase::PARAM::GLOW_ENABLED       = "glowEnabled";
const std::string TextPluginBase::PARAM::GLOW_STRENGTH      = "glowStrength";
const std::string TextPluginBase::PARAM::GLOW_COLOR         = "glowColor";

const std::string TextPluginBase::PARAM::SPACING            = "spacing";
const std::string TextPluginBase::PARAM::ALIGNEMENT         = "alignment";
const std::string TextPluginBase::PARAM::ALIGN_CHARACTER    = "alignCharacter";

const std::string TextPluginBase::PARAM::FIRST_TEXT_CC      = "firstTextCC";
const std::string TextPluginBase::PARAM::FIRST_TEXT_OUT_CC  = "firstTextOutCC";
const std::string TextPluginBase::PARAM::FIRST_TEXT_SH_CC   = "firstTextShCC";
const std::string TextPluginBase::PARAM::FIRST_TEXT_GLOW_CC = "firstTextGlowCC";

// *******************************
//
TextPluginBaseDesc::TextPluginBaseDesc                            (const std::string & uid, const std::string & defaultName, const std::string & abbrv)
    : BasePluginDescriptor( uid, defaultName, abbrv )
{}

// *******************************
//
DefaultPluginParamValModelPtr   TextPluginBaseDesc::CreateDefaultModel( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper h( timeEvaluator );

    h.SetOrCreatePSModel();

    h.AddSimpleStatedParam( TextPluginBase::PARAM::ALPHA, 1.f );
    h.AddSimpleStatedParam( TextPluginBase::PARAM::OUTLINE_COLOR, glm::vec4( 0.f, 0.f, 0.f, 0.f ) );
    h.AddSimpleStatedParam( TextPluginBase::PARAM::SHADOW_COLOR, glm::vec4( 0.f, 0.f, 0.f, 0.f ) );
    h.AddSimpleStatedParam( TextPluginBase::PARAM::GLOW_COLOR, glm::vec4( 0.f, 0.f, 0.f, 0.f ) );
    h.AddSimpleStatedParam( TextPluginBase::PARAM::GLOW_ENABLED, false );

    h.AddSimpleStatedParam( TextPluginBase::PARAM::GLOW_STRENGTH, 0.f );

    h.AddValue( TextPluginBase::PARAM::FIRST_TEXT_CC, 0 );
    h.AddValue( TextPluginBase::PARAM::FIRST_TEXT_OUT_CC, 0 );
    h.AddValue( TextPluginBase::PARAM::FIRST_TEXT_SH_CC, 0 );
    h.AddValue( TextPluginBase::PARAM::FIRST_TEXT_GLOW_CC, 0 );

    h.SetOrCreateVSModel();
    h.AddTransformParam( TextPluginBase::PARAM::OUTLINE_TX );
    h.AddTransformParam( TextPluginBase::PARAM::SHADOW_TX );

    h.SetOrCreatePluginModel();

    h.AddSimpleStatedParam( TextPluginBase::PARAM::SHADOW_ENABLED, false );
    h.AddSimpleStatedParam( TextPluginBase::PARAM::OUTLINE_ENABLED, false );
	h.AddSimpleStatedParam( TextPluginBase::PARAM::GLOW_ENABLED, false );

    h.AddSimpleStatedParam( TextPluginBase::PARAM::FONT_SIZE, 0.f );
    h.AddSimpleStatedParam( TextPluginBase::PARAM::TEXT_BOX, glm::vec2( 1.0f, 1.0f ) );
    h.AddSimpleStatedParam( TextPluginBase::PARAM::USE_TEXT_BOX, false );
    h.AddSimpleStatedParam( TextPluginBase::PARAM::NEW_LINE_SIZE, 1.0f );
    h.AddSimpleStatedParam( TextPluginBase::PARAM::SPACING, 0.0f );
    h.AddEnumParam( TextPluginBase::PARAM::ALIGNEMENT, TextAlignmentType::Left, true, true );
    h.AddSimpleStatedParam( TextPluginBase::PARAM::ALIGN_CHARACTER, (int)L'.' );

    return h.GetModel();
}


// *********************************
//
TextPluginBase::TextPluginBase              ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin( name, uid, prev, model )
    , m_psc( nullptr )
    , m_vsc( nullptr )
    , m_vaChannel( nullptr )
    , m_fontSize( 0 )
    , m_blurSize( 0 )
    , m_glowBlurSize( 0 )
    , m_outlineSize( 0 )
    , m_atlas( nullptr )
{
    m_newLineSize       = GetValueParamState< Float32 >( GetPluginParamValModel()->GetPluginModel().get(), PARAM::NEW_LINE_SIZE );
    m_spacing			= GetValueParamState< Float32 >( GetPluginParamValModel()->GetPluginModel().get(), PARAM::SPACING );
    m_alignment			= GetValueParamState< TextAlignmentType >( GetPluginParamValModel()->GetPluginModel().get(), PARAM::ALIGNEMENT );
    m_alignCharacter    = GetValueParamState< Int32 >( GetPluginParamValModel()->GetPluginModel().get(), PARAM::ALIGN_CHARACTER );

	m_shadowEnabled		= GetValueParamState< bool >( GetPluginParamValModel()->GetPluginModel().get(), PARAM::SHADOW_ENABLED );
	m_outlineEnabled	= GetValueParamState< bool >( GetPluginParamValModel()->GetPluginModel().get(), PARAM::OUTLINE_ENABLED );
	m_glowEnabled		= GetValueParamState< bool >( GetPluginParamValModel()->GetPluginModel().get(), PARAM::GLOW_ENABLED );

	m_textBox			= GetValueParamState< glm::vec2 >( GetPluginParamValModel()->GetPluginModel().get(), PARAM::TEXT_BOX );
	m_useTextBox		= GetValueParamState< bool >( GetPluginParamValModel()->GetPluginModel().get(), PARAM::USE_TEXT_BOX );

    m_firstTextCC       = QueryTypedValue< ValueIntPtr >( GetPluginParamValModel()->GetPixelShaderChannelModel()->GetValue( PARAM::FIRST_TEXT_CC ) );
    m_firstTextOutCC    = QueryTypedValue< ValueIntPtr >( GetPluginParamValModel()->GetPixelShaderChannelModel()->GetValue( PARAM::FIRST_TEXT_OUT_CC ) );
    m_firstTextGlowCC   = QueryTypedValue< ValueIntPtr >( GetPluginParamValModel()->GetPixelShaderChannelModel()->GetValue( PARAM::FIRST_TEXT_GLOW_CC ) );
    m_firstTextShCC     = QueryTypedValue< ValueIntPtr >( GetPluginParamValModel()->GetPixelShaderChannelModel()->GetValue( PARAM::FIRST_TEXT_SH_CC ) );

    m_psc = DefaultPixelShaderChannel::Create( model->GetPixelShaderChannelModel() );
    m_vsc = DefaultVertexShaderChannel::Create( model->GetVertexShaderChannelModel() );
    m_vaChannel = TextHelper::CreateEmptyVACForText();

    SetPrevPlugin( prev );
}

// *********************************
//
TextPluginBase::~TextPluginBase             ()
{}

// *********************************
//
bool                                TextPluginBase::LoadResource                ( AssetDescConstPtr assetDescr, const std::string & name )
{
    auto txAssetDescr = QueryTypedDesc< FontAssetDescConstPtr >( assetDescr );

    if ( txAssetDescr != nullptr )
    {
        if( !LoadAtlas( txAssetDescr, name ) )
        {
            return false;
        }

        m_fontSize = txAssetDescr->GetFontSize();
        m_blurSize = txAssetDescr->GetBlurSize();
        m_glowBlurSize = txAssetDescr->GetGlowBlurSize();
        m_outlineSize = txAssetDescr->GetOutlineSize();

        auto fonts = m_psc->GetTexturesDataImpl()->GetFonts();
        assert( fonts.size() == 1 );
        SetAsset( 0, LAsset( name, assetDescr, fonts[ 0 ]->GetStateModel() ) );

        return true;
    }    
    else
    {
        return false;
    }
}

// *********************************
//
IVertexAttributesChannelConstPtr    TextPluginBase::GetVertexAttributesChannel  () const
{
    return m_vaChannel;
}

// *********************************
//
IPixelShaderChannelPtr              TextPluginBase::GetPixelShaderChannel       () const
{
    return m_psc;
}

// *********************************
//
IVertexShaderChannelConstPtr        TextPluginBase::GetVertexShaderChannel      () const
{
    return m_vsc;
}

// *********************************
//
void                                TextPluginBase::SetPrevPlugin               ( IPluginPtr plugin )
{
    BasePlugin::SetPrevPlugin( plugin );

    HelperPixelShaderChannel::CloneRenderContext( m_psc, plugin );

    auto ctx = m_psc->GetRendererContext();
    
	ctx->depthCtx->enabled = false;
    ctx->cullCtx->enabled = false;
    ctx->depthCtx->enabled = true;
    ctx->depthCtx->writable = false;
    ctx->alphaCtx->blendEnabled = true;
    ctx->alphaCtx->srcRGBBlendMode = model::AlphaContext::SrcBlendMode::SBM_ONE;
    ctx->alphaCtx->dstRGBBlendMode = model::AlphaContext::DstBlendMode::DBM_ONE_MINUS_SRC_ALPHA;
}

// *************************************
// 
bool                                TextPluginBase::LoadAtlas                   ( const FontAssetDescConstPtr & fontAssetDesc, const std::string & name )
{
    auto txData = m_psc->GetTexturesDataImpl();
    assert( txData->GetTextures().size() <= 1 );

    auto fontResource = LoadTypedAsset<FontAsset>( fontAssetDesc );
    if( fontResource == nullptr )
    {
        return false;
    }

    m_atlas = TextHelper::GetAtlas( fontResource );

    auto textureResource = m_atlas->GetAsset();

    //FIXME: use some better API to handle resources in general and textures in this specific case
    auto tfm = TextureFilteringMode::TFM_LINEAR;

    if( textureResource->HasMipMaps() )
    {
        tfm = TextureFilteringMode::TFM_LINEAR_MIPMAP_LINEAR;
    }

    auto texDesc = LoadTexture(     txData   
                                ,   textureResource
                                ,   name
                                ,   TextureWrappingMode::TWM_CLAMP_BORDER
                                ,   TextureWrappingMode::TWM_CLAMP_BORDER
                                ,   tfm
                                ,   glm::vec4( 0.f, 0.f, 0.f, 0.f )
                                ,   DataBuffer::Semantic::S_TEXTURE_STATIC );

    auto fontDesc = std::make_shared< DefaultFontDescriptor >( texDesc, texDesc->GetName() );
    txData->SetFont( 0, fontDesc );

    return true;
}

// *************************************
// 
void                                TextPluginBase::Update                      ( TimeType t )
{
    BasePlugin::Update( t );
}

// *************************************
// 
DefaultTextureDescriptorPtr         TextPluginBase::LoadTexture                 (    DefaultTexturesDataPtr txData,
                                                                                     TextureAssetConstPtr res,
                                                                                     const std::string & name,
                                                                                     TextureWrappingMode hWrappingMode,
                                                                                     TextureWrappingMode vWrappingMode,
                                                                                     TextureFilteringMode txFilteringMode,
                                                                                     const glm::vec4 & bColor,
                                                                                     DataBuffer::Semantic semantic )
{
      //FIXME: use some better API to handle resources in general and textures in this specific case
    auto txDesc = std::make_shared< DefaultTextureDescriptor >( res, name, semantic );

    if( txDesc != nullptr )
    {
        auto timeEval = m_pluginParamValModel->GetTimeEvaluator();
        txDesc->SetSamplerState( SamplerStateModel::Create( timeEval, hWrappingMode, vWrappingMode, vWrappingMode, txFilteringMode, bColor ) );
        txDesc->SetBits( res );
        txDesc->SetName( name );

        txData->SetTexture( 0, txDesc );

        HelperPixelShaderChannel::SetTexturesDataUpdate( m_psc );

        return txDesc;
    }
    else
    {
        return nullptr;
    }
}

// *************************************
// 
Float32                             TextPluginBase::BuildVACForText             ( const std::wstring & text, bool useKerning, bool useBox )
{
    auto alignType  = TextAlignmentType( m_alignment.GetParameter().Evaluate() );
    auto alignCh    = (wchar_t)m_alignCharacter.GetValue();
    auto spacing    = m_spacing.GetParameter().Evaluate();

    auto viewWidth  = ApplicationContext::Instance().GetWidth();
    auto viewHeight = ApplicationContext::Instance().GetHeight();

    m_firstTextOutCC->SetValue( 0 );
    m_firstTextCC->SetValue( 0 );
    m_firstTextShCC->SetValue( 0 );
    m_firstTextGlowCC->SetValue( 0 );

    if( m_blurSize > 0 && m_shadowEnabled.valuePtr->GetValue() ) 
    {
        TextHelper::BuildVACForText(    m_vaChannel.get(),
                                        m_atlas,
                                        text,
                                        m_blurSize,
                                        spacing,
                                        alignType,
                                        alignCh,
                                        0,
                                        viewWidth,
                                        viewHeight,
                                        m_newLineSize.GetValue(),
                                        m_textBox.GetValue(),
                                        nullptr,
                                        useKerning,
                                        useBox);

        m_firstTextGlowCC->SetValue( ( Int32 ) m_vaChannel->GetComponents().size() );
    }

    if( m_glowBlurSize > 0 && m_glowEnabled.GetValue() ) 
    {
        TextHelper::BuildVACForText(    m_vaChannel.get(),
                                        m_atlas,
                                        text,
                                        m_glowBlurSize,
                                        spacing,
                                        alignType,
                                        alignCh,
                                        m_outlineSize,
                                        viewWidth,
                                        viewHeight,
                                        m_newLineSize.GetValue(),
                                        m_textBox.GetValue(),
                                        nullptr,
                                        useKerning,
                                        useBox);

        m_firstTextOutCC->SetValue( ( Int32 ) m_vaChannel->GetComponents().size() );
    }

    if( m_outlineSize > 0 && m_outlineEnabled.GetValue() ) 
    {
        TextHelper::BuildVACForText(    m_vaChannel.get(),
                                        m_atlas,
                                        text,
                                        m_glowBlurSize,
                                        spacing,
                                        alignType,
                                        alignCh,
                                        m_outlineSize,
                                        viewWidth,
                                        viewHeight,
                                        m_newLineSize.GetValue(),
                                        m_textBox.GetValue(),
                                        nullptr,
                                        useKerning,
                                        useBox );

        m_firstTextCC->SetValue( ( Int32 ) m_vaChannel->GetComponents().size() );
    }

    auto textLength = TextHelper::BuildVACForText(  m_vaChannel.get(),
                                                    m_atlas,
                                                    text,
                                                    m_glowBlurSize,
                                                    spacing,
                                                    alignType,
                                                    alignCh,
                                                    0,
                                                    viewWidth,
                                                    viewHeight,
                                                    m_newLineSize.GetValue(),
                                                    m_textBox.GetValue(),
                                                    nullptr,
                                                    useKerning,
                                                    useBox );


    return textLength;
}

} // model
} // bv