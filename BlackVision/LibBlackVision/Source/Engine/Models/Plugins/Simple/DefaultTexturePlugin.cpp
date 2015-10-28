#include "DefaultTexturePlugin.h"

#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"
#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"
#include "Engine/Models/Plugins/Channels/Geometry/VacAABB.h"
#include "Engine/Models/Plugins/Channels/Geometry/HelperVertexAttributesChannel.h"
#include "Engine/Models/Plugins/HelperUVGenerator.h"

#include "Assets/Texture/TextureAssetDescriptor.h"


namespace bv { namespace model {

// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultTexturePluginDesc::DefaultTexturePluginDesc                          ()
    : BasePluginDescriptor( UID(), "texture", "tx" )
{
}

// *******************************
//
IPluginPtr              DefaultTexturePluginDesc::CreatePlugin              ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultTexturePlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   DefaultTexturePluginDesc::CreateDefaultModel( ITimeEvaluatorPtr timeEvaluator ) const
{
    //Create all models
    DefaultPluginParamValModelPtr model  = std::make_shared< DefaultPluginParamValModel >();
    DefaultParamValModelPtr psModel      = std::make_shared< DefaultParamValModel >();
    DefaultParamValModelPtr vsModel      = std::make_shared< DefaultParamValModel >();

    //Create all parameters and evaluators
    SimpleVec4EvaluatorPtr      borderColorEvaluator = ParamValEvaluatorFactory::CreateSimpleVec4Evaluator( "borderColor", timeEvaluator );
    SimpleFloatEvaluatorPtr     alphaEvaluator   = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "alpha", timeEvaluator );
    SimpleTransformEvaluatorPtr trTxEvaluator    = ParamValEvaluatorFactory::CreateSimpleTransformEvaluator( "txMat", timeEvaluator );
	SimpleFloatEvaluatorPtr		wrapModeXEvaluator = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "wrapModeX", timeEvaluator );
	SimpleFloatEvaluatorPtr		wrapModeYEvaluator = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "wrapModeY", timeEvaluator );

    //ParamFloatPtr  paramWrapModeX     = ParametersFactory::CreateParameterFloat( "wrapModeX", timeEvaluator );
    //ParamFloatPtr  paramWrapModeY     = ParametersFactory::CreateParameterFloat( "wrapModeY", timeEvaluator );
    ParamFloatPtr  paramFilteringMode = ParametersFactory::CreateParameterFloat( "filteringMode", timeEvaluator );
    ParamFloatPtr  paramAttachMode    = ParametersFactory::CreateParameterFloat( "attachmentMode", timeEvaluator );

    //Register all parameters and evaloators in models
    vsModel->RegisterAll( trTxEvaluator );
    psModel->RegisterAll( borderColorEvaluator );
    psModel->RegisterAll( alphaEvaluator );
	psModel->RegisterAll( wrapModeXEvaluator );
	psModel->RegisterAll( wrapModeYEvaluator );
    //psModel->AddParameter( paramWrapModeX );
    //psModel->AddParameter( paramWrapModeY );
    psModel->AddParameter( paramFilteringMode );
    psModel->AddParameter( paramAttachMode );

    //Set models structure
    model->SetVertexShaderChannelModel( vsModel );
    model->SetPixelShaderChannelModel( psModel );

    //Set default values of all parameters
    alphaEvaluator->Parameter()->SetVal( 1.f, TimeType( 0.0 ) );
    borderColorEvaluator->Parameter()->SetVal( glm::vec4( 0.f, 0.f, 0.f, 0.f ), TimeType( 0.f ) );
    trTxEvaluator->Parameter()->Transform().InitializeDefaultSRT();
	wrapModeXEvaluator->Parameter()->SetVal( static_cast<int>( TextureWrappingMode::TWM_CLAMP_BORDER ), TimeType( 0.0 ) ); 
	wrapModeYEvaluator->Parameter()->SetVal( static_cast<int>( TextureWrappingMode::TWM_CLAMP_BORDER ), TimeType( 0.0 ) ); 

    //FIXME: integer parmeters should be used here
    //paramWrapModeX->SetVal( (float) TextureWrappingMode::TWM_REPEAT, TimeType( 0.f ) );
    //paramWrapModeY->SetVal( (float) TextureWrappingMode::TWM_REPEAT, TimeType( 0.f ) );
    paramFilteringMode->SetVal( (float) TextureFilteringMode::TFM_LINEAR, TimeType( 0.f ) );
    paramAttachMode->SetVal( (float) TextureAttachmentMode::MM_ATTACHED, TimeType( 0.f ) );

    return model;
}

