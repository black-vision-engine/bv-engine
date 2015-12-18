#include "DefaultTextPlugin.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"
#include "Engine/Types/Values/ValuesFactory.h"

#include "Engine/Models/Plugins/Channels/Geometry/HelperVertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/PixelShader/DefaultFontDescriptor.h"

#include "Engine/Models/Plugins/Channels/Geometry/VacAABB.h"

#include "Assets/Font/FontAssetDescriptor.h"
#include "Assets/Font/FontLoader.h"
#include "Assets/Font/Text.h"

#include "Application/ApplicationContext.h"

#include "Engine/Events/Events.h"
#include "Engine/Events/Interfaces/IEventManager.h"

namespace bv { namespace model {

extern TextArranger CircleArranger;

// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultTextPluginDesc::DefaultTextPluginDesc                            ()
    : BasePluginDescriptor( UID(), "text", "txt" )
{
}

// *******************************
//
IPluginPtr              DefaultTextPluginDesc::CreatePlugin             ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultTextPlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   DefaultTextPluginDesc::CreateDefaultModel( ITimeEvaluatorPtr timeEvaluator ) const
{
    //Create all models
    DefaultPluginParamValModelPtr model  = std::make_shared< DefaultPluginParamValModel >( timeEvaluator );
    DefaultParamValModelPtr psModel      = std::make_shared< DefaultParamValModel >();
    DefaultParamValModelPtr vsModel      = std::make_shared< DefaultParamValModel >();
    DefaultParamValModelPtr plModel      = std::make_shared< DefaultParamValModel >();


    //Create all parameters and evaluators
    SimpleWStringEvaluatorPtr   textEvaluator           = ParamValEvaluatorFactory::CreateSimpleWStringEvaluator( "text", timeEvaluator );
    SimpleFloatEvaluatorPtr     alphaEvaluator          = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "alpha", timeEvaluator );
    SimpleTransformEvaluatorPtr trTxEvaluator           = ParamValEvaluatorFactory::CreateSimpleTransformEvaluator( "txMat", timeEvaluator );
    //SimpleFloatEvaluatorPtr     fontSizeEvaluator       = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "fontSize", timeEvaluator );

    //SimpleFloatEvaluatorPtr     blurSizeEvaluator       = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "blurSize", timeEvaluator );
	//SimpleFloatEvaluatorPtr     outlineSizeEvaluator    = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "outlineSize", timeEvaluator );
	SimpleVec4EvaluatorPtr      outlineColorEvaluator   = ParamValEvaluatorFactory::CreateSimpleVec4Evaluator( "outlineColor", timeEvaluator );

    SimpleVec4EvaluatorPtr      rccBeginColorEvaluator  = ParamValEvaluatorFactory::CreateSimpleVec4Evaluator( "rcc_beginColor", timeEvaluator );
    SimpleVec4EvaluatorPtr      rccEndColorEvaluator    = ParamValEvaluatorFactory::CreateSimpleVec4Evaluator( "rcc_endColor", timeEvaluator );
    SimpleIntEvaluatorPtr       colTextEffectIdEvaluator= ParamValEvaluatorFactory::CreateSimpleIntEvaluator( "colTextEffectId", timeEvaluator );
    SimpleIntEvaluatorPtr       transformTextEffectIdEvaluator= ParamValEvaluatorFactory::CreateSimpleIntEvaluator( "transformTextEffectId", timeEvaluator );

    SimpleFloatEvaluatorPtr     timeValEvaluator       = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "time", timeEvaluator );
    SimpleFloatEvaluatorPtr     transformEffectVal1Evaluator       = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "transformEffectVal1", timeEvaluator );
    SimpleFloatEvaluatorPtr     transformEffectVal2Evaluator       = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "transformEffectVal2", timeEvaluator );

    SimpleFloatEvaluatorPtr     spacingEvaluator        = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "spacing", timeEvaluator );
    SimpleFloatEvaluatorPtr     alignmentEvaluator      = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "alignment", timeEvaluator );
    SimpleFloatEvaluatorPtr     maxTextLenghtEvaluator  = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "maxTextLenght", timeEvaluator );

    SimpleVec2EvaluatorPtr      explosionCenterEvaluator = ParamValEvaluatorFactory::CreateSimpleVec2Evaluator( "explosionCenter", timeEvaluator );


    SimpleFloatEvaluatorPtr      animScaleOffsetEvaluator = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "animScaleOffset", timeEvaluator );
    SimpleFloatEvaluatorPtr      animScaleEvaluator     = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "animScale", timeEvaluator );

    SimpleFloatEvaluatorPtr      animAlphaOffsetEvaluator = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "animAlphaOffset", timeEvaluator );
    SimpleFloatEvaluatorPtr      animAlphaEvaluator     = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "animAlpha", timeEvaluator );

    //Register all parameters and evaloators in models
    vsModel->RegisterAll( trTxEvaluator );

    vsModel->RegisterAll( transformEffectVal1Evaluator );
    vsModel->RegisterAll( transformEffectVal2Evaluator );
    vsModel->RegisterAll( animScaleOffsetEvaluator );
    vsModel->RegisterAll( animScaleEvaluator );

    psModel->RegisterAll( outlineColorEvaluator );
    psModel->RegisterAll( alphaEvaluator );
    vsModel->RegisterAll( animAlphaOffsetEvaluator );
    vsModel->RegisterAll( animAlphaEvaluator );

    psModel->RegisterAll( rccBeginColorEvaluator );
    psModel->RegisterAll( rccEndColorEvaluator );
    psModel->RegisterAll( timeValEvaluator );
    psModel->RegisterAll( explosionCenterEvaluator );
    psModel->RegisterAll( colTextEffectIdEvaluator );
    psModel->RegisterAll( transformTextEffectIdEvaluator );

    plModel->RegisterAll( textEvaluator );
    //plModel->RegisterAll( blurSizeEvaluator );
	//plModel->RegisterAll( outlineSizeEvaluator );
    plModel->RegisterAll( spacingEvaluator );
    plModel->RegisterAll( alignmentEvaluator );
    //plModel->RegisterAll( fontSizeEvaluator );
    plModel->RegisterAll( maxTextLenghtEvaluator );

    //Set models structure
    model->SetVertexShaderChannelModel( vsModel );
    model->SetPixelShaderChannelModel( psModel );
    model->SetPluginModel( plModel );

    //Set default values of all parameters
    textEvaluator->Parameter()->SetVal( L"", TimeType( 0.f ) );
    textEvaluator->Parameter()->AccessInterpolator().SetWrapPostMethod( WrapMethod::pingPong );
    alphaEvaluator->Parameter()->SetVal( 1.f, TimeType( 0.0 ) );
    //blurSizeEvaluator->Parameter()->SetVal( 0.f, TimeType( 0.0 ) );
	//outlineSizeEvaluator->Parameter()->SetVal( 0.f, TimeType( 0.0 ) );
    spacingEvaluator->Parameter()->SetVal( 0.f, TimeType( 0.0 ) );
    alignmentEvaluator->Parameter()->SetVal( 0.f, TimeType( 0.0 ) );
	outlineColorEvaluator->Parameter()->SetVal( glm::vec4( 0.f, 0.f, 0.f, 0.f ), TimeType( 0.f ) );

    rccBeginColorEvaluator->Parameter()->SetVal( glm::vec4( 1.f, 1.f, 1.f, 1.f ), TimeType( 0.f ) );
    rccEndColorEvaluator->Parameter()->SetVal( glm::vec4( 0.f, 0.f, 0.f, 1.f ), TimeType( 0.f ) );

    rccBeginColorEvaluator->Parameter()->SetVal( glm::vec4( 0.f, 1.f, 0.f, 1.f ), TimeType( 10.f ) );
    rccEndColorEvaluator->Parameter()->SetVal( glm::vec4( 1.f, 0.f, 0.f, 1.f ), TimeType( 10.f ) );

    colTextEffectIdEvaluator->Parameter()->SetVal( 0, TimeType( 0.f ) );
    transformTextEffectIdEvaluator->Parameter()->SetVal( 0, TimeType( 0.f ) );

    explosionCenterEvaluator->Parameter()->SetVal( glm::vec2( 0.0, -0.2 ), TimeType( 0.f ) );

    trTxEvaluator->Parameter()->Transform().InitializeDefaultSRT();
    //fontSizeEvaluator->Parameter()->SetVal( 8.f, TimeType( 0.f ) );
    maxTextLenghtEvaluator->Parameter()->SetVal( 0.f, TimeType( 0.f ) );

    transformEffectVal1Evaluator->Parameter()->SetVal( 1.f, TimeType( 0.f ) );

    transformEffectVal1Evaluator->Parameter()->SetVal( 0.1f, TimeType( 10.f ) );

    transformEffectVal2Evaluator->Parameter()->SetVal( 2.f, TimeType( 0.f ) );

    transformEffectVal2Evaluator->Parameter()->SetVal( 5.f, TimeType( 10.f ) );

    animScaleOffsetEvaluator->Parameter()->SetVal( 0.f, TimeType( 0.f ) );
    animScaleOffsetEvaluator->Parameter()->SetVal( 1.f, TimeType( 15.f ) );

    animScaleOffsetEvaluator->Parameter()->AccessInterpolator().SetWrapPostMethod( WrapMethod::pingPong );

    animScaleEvaluator->Parameter()->SetVal( 0.f, TimeType( 0.f ) );

    animAlphaOffsetEvaluator->Parameter()->SetVal( 0.f, TimeType( 0.f ) );
    animAlphaOffsetEvaluator->Parameter()->SetVal( 1.f, TimeType( 5.f ) );
    animAlphaOffsetEvaluator->Parameter()->AccessInterpolator().SetWrapPostMethod( WrapMethod::pingPong );
    animAlphaEvaluator->Parameter()->SetVal( 0.f, TimeType( 0.f ) );

    return model;
}

// *******************************
//
std::string             DefaultTextPluginDesc::UID                      ()
{
    return "DEFAULT_TEXT";
}

//
std::wstring            DefaultTextPlugin::GetText                      () const
{
    return m_textParam->Evaluate();
}

// *******************************
//
std::string             DefaultTextPluginDesc::TextureName              ()
{
    return "AtlasTex0";
}

// *************************************
// 
void DefaultTextPlugin::SetPrevPlugin( IPluginPtr prev )
{
    BasePlugin::SetPrevPlugin( prev );

	m_scaleValue =  ValuesFactory::CreateValueMat4( "" );
	m_scaleValue->SetValue( glm::mat4( 1.0 ) );

	HelperPixelShaderChannel::CloneRenderContext( m_psc, prev );
	auto ctx = m_psc->GetRendererContext();
	ctx->cullCtx->enabled = false;
	ctx->alphaCtx->blendEnabled = true;
	ctx->alphaCtx->srcRGBBlendMode = model::AlphaContext::SrcBlendMode::SBM_ONE;
	ctx->alphaCtx->dstRGBBlendMode = model::AlphaContext::DstBlendMode::DBM_ONE_MINUS_SRC_ALPHA;
}

// *************************************
// 
DefaultTextPlugin::DefaultTextPlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin< IPlugin >( name, uid, prev, model )
    , m_psc( nullptr )
    , m_vsc( nullptr )
    , m_vaChannel( nullptr )
    , m_atlas( nullptr )
	, m_textLength( 0.f )
    , m_arranger( nullptr )
{
    //m_arranger = &CircleArranger;
    m_psc = DefaultPixelShaderChannel::Create( model->GetPixelShaderChannelModel() );
    m_vsc = DefaultVertexShaderChannel::Create( model->GetVertexShaderChannelModel() );
	m_vaChannel = TextHelper::CreateEmptyVACForText();

    SetPrevPlugin( prev );

	//FIXME: 'reserve' required texture
	m_psc->GetTexturesDataImpl()->SetTexture( 0, DefaultTextureDescriptor::CreateEmptyTexture2DDesc( DefaultTextPluginDesc::TextureName(), m_pluginParamValModel->GetTimeEvaluator() ) );

    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &DefaultTextPlugin::OnSetText ), KeyPressedEvent::Type() );

    m_spacingParam          = QueryTypedParam< ParamFloatPtr >( GetPluginParamValModel()->GetPluginModel()->GetParameter( "spacing" ) );
    m_alignmentParam        = QueryTypedParam< ParamFloatPtr >( GetPluginParamValModel()->GetPluginModel()->GetParameter( "alignment" ) );
    m_maxTextLengthParam    = QueryTypedParam< ParamFloatPtr >( GetPluginParamValModel()->GetPluginModel()->GetParameter( "maxTextLenght" ) );
    m_textParam             = QueryTypedParam< ParamWStringPtr >( GetPluginParamValModel()->GetPluginModel()->GetParameter( "text" ) );
    
    m_timeParam             = QueryTypedParam< ParamFloatPtr >( GetParameter( "time" ) );
}

