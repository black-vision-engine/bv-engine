#include "DefaultAnimationPlugin.h"

#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"
#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"
#include "Engine/Models/Plugins/Channels/Geometry/HelperVertexAttributesChannel.h"
#include "Engine/Models/Plugins/HelperUVGenerator.h"

#include "Assets/Texture/AnimationAssetDescriptor.h"


namespace bv { namespace model {


// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultAnimationPluginDesc::DefaultAnimationPluginDesc                          ()
    : BasePluginDescriptor( UID(), "animation", "tx" )
{
}

// *******************************
//
IPluginPtr              DefaultAnimationPluginDesc::CreatePlugin              ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultAnimationPlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   DefaultAnimationPluginDesc::CreateDefaultModel( ITimeEvaluatorPtr timeEvaluator ) const
{
    //Create all models
    DefaultPluginParamValModelPtr model  = std::make_shared< DefaultPluginParamValModel >();
    DefaultParamValModelPtr psModel      = std::make_shared< DefaultParamValModel >();
    DefaultParamValModelPtr vsModel      = std::make_shared< DefaultParamValModel >();

    //Create all parameters and evaluators
    SimpleVec4EvaluatorPtr      borderColorEvaluator = ParamValEvaluatorFactory::CreateSimpleVec4Evaluator( "borderColor", timeEvaluator );
    SimpleFloatEvaluatorPtr     alphaEvaluator   = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "alpha", timeEvaluator );
    SimpleTransformEvaluatorPtr trTxEvaluator    = ParamValEvaluatorFactory::CreateSimpleTransformEvaluator( "txMat", timeEvaluator );

    ParamFloatPtr  paramFrameNum      = ParametersFactory::CreateParameterFloat( "frameNum", timeEvaluator );
    ParamFloatPtr  paramWrapModeX     = ParametersFactory::CreateParameterFloat( "wrapModeX", timeEvaluator );
    ParamFloatPtr  paramWrapModeY     = ParametersFactory::CreateParameterFloat( "wrapModeY", timeEvaluator );
    ParamFloatPtr  paramFilteringMode = ParametersFactory::CreateParameterFloat( "filteringMode", timeEvaluator );
    ParamFloatPtr  paramAttachMode    = ParametersFactory::CreateParameterFloat( "attachmentMode", timeEvaluator );

    //Register all parameters and evaloators in models
    vsModel->RegisterAll( trTxEvaluator );
    psModel->RegisterAll( borderColorEvaluator );
    psModel->RegisterAll( alphaEvaluator );
    psModel->AddParameter( paramFrameNum );
    psModel->AddParameter( paramWrapModeX );
    psModel->AddParameter( paramWrapModeY );
    psModel->AddParameter( paramFilteringMode );
    psModel->AddParameter( paramAttachMode );

    //Set models structure
    model->SetVertexShaderChannelModel( vsModel );
    model->SetPixelShaderChannelModel( psModel );

    //Set default values of all parameters
    alphaEvaluator->Parameter()->SetVal( 1.f, TimeType( 0.0 ) );
    borderColorEvaluator->Parameter()->SetVal( glm::vec4( 0.f, 0.f, 0.f, 0.f ), TimeType( 0.f ) );
    trTxEvaluator->Parameter()->Transform().InitializeDefaultSRT();

    //FIXME: integer parmeters should be used here
    paramFrameNum->SetVal( 0.f, TimeType( 0.f ) );
    paramWrapModeX->SetVal( (float) TextureWrappingMode::TWM_REPEAT, TimeType( 0.f ) );
    paramWrapModeY->SetVal( (float) TextureWrappingMode::TWM_REPEAT, TimeType( 0.f ) );
    paramFilteringMode->SetVal( (float) TextureFilteringMode::TFM_LINEAR, TimeType( 0.f ) );
    paramAttachMode->SetVal( (float) TextureAttachmentMode::MM_ATTACHED, TimeType( 0.f ) );

    return model;
}

// *******************************
//
bool                   DefaultAnimationPluginDesc::CanBeAttachedTo     ( IPluginConstPtr plugin ) const
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

    auto numChannels = vac->GetDescriptor()->GetNumVertexChannels();
    if ( numChannels != 1 ) //only vertex attribute data allowed here
    {
        return false;
    }

    return true;
}

// *******************************
//
std::string             DefaultAnimationPluginDesc::UID                       ()
{
    return "DEFAULT_ANIMATION";
}

// *******************************
//
std::string             DefaultAnimationPluginDesc::TextureName               ()
{
    return "Tex0";
}

//FIXME: dodawanie kanalow w ten sposob (przez przypisanie na m_<xxx>channel powoduje bledy, trzeba to jakos poprawic, zeby bylo wiadomo, o co chodzi
//FIXME: teraz zle dodanie wychodzi dopiero po odpaleniu silnika, a to jest oczywisty blad

// ************************************************************************* PLUGIN *************************************************************************

void								DefaultAnimationPlugin::SetPrevPlugin               ( IPluginPtr prev )
{
	BasePlugin::SetPrevPlugin( prev );

    InitVertexAttributesChannel();
}

// *************************************
// 
DefaultAnimationPlugin::DefaultAnimationPlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin< IPlugin >( name, uid, prev, std::static_pointer_cast< IPluginParamValModel >( model ) )
    , m_psc( nullptr )
    , m_vsc( nullptr )
    , m_vaChannel( nullptr )
    , m_paramValModel( model )
{
    m_psc = DefaultPixelShaderChannelPtr( DefaultPixelShaderChannel::Create( model->GetPixelShaderChannelModel(), nullptr ) );
    m_vsc = DefaultVertexShaderChannelPtr( DefaultVertexShaderChannel::Create( model->GetVertexShaderChannelModel() ) );

    SetPrevPlugin( prev );

    auto ctx = m_psc->GetRendererContext();
    ctx->cullCtx->enabled = false;

    m_texturesData = m_psc->GetTexturesDataImpl();

    //Direct param state access (to bypass model querying)
    auto psModel = PixelShaderChannelModel();
    
    m_paramFrameNum         = QueryTypedParam< ParamFloatPtr >( psModel->GetParameter( "frameNum" ) );
    m_paramWrapModeX        = QueryTypedParam< ParamFloatPtr >( psModel->GetParameter( "wrapModeX" ) );
    m_paramWrapModeY        = QueryTypedParam< ParamFloatPtr >( psModel->GetParameter( "wrapModeY" ) );
    m_paramFilteringMode    = QueryTypedParam< ParamFloatPtr >( psModel->GetParameter( "filteringMode" ) );
    m_paramAttachMode       = QueryTypedParam< ParamFloatPtr >( psModel->GetParameter( "attachmentMode" ) );

    assert( m_paramFrameNum );
    assert( m_paramWrapModeX );
    assert( m_paramWrapModeY );
    assert( m_paramFilteringMode );
    assert( m_paramAttachMode );

    UpdateState();
}

// *************************************
// 
DefaultAnimationPlugin::~DefaultAnimationPlugin         ()
{
}

// *************************************
// 
bool                            DefaultAnimationPlugin::LoadResource  ( AssetDescConstPtr assetDescr )
{
	auto animAssetDescr = QueryTypedDesc< AnimationAssetDescConstPtr >( assetDescr );

    // FIXME: dodac tutaj API pozwalajace tez ustawiac parametry dodawanej tekstury (normalny load z dodatkowymi parametrami)
    if ( animAssetDescr != nullptr )
    {
        auto txData = m_psc->GetTexturesDataImpl();
        assert( txData->GetAnimations().size() <= 1 );

        //FIXME: use some better API to handle resources in general and textures in this specific case
        auto animDesc = DefaultAnimationDescriptor::LoadAnimation( animAssetDescr, DefaultAnimationPluginDesc::TextureName() );

        if( animDesc != nullptr )
        {
			if( !txData->SetAnimation( 0, animDesc ) )
			{
				txData->AddAnimation( animDesc );
			}
            return true;
        }
    }

    return false;
}

