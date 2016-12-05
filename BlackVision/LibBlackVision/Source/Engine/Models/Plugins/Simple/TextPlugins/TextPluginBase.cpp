#include "stdafx.h"

#include "TextPluginBase.h"

#include "Engine/Models/Plugins/Channels/PixelShader/DefaultTextureDescriptor.h"
#include "Engine/Models/Plugins/Channels/PixelShader/DefaultFontDescriptor.h"
#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"

#include "Assets/DefaultAssets.h"
#include "Assets/Font/TextHelper.h"
#include "Assets/Font/2D/FontAssetDescriptor.h"
#include "Assets/Font/FontLoader.h"
#include "Assets/Font/Text.h"



#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {

// ************************************************************************* DESCRIPTOR *************************************************************************

const std::string TextPluginBase::PARAM::ALPHA           = "alpha";
const std::string TextPluginBase::PARAM::FONT_SIZE       = "fontSize";
const std::string TextPluginBase::PARAM::BLUR_SIZE       = "blurSize";
const std::string TextPluginBase::PARAM::GLOW_COLOR      = "glowColor";
const std::string TextPluginBase::PARAM::OUTLINE_SIZE    = "outlineSize";
const std::string TextPluginBase::PARAM::OUTLINE_COLOR   = "outlineColor";
const std::string TextPluginBase::PARAM::SPACING         = "spacing";
const std::string TextPluginBase::PARAM::ALIGNEMENT      = "alingment";
const std::string TextPluginBase::PARAM::ALIGN_CHARACTER = "alignCharacter";

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

    h.SetOrCreatePluginModel();

    h.AddSimpleStatedParam( TextPluginBase::PARAM::FONT_SIZE, 0.f );
    h.AddSimpleStatedParam( TextPluginBase::PARAM::BLUR_SIZE, 0.f );
    h.AddSimpleStatedParam( TextPluginBase::PARAM::OUTLINE_SIZE, 0.f );
    h.AddSimpleStatedParam( TextPluginBase::PARAM::SPACING, 0.0f );
    h.AddSimpleStatedParam( TextPluginBase::PARAM::ALIGNEMENT, 0.0f );
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
    , m_outlineSize( 0 )
    , m_atlas( nullptr )
{
    m_spacingParam      = QueryTypedParam< ParamFloatPtr >( GetPluginParamValModel()->GetPluginModel()->GetParameter( PARAM::SPACING ) );
    m_alignmentParam    = QueryTypedParam< ParamFloatPtr >( GetPluginParamValModel()->GetPluginModel()->GetParameter( PARAM::ALIGNEMENT ) );
    m_alignCharacter    = QueryTypedValue< ValueIntPtr >( GetPluginParamValModel()->GetPluginModel()->GetValue( PARAM::ALIGN_CHARACTER ) );

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
    
    ctx->cullCtx->enabled = false;
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

} // model
} // bv