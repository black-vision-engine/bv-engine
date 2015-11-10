#include "DefaultTextPlugin.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"

#include "Engine/Models/Plugins/Channels/Geometry/HelperVertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"

#include "Engine/Models/Plugins/Channels/Geometry/VacAABB.h"

#include "Assets/Font/FontAssetDescriptor.h"
#include "Assets/Font/FontLoader.h"
#include "Assets/Font/Text.h"

#include "Engine/Events/Events.h"
#include "Engine/Events/Interfaces/IEventManager.h"

namespace bv { namespace model {


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
    SimpleFloatEvaluatorPtr     alphaEvaluator          = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "alpha", timeEvaluator );
    SimpleTransformEvaluatorPtr trTxEvaluator           = ParamValEvaluatorFactory::CreateSimpleTransformEvaluator( "txMat", timeEvaluator );
    SimpleFloatEvaluatorPtr     fontSizeEvaluator       = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "fontSize", timeEvaluator );

    SimpleFloatEvaluatorPtr     blurSizeEvaluator       = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "blurSize", timeEvaluator );
	SimpleFloatEvaluatorPtr     outlineSizeEvaluator    = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "outlineSize", timeEvaluator );
	SimpleVec4EvaluatorPtr      outlineColorEvaluator   = ParamValEvaluatorFactory::CreateSimpleVec4Evaluator( "outlineColor", timeEvaluator );

    SimpleFloatEvaluatorPtr     spacingEvaluator        = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "spacing", timeEvaluator );
    SimpleFloatEvaluatorPtr     alignmentEvaluator      = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "alignment", timeEvaluator );
    SimpleFloatEvaluatorPtr     maxTextLenghtEvaluator  = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "maxTextLenght", timeEvaluator );

    //Register all parameters and evaloators in models
    vsModel->RegisterAll( trTxEvaluator );
	psModel->RegisterAll( outlineColorEvaluator );
    psModel->RegisterAll( alphaEvaluator );
    plModel->RegisterAll( blurSizeEvaluator );
	plModel->RegisterAll( outlineSizeEvaluator );
    plModel->RegisterAll( spacingEvaluator );
    plModel->RegisterAll( alignmentEvaluator );
    plModel->RegisterAll( fontSizeEvaluator );
    plModel->RegisterAll( maxTextLenghtEvaluator );

    //Set models structure
    model->SetVertexShaderChannelModel( vsModel );
    model->SetPixelShaderChannelModel( psModel );
    model->SetPluginModel( plModel );

    //Set default values of all parameters
    alphaEvaluator->Parameter()->SetVal( 1.f, TimeType( 0.0 ) );
    blurSizeEvaluator->Parameter()->SetVal( 0.f, TimeType( 0.0 ) );
	outlineSizeEvaluator->Parameter()->SetVal( 0.f, TimeType( 0.0 ) );
    spacingEvaluator->Parameter()->SetVal( 0.f, TimeType( 0.0 ) );
    alignmentEvaluator->Parameter()->SetVal( 0.f, TimeType( 0.0 ) );
	outlineColorEvaluator->Parameter()->SetVal( glm::vec4( 0.f, 0.f, 0.f, 0.f ), TimeType( 0.f ) );
    trTxEvaluator->Parameter()->Transform().InitializeDefaultSRT();
    fontSizeEvaluator->Parameter()->SetVal( 8.f, TimeType( 0.f ) );
    maxTextLenghtEvaluator->Parameter()->SetVal( 0.f, TimeType( 0.f ) );

    return model;
}

// *******************************
//
bool                   DefaultTextPluginDesc::CanBeAttachedTo   ( IPluginConstPtr plugin ) const
{
    if ( plugin == nullptr )
    {
        return true;
    }

    auto  vac = plugin->GetVertexAttributesChannel();
    if ( vac != nullptr )
    {
        return false;
    }

    return true;
}

// *******************************
//
std::string             DefaultTextPluginDesc::UID                      ()
{
    return "DEFAULT_TEXT";
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
    ValueMat4PtrVec values;
	values.push_back( m_scaleValue );
	m_transformChannel = DefaultTransformChannelPtr( DefaultTransformChannel::Create( m_prevPlugin, values, false ) ); //<3

	HelperPixelShaderChannel::CloneRenderContext( m_psc, prev );
	auto ctx = m_psc->GetRendererContext();
	ctx->cullCtx->enabled = false;
	ctx->alphaCtx->blendEnabled = true;
	ctx->alphaCtx->srcBlendMode = model::AlphaContext::SrcBlendMode::SBM_ONE;
	ctx->alphaCtx->dstBlendMode = model::AlphaContext::DstBlendMode::DBM_ONE_MINUS_SRC_ALPHA;
}

// *************************************
// 
DefaultTextPlugin::DefaultTextPlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin< IPlugin >( name, uid, prev, model )
    , m_psc( nullptr )
    , m_vsc( nullptr )
    , m_vaChannel( nullptr )
    , m_atlas( nullptr )
    , m_text( L"" )
	, m_textLength( 0.f )
{
    m_psc = DefaultPixelShaderChannel::Create( model->GetPixelShaderChannelModel() );
    m_vsc = DefaultVertexShaderChannel::Create( model->GetVertexShaderChannelModel() );
	m_vaChannel = TextHelper::CreateEmptyVACForText();

    SetPrevPlugin( prev );

    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &DefaultTextPlugin::OnSetText ), KeyPressedEvent::Type() );

    m_spacingParam          = QueryTypedParam< ParamFloatPtr >( GetPluginParamValModel()->GetPluginModel()->GetParameter( "spacing" ) );
    m_alignmentParam        = QueryTypedParam< ParamFloatPtr >( GetPluginParamValModel()->GetPluginModel()->GetParameter( "alignment" ) );
    m_maxTextLengthParam    = QueryTypedParam< ParamFloatPtr >( GetPluginParamValModel()->GetPluginModel()->GetParameter( "maxTextLenght" ) );
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
void							DefaultTextPlugin::LoadAtlas	( const FontAssetDescConstPtr & fontAssetDesc )
{
	auto txData = m_psc->GetTexturesDataImpl();
    assert( txData->GetTextures().size() <= 1 );

	auto fontResource = LoadTypedAsset<FontAsset>( fontAssetDesc );

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
}