// *************************************
// 
IVertexAttributesChannelConstPtr    DefaultAnimationPlugin::GetVertexAttributesChannel  () const
{
    return m_vaChannel;
}

// *************************************
// 
IPixelShaderChannelConstPtr         DefaultAnimationPlugin::GetPixelShaderChannel       () const
{
    return m_psc;
}

// *************************************
// 
IVertexShaderChannelConstPtr        DefaultAnimationPlugin::GetVertexShaderChannel      () const
{
    return m_vsc;
}

// *************************************
// 
void                                DefaultAnimationPlugin::Update                      ( TimeType t )
{
    { t; } // FIXME: suppress unused variable
    m_paramValModel->Update();

    unsigned int frameNum = (unsigned int )m_paramFrameNum->Evaluate(); // TODO: A to chyba juz nie potrzebne bo Update na modelu zrobiony
    m_texturesData->SetAnimationFrame( 0, frameNum ); // TODO: A to chyba juz nie potrzebne bo Update na modelu zrobiony

	HelperVertexAttributesChannel::SetAttributesUpdate( m_vaChannel, UpdateState() );
	HelperVertexAttributesChannel::PropagateAttributesUpdate( m_vaChannel, m_prevPlugin );
	if( HelperVertexAttributesChannel::PropagateTopologyUpdate( m_vaChannel, m_prevPlugin ) )
	{
		InitVertexAttributesChannel();
	}

	//bool prevVAC = m_prevPlugin && m_prevPlugin->GetVertexAttributesChannel();
	//if( m_vaChannel )
	//{
	//	if ( ( prevVAC && m_prevPlugin->GetVertexAttributesChannel()->NeedsAttributesUpdate() )
	//		|| StateChanged() )
	//	{
	//		UpdateState();
	//		m_vaChannel->SetNeedsAttributesUpdate( true );
	//	}
	//	else
	//	{
	//		m_vaChannel->SetNeedsAttributesUpdate( false );
	//	}
	//}

    m_vsc->PostUpdate();
    m_psc->PostUpdate();    
}

// *************************************
//
void		DefaultAnimationPlugin::InitVertexAttributesChannel		()
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
	m_vaChannel->SetLastTopologyUpdateID( prevGeomChannel->GetLastTopologyUpdateID() );
	
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
			auto uvs = new model::Float2AttributeChannel( desc, DefaultAnimationPluginDesc::TextureName(), true );
			auto uvsPtr = Float2AttributeChannelPtr( uvs );
			
			Helper::UVGenerator::generateUV( reinterpret_cast< const glm::vec3 * >( posChannel->GetData() ), posChannel->GetNumEntries(),
											uvsPtr, glm::vec3( 1.0, 0.0, 0.0 ), glm::vec3( 0.0, 1.0, 0.0 ), true );

			connComp->AddAttributeChannel( uvsPtr );
		}

        m_vaChannel->AddConnectedComponent( connComp );
    }
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
TextureWrappingMode                         DefaultAnimationPlugin::GetWrapModeX            () const
{
    return EvaluateAsInt< TextureWrappingMode >( m_paramWrapModeX );
}

// *************************************
// 
TextureWrappingMode                         DefaultAnimationPlugin::GetWrapModeY            () const
{
    return EvaluateAsInt< TextureWrappingMode >( m_paramWrapModeY );
}

// *************************************
// 
TextureFilteringMode                        DefaultAnimationPlugin::GetFilteringMode        () const
{
    return EvaluateAsInt< TextureFilteringMode >( m_paramFilteringMode );
}

// *************************************
// 
TextureAttachmentMode                       DefaultAnimationPlugin::GetAttachementMode      () const
{
    return EvaluateAsInt< TextureAttachmentMode >( m_paramAttachMode );
}

// *************************************
// 
bool                                        DefaultAnimationPlugin::UpdateState             ()
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

} // model
} // bv