// *************************************
// 
DefaultTextPlugin::~DefaultTextPlugin         ()
{
}

// *************************************
// 
void							DefaultTextPlugin::LoadTexture(	DefaultTexturesDataPtr txData,
															   TextureAssetConstPtr res,
																const std::string & name,
																TextureWrappingMode hWrappingMode,
																TextureWrappingMode vWrappingMode,
																TextureFilteringMode txFilteringMode,
																const glm::vec4 & bColor,
																DataBuffer::Semantic semantic )
{
	
      //FIXME: use some better API to handle resources in general and textures in this specific case
	auto txDesc = std::make_shared< DefaultTextureDescriptor >(	res, name, semantic );

	if( txDesc != nullptr )
	{
		auto timeEval = m_pluginParamValModel->GetTimeEvaluator();
		txDesc->SetSamplerState( SamplerStateModel::Create( timeEval, hWrappingMode, vWrappingMode, vWrappingMode, txFilteringMode, bColor ) );
		txDesc->SetBits( res );
		txDesc->SetName( name );

		txData->SetTexture( 0, txDesc );

		HelperPixelShaderChannel::SetTexturesDataUpdate( m_psc );
	}
}

// *************************************
// 
bool							DefaultTextPlugin::LoadAtlas	( const FontAssetDescConstPtr & fontAssetDesc )
{
	auto txData = m_psc->GetTexturesDataImpl();
    assert( txData->GetTextures().size() <= 1 );

	auto fontResource = LoadTypedAsset<FontAsset>( fontAssetDesc );
    if( fontResource == nullptr )
        return false;

	m_atlas = TextHelper::GetAtlas( fontResource );

	auto textureResource = m_atlas->GetAsset();

    //FIXME: use some better API to handle resources in general and textures in this specific case
	auto tfm = TextureFilteringMode::TFM_LINEAR;

	if( textureResource->HasMipMaps() )
	{
		tfm = TextureFilteringMode::TFM_LINEAR_MIPMAP_LINEAR;
	}

	LoadTexture(	txData   
				,	textureResource
                ,   DefaultTextPluginDesc::TextureName()
                ,   TextureWrappingMode::TWM_CLAMP_BORDER
                ,   TextureWrappingMode::TWM_CLAMP_BORDER
				,   tfm
                ,   glm::vec4( 0.f, 0.f, 0.f, 0.f )
                ,   DataBuffer::Semantic::S_TEXTURE_STATIC );

    auto texDesc = txData->GetTexture( DefaultTextPluginDesc::TextureName() );
    auto fontDesc = std::make_shared< DefaultFontDescriptor >( texDesc );
    txData->AddFont( fontDesc );

    return true;
}

// *************************************
// 
bool                            DefaultTextPlugin::LoadResource  ( AssetDescConstPtr assetDescr )
{
	auto txAssetDescr = QueryTypedDesc< FontAssetDescConstPtr >( assetDescr );

    if ( txAssetDescr != nullptr )
    {
		if( !LoadAtlas( txAssetDescr ) )
            return false;

		m_fontSize = txAssetDescr->GetFontSize();
		m_blurSize = txAssetDescr->GetBlurSize();
		m_outlineSize = txAssetDescr->GetOutlineSize();

        SetText( m_textParam->Evaluate() );

        auto fonts = m_psc->GetTexturesDataImpl()->GetFonts();
        assert( fonts.size() == 1 );
        SetAsset( 0, assetDescr, fonts[ 0 ]->GetStateModel() );

		return true;
    }    

    return false;
}