// *************************************
// 
bool                            DefaultTextPlugin::LoadResource  ( AssetDescConstPtr assetDescr )
{
	auto txAssetDescr = QueryTypedDesc< FontAssetDescConstPtr >( assetDescr );

    if ( txAssetDescr != nullptr )
    {
		m_fontSize = txAssetDescr->GetFontSize();
		m_blurSize = txAssetDescr->GetBlurSize();
		m_outlineSize = txAssetDescr->GetOutlineSize();
		LoadAtlas( txAssetDescr );
		SetText( m_text );

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
IPixelShaderChannelConstPtr         DefaultTextPlugin::GetPixelShaderChannel       () const
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
ITransformChannelConstPtr           DefaultTextPlugin::GetTransformChannel         () const
{
	return m_transformChannel;
}


// *************************************
// 
mathematics::RectConstPtr			DefaultTextPlugin::GetAABB						( const glm::mat4 & trans ) const
{
	//auto trParam = GetCurrentParamTransform( this );

	//if( !trParam )
	//	return nullptr;

	//assert( trParam->NumTransforms() <= 1 );

	//if( trParam->NumTransforms() == 1 )
	//{
	//	auto trValue = trParam->Evaluate( 0 );

		auto rect = mathematics::Rect::Create();
		if( AABB( m_vaChannel.get(), trans, rect.get() ) )
			return rect;
		else
			return nullptr;
	//}
	//	
	//return nullptr;
}

// *************************************
// 
void                                DefaultTextPlugin::Update                      ( TimeType t )
{
	BasePlugin::Update( t );

	m_scaleMat = glm::mat4( 1.0 );

	ScaleToMaxTextLength();

	m_scaleValue->SetValue( m_scaleMat );

	//assumption that text plugin provides vertices, so no need for backward topology propagation
	HelperVertexAttributesChannel::PropagateAttributesUpdate( m_vaChannel, m_prevPlugin );
	HelperPixelShaderChannel::PropagateUpdate( m_psc, m_prevPlugin );

    m_vsc->PostUpdate();
    m_psc->PostUpdate();

	if( m_transformChannel )
	{
		m_transformChannel->PostUpdate();
	}
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
    if( evt->GetEventType() == KeyPressedEvent::m_sEventType)
    {
        KeyPressedEventPtr evtTyped = std::static_pointer_cast<KeyPressedEvent>( evt );
        wchar_t c[2] = {evtTyped->GetChar() , '\0'};

        if( c[0] == L'\b' )
        {
            if( !m_text.empty() )
            {
                m_text.pop_back();
                SetText( m_text );
            }
        }
        else
		{
			SetText( m_text + std::wstring( c ) );
		}
    }
}

namespace 
{

// *************************************
//
glm::mat4 BuildScaleMatrix( const glm::vec3 & center, const glm::vec3 & scale )
{
	{ center; }
    return  glm::translate( glm::mat4( 1.f ), center ) *
            glm::scale( glm::mat4( 1.f ), scale ) *
            glm::translate( glm::mat4( 1.f ), -center );
}

// *************************************
//
void TransformPosChannel( VertexAttributesChannelPtr vaChannel, const glm::mat4 & trans )
{
    auto components = vaChannel->GetComponents();

    for( auto cc : components )
    {
        auto vertsNum = cc->GetNumVertices();

        auto arttChannels = cc->GetAttributeChannels();

        if( vertsNum > 0 && arttChannels.size() > 0 )
        {
            auto attrChannelDesc = arttChannels[ 0 ]->GetDescriptor();
            { attrChannelDesc; } // FIXME: suppress unused warning
            assert( attrChannelDesc->GetType() == AttributeType::AT_FLOAT3 );
            assert( attrChannelDesc->GetSemantic() == AttributeSemantic::AS_POSITION );

            auto f3AttrChannel = std::static_pointer_cast< Float3AttributeChannel >( arttChannels[ 0 ] );
            auto & verts = f3AttrChannel->GetVertices();

            for( auto& v : verts )
            {
                glm::vec4 tmp( v.x, v.y, v.z, 1.f ); 
                tmp = trans * tmp;
                v.x = tmp.x;
                v.y = tmp.y;
                v.z = tmp.z;
            }
        }

        
    }
}

} // anonymous

// *************************************
//
void DefaultTextPlugin::ScaleToMaxTextLength		()
{
	auto maxTextLenght = m_maxTextLengthParam->Evaluate();

    if( maxTextLenght > 0.f && m_textLength > 0.f && m_textLength > maxTextLenght )
    {
        //auto center = glm::vec3( 0.f, 0.f, 0.f );

        //switch( EvaluateAsInt< TextAlignmentType >( m_alignmentParam ) )
        //{
        //case TextAlignmentType::Center:
        //    center = glm::vec3( m_textLength / 2.f, 0.f, 0.f );
        //    break;
        //case TextAlignmentType::Right:
        //    center = glm::vec3( m_textLength, 0.f, 0.f );
        //    break;
        //default:
        //    break;
        //}

        m_scaleMat = glm::scale( glm::mat4( 1.f ), glm::vec3( maxTextLenght / m_textLength, 1.f, 1.f ) );

		m_scaleValue->SetValue( m_scaleMat );
    }
}

// *************************************
//
void DefaultTextPlugin::SetText                     ( const std::wstring & newText )
{
    m_text = newText;

    m_vaChannel->ClearAll();

    auto alignType		=  EvaluateAsInt< TextAlignmentType >( m_alignmentParam );

    m_textLength = TextHelper::BuildVACForText( m_vaChannel.get(), m_atlas, m_text, m_blurSize, m_spacingParam->Evaluate(), alignType, m_outlineSize, false );

	ScaleToMaxTextLength();

	HelperVertexAttributesChannel::SetTopologyUpdate( m_vaChannel );
}

// *************************************
//
bool DefaultTextPlugin::SetText( IPluginPtr textPlugin, const std::wstring& text )
{
    if( textPlugin->GetTypeUid() == DefaultTextPluginDesc::UID() )
    {
        std::static_pointer_cast< DefaultTextPlugin >( textPlugin )->SetText( text );
        return true;
    }
    else
        return false;
}

} // model
} // bv