// *******************************
//
bool                   DefaultTexturePluginDesc::CanBeAttachedTo     ( IPluginConstPtr plugin ) const
{
    if ( plugin == nullptr )
    {
        return false;
    }

    auto  vac = plugin->GetVertexAttributesChannel();
    if ( vac == nullptr )
    {
        return false;
    }

    //auto numChannels = vac->GetDescriptor()->GetNumVertexChannels();
    //if ( numChannels != 1 ) //only vertex attribute data allowed here
    //{
    //    return false;
    //}

    return true;
}

// *******************************
//
std::string             DefaultTexturePluginDesc::UID                       ()
{
    return "DEFAULT_TEXTURE";
}

// *******************************
// 
std::string             DefaultTexturePluginDesc::TextureName               ()
{
    return "Tex0";
}

//FIXME: dodawanie kanalow w ten sposob (przez przypisanie na m_<xxx>channel powoduje bledy, trzeba to jakos poprawic, zeby bylo wiadomo, o co chodzi
//FIXME: teraz zle dodanie wychodzi dopiero po odpaleniu silnika, a to jest oczywisty blad

// ************************************************************************* PLUGIN *************************************************************************

// *************************************
// 
void DefaultTexturePlugin::SetPrevPlugin( IPluginPtr prev )
{
    BasePlugin::SetPrevPlugin( prev );

    InitVertexAttributesChannel();
}

// *************************************
// 
DefaultTexturePlugin::DefaultTexturePlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin< IPlugin >( name, uid, prev, std::static_pointer_cast< IPluginParamValModel >( model ) )
    , m_psc( nullptr )
    , m_vsc( nullptr )
    , m_vaChannel( nullptr )
    , m_paramValModel( model )
{
    m_psc = DefaultPixelShaderChannel::Create( model->GetPixelShaderChannelModel() );
    m_vsc = DefaultVertexShaderChannel::Create( model->GetVertexShaderChannelModel() );

    SetPrevPlugin( prev );

    auto ctx = m_psc->GetRendererContext();
    ctx->cullCtx->enabled = false;
    
    ctx->alphaCtx->blendEnabled = true;
    ctx->alphaCtx->srcBlendMode = model::AlphaContext::SrcBlendMode::SBM_SRC_ALPHA;
    ctx->alphaCtx->dstBlendMode = model::AlphaContext::DstBlendMode::DBM_ONE_MINUS_SRC_ALPHA;

    m_texturesData = m_psc->GetTexturesDataImpl();

    //Direct param state access (to bypass model querying)
    auto psModel = PixelShaderChannelModel();
    
    //m_paramWrapModeX        = QueryTypedParam< ParamFloatPtr >( psModel->GetParameter( "wrapModeX" ) );
    //m_paramWrapModeY        = QueryTypedParam< ParamFloatPtr >( psModel->GetParameter( "wrapModeY" ) );
    m_paramFilteringMode    = QueryTypedParam< ParamFloatPtr >( psModel->GetParameter( "filteringMode" ) );
    m_paramAttachMode       = QueryTypedParam< ParamFloatPtr >( psModel->GetParameter( "attachmentMode" ) );

    //assert( m_paramWrapModeX );
    //assert( m_paramWrapModeY );
    assert( m_paramFilteringMode );
    assert( m_paramAttachMode );

    UpdateState();
}

// *************************************
// 
DefaultTexturePlugin::~DefaultTexturePlugin         ()
{
}

// *************************************
// 
bool                            DefaultTexturePlugin::LoadResource  ( AssetDescConstPtr assetDescr )
{
	auto txAssetDescr = QueryTypedDesc< TextureAssetDescConstPtr >( assetDescr );

    // FIXME: dodac tutaj API pozwalajace tez ustawiac parametry dodawanej tekstury (normalny load z dodatkowymi parametrami)
    if ( txAssetDescr != nullptr )
    {
        auto txData = m_psc->GetTexturesDataImpl();
        assert( txData->GetTextures().size() <= 2 ); //FIXME: Second one may be added by a mask

        //FIXME: use some better API to handle resources in general and textures in this specific case
        auto txDesc = DefaultTextureDescriptor::LoadTexture( txAssetDescr, DefaultTexturePluginDesc::TextureName() );
        txDesc->SetSemantic( DataBuffer::Semantic::S_TEXTURE_STATIC );
        txDesc->SetBorderColor( GetBorderColor() );

        if( txDesc != nullptr )
        {
            txData->SetTexture( 0, txDesc );

			m_textureWidth = txAssetDescr->GetOrigTextureDesc()->GetWidth();
            m_textureHeight = txAssetDescr->GetOrigTextureDesc()->GetHeight();

            return true;
        }
    }

    return false;
}

// *************************************
// 
IVertexAttributesChannelConstPtr    DefaultTexturePlugin::GetVertexAttributesChannel  () const
{
    return m_vaChannel;
}

// *************************************
// 
IPixelShaderChannelConstPtr         DefaultTexturePlugin::GetPixelShaderChannel       () const
{
    return m_psc;
}

// *************************************
// 
IVertexShaderChannelConstPtr        DefaultTexturePlugin::GetVertexShaderChannel      () const
{
    return m_vsc;
}

// *************************************
// 
void                                DefaultTexturePlugin::Update                      ( TimeType t )
{
    { t; } // FIXME: suppress unused warning
    m_paramValModel->Update();

	HelperVertexAttributesChannel::AttributesUpdate( m_vaChannel, UpdateState() );
	HelperVertexAttributesChannel::FetchAttributesUpdate( m_vaChannel, m_prevPlugin );
	if( HelperVertexAttributesChannel::FetchTopologyUpdate( m_vaChannel, m_prevPlugin ) )
	{
		InitVertexAttributesChannel();
	}

	//if( m_vaChannel )
	//{
	//	bool hasPrevVAC = m_prevPlugin && m_prevPlugin->GetVertexAttributesChannel();
	//	if ( ( hasPrevVAC && m_prevPlugin->GetVertexAttributesChannel()->NeedsAttributesUpdate() )
	//		|| StateChanged( wX, wY, fm, attachmentMode ) )
	//	{
	//		UpdateState( wX, wY, fm, attachmentMode );
	//		m_vaChannel->SetNeedsAttributesUpdate( true );
	//	}
	//	else
	//	{
	//		m_vaChannel->SetNeedsAttributesUpdate( false );
	//	}

	//	if( hasPrevVAC && m_prevPlugin->GetVertexAttributesChannel()->NeedsTopologyUpdate() )
	//	{
	//		m_vaChannel->ClearAll();
	//		InitVertexAttributesChannel( m_prevPlugin );
	//		m_vaChannel->SetNeedsTopologyUpdate( true );
	//		m_vaChannel->SetNeedsAttributesUpdate( false ); // FIXME: very ugly hack this is
	//	}
	//	else
	//	{
	//		m_vaChannel->SetNeedsTopologyUpdate( false );
	//	}
	//}

    m_vsc->PostUpdate();
    m_psc->PostUpdate();    
}

// *************************************
//
void		DefaultTexturePlugin::InitVertexAttributesChannel		()
{
	if( !( m_prevPlugin && m_prevPlugin->GetVertexAttributesChannel() ) )
	{
		m_vaChannel = nullptr;
		return;
	}

    auto prevGeomChannel = m_prevPlugin->GetVertexAttributesChannel();
	auto prevCC = prevGeomChannel->GetComponents();

    //Only one texture
	//FIXME: is it possible that CC is empty?
	auto vaChannelDesc = HelperVertexAttributesChannel::CreateVertexAttributesChannelDescriptor( prevCC[ 0 ]->GetAttributeChannels() );
	if( !AttributeChannel::GetAttrChannel( prevCC[ 0 ]->GetAttributeChannels(), AttributeSemantic::AS_TEXCOORD ) )
	{
		vaChannelDesc.AddAttrChannelDesc( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );
	}
	
	if( !m_vaChannel )
	{		
		m_vaChannel = std::make_shared< VertexAttributesChannel >( prevGeomChannel->GetPrimitiveType(), vaChannelDesc, true, prevGeomChannel->IsTimeInvariant() );
	}
	else
	{
		m_vaChannel->ClearAll();
		m_vaChannel->SetDescriptor( vaChannelDesc );
	}

	auto desc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );
    for( unsigned int i = 0; i < prevCC.size(); ++i )
    {
        auto connComp = ConnectedComponent::Create();

        auto prevConnComp = std::static_pointer_cast< const model::ConnectedComponent >( prevCC[ i ] );
        auto prevCompChannels = prevConnComp->GetAttributeChannelsPtr();

        for( auto prevCompCh : prevCompChannels )
        {
            connComp->AddAttributeChannel( prevCompCh );
        }

		auto posChannel = AttributeChannel::GetAttrChannel( prevConnComp->GetAttributeChannels(), AttributeSemantic::AS_POSITION );
		if( posChannel && !AttributeChannel::GetAttrChannel( prevConnComp->GetAttributeChannels(), AttributeSemantic::AS_TEXCOORD ) )
		{
			//FIXME: only one texture - convex hull calculations
			auto uvs = new model::Float2AttributeChannel( desc, DefaultTexturePluginDesc::TextureName(), true );
			auto uvsPtr = Float2AttributeChannelPtr( uvs );
			
			Helper::UVGenerator::generateUV( reinterpret_cast< const glm::vec3 * >( posChannel->GetData() ), posChannel->GetNumEntries(),
											uvsPtr, glm::vec3( 1.0, 0.0, 0.0 ), glm::vec3( 0.0, 1.0, 0.0 ), true );

			connComp->AddAttributeChannel( uvsPtr );
		}

        m_vaChannel->AddConnectedComponent( connComp );
    }
	
    assert( prevGeomChannel->GetComponents().size() > 0 );
}

namespace {

// *************************************
// FIXME: implement int parameters and bool parameters
template< typename EnumClassType >
inline EnumClassType EvaluateAsInt( ParamFloat * param )
{
    int val = int( param->Evaluate() );

    return EnumClassType( val );
}

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
TextureWrappingMode                         DefaultTexturePlugin::GetWrapModeX          () const
{
    //return EvaluateAsInt< TextureWrappingMode >( m_paramWrapModeX );
	auto param = this->GetParameter( "wrapModeX" );
	assert( param );
	return static_cast<TextureWrappingMode>( static_cast<int>( QueryTypedParam< ParamFloatPtr >( param )->Evaluate() ) );
}

// *************************************
// 
TextureWrappingMode                         DefaultTexturePlugin::GetWrapModeY          () const
{
    //return EvaluateAsInt< TextureWrappingMode >( m_paramWrapModeY );
	auto param = this->GetParameter( "wrapModeY" );
	assert( param );
	return static_cast<TextureWrappingMode>( static_cast<int>( QueryTypedParam< ParamFloatPtr >( param )->Evaluate() ) );
}

// *************************************
// 
TextureFilteringMode                        DefaultTexturePlugin::GetFilteringMode      () const
{
    return EvaluateAsInt< TextureFilteringMode >( m_paramFilteringMode );
}

// *************************************
// 
TextureAttachmentMode                       DefaultTexturePlugin::GetAttachementMode    () const
{
    return EvaluateAsInt< TextureAttachmentMode >( m_paramAttachMode );
}

// *************************************
// 
bool                                        DefaultTexturePlugin::UpdateState           ()
{
	auto wmx = GetWrapModeX();
	auto wmy = GetWrapModeY();
	auto fm = GetFilteringMode();
	auto am = GetAttachementMode();

	if( wmx != m_lastTextureWrapModeX || wmy != m_lastTextureWrapModeY || 
		fm != m_lastTextureFilteringMode || am != m_lastTextureAttachMode )
	{
		m_lastTextureWrapModeX      = wmx;
		m_lastTextureWrapModeY      = wmy;
		m_lastTextureFilteringMode  = fm;
		m_lastTextureAttachMode     = am;
		
		return true;
	}
	return false;
}

// *************************************
// 
SizeType									DefaultTexturePlugin::GetTextureWidth             () const
{
    return m_textureWidth;
}

// *************************************
//
SizeType									DefaultTexturePlugin::GetTextureHeight            () const
{
    return m_textureHeight;
}

// *************************************
// 
mathematics::RectConstPtr					DefaultTexturePlugin::GetAABB						( const glm::mat4 & trans ) const
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
glm::vec4                                   DefaultTexturePlugin::GetBorderColor        () const
{
	auto param = this->GetParameter( "borderColor" );
	assert( param );
	return QueryTypedParam< ParamVec4Ptr >( param )->Evaluate();
}


} // model
} // bv