// *************************************
// 
IVertexAttributesChannelConstPtr    DefaultTextPlugin::GetVertexAttributesChannel  () const
{
    return m_vaChannel;
}

// *************************************
// 
IPixelShaderChannelPtr              DefaultTextPlugin::GetPixelShaderChannel       () const
{
    return m_psc;
}

// *************************************
// 
IVertexShaderChannelConstPtr        DefaultTextPlugin::GetVertexShaderChannel      () const
{
    return m_vsc;
}

// *************************************
// 
mathematics::RectConstPtr			DefaultTextPlugin::GetAABB						( const glm::mat4 & trans ) const
{
    auto rect = mathematics::Rect::Create();
    if( AABB( m_vaChannel.get(), trans, rect.get() ) )
	    return rect;
    else
	    return nullptr;

}

// *************************************
// 
void                                DefaultTextPlugin::Update                      ( TimeType t )
{
    m_timeParam->SetVal( t, TimeType( 0.0 ) );
	BasePlugin::Update( t );

    if( m_currentText != m_textParam->Evaluate() )
    {
        SetText( m_textParam->Evaluate() );
    }

	m_scaleMat = glm::mat4( 1.0 );

	ScaleToMaxTextLength();

	m_scaleValue->SetValue( m_scaleMat );

	//assumption that text plugin provides vertices, so no need for backward topology propagation
	HelperVertexAttributesChannel::PropagateAttributesUpdate( m_vaChannel, m_prevPlugin );
	HelperPixelShaderChannel::PropagateUpdate( m_psc, m_prevPlugin );

    m_vsc->PostUpdate();
    m_psc->PostUpdate();
}

namespace {
// *************************************
// FIXME: implement int parameters and bool parameters
template< typename EnumClassType >
inline EnumClassType EvaluateAsInt( ParamFloatPtr param )
{
    int val = int( param->Evaluate() );

    return EnumClassType( val );
}

} //anonymous

// *************************************
//
void DefaultTextPlugin::OnSetText                   ( IEventPtr evt )
{
    if( evt->GetEventType() == KeyPressedEvent::Type())
    {
        KeyPressedEventPtr evtTyped = std::static_pointer_cast<KeyPressedEvent>( evt );
        wchar_t c[2] = {evtTyped->GetChar() , '\0'};

        if( c[0] == L'\b' )
        {
            if( !m_textParam->Evaluate().empty() )
            {
                auto text = m_textParam->Evaluate();
                text.pop_back();
                SetText( text );
            }
        }
        else
		{
			SetText( m_textParam->Evaluate() + std::wstring( c ) );
		}
    }
}

// *************************************
//
void DefaultTextPlugin::ScaleToMaxTextLength		()
{
	auto maxTextLenght = m_maxTextLengthParam->Evaluate();

    if( maxTextLenght > 0.f && m_textLength > 0.f && m_textLength > maxTextLenght )
    {
        m_scaleMat = glm::scale( glm::mat4( 1.f ), glm::vec3( maxTextLenght / m_textLength, 1.f, 1.f ) );

		m_scaleValue->SetValue( m_scaleMat );
    }
}

// *************************************
//
void DefaultTextPlugin::SetText                     ( const std::wstring & newText )
{
    m_currentText = newText;

    m_vaChannel->ClearAll();

    auto alignType		=  EvaluateAsInt< TextAlignmentType >( m_alignmentParam );

	auto viewWidth  = ApplicationContext::Instance().GetWidth();
    auto viewHeight = ApplicationContext::Instance().GetHeight();
    m_textLength = TextHelper::BuildVACForText( m_vaChannel.get(), m_atlas, m_currentText, m_blurSize, m_spacingParam->Evaluate(), alignType, m_outlineSize, viewWidth, viewHeight, m_arranger, false );

	ScaleToMaxTextLength();

	HelperVertexAttributesChannel::SetTopologyUpdate( m_vaChannel );
}


} // model
} // bv
