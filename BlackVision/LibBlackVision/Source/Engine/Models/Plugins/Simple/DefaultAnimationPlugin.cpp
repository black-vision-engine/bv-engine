#include "DefaultAnimationPlugin.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"
#include "Engine/Models/Plugins/Channels/Geometry/HelperVertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"

#include "Engine/Models/Plugins/HelperUVGenerator.h"

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
    DefaultPluginParamValModelPtr model  = std::make_shared< DefaultPluginParamValModel >( timeEvaluator );
    DefaultParamValModelPtr psModel      = std::make_shared< DefaultParamValModel >();
    DefaultParamValModelPtr vsModel      = std::make_shared< DefaultParamValModel >();

    //Create all parameters and evaluators
    SimpleFloatEvaluatorPtr     alphaEvaluator   = ParamValEvaluatorFactory::CreateSimpleFloatEvaluator( "alpha", timeEvaluator );
    SimpleTransformEvaluatorPtr trTxEvaluator    = ParamValEvaluatorFactory::CreateSimpleTransformEvaluator( "txMat", timeEvaluator );

    ParamFloatPtr  paramFrameNum      = ParametersFactory::CreateParameterFloat( "frameNum", timeEvaluator );

    //Register all parameters and evaloators in models
    vsModel->RegisterAll( trTxEvaluator );
    psModel->RegisterAll( alphaEvaluator );
    psModel->AddParameter( paramFrameNum );

    //Set models structure
    model->SetVertexShaderChannelModel( vsModel );
    model->SetPixelShaderChannelModel( psModel );

    //Set default values of all parameters
    alphaEvaluator->Parameter()->SetVal( 1.f, TimeType( 0.0 ) );
    trTxEvaluator->Parameter()->Transform().InitializeDefaultSRT();

    //FIXME: integer parmeters should be used here
    paramFrameNum->SetVal( 0.f, TimeType( 0.f ) );

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

    //auto numChannels = vac->GetDescriptor()->GetNumVertexChannels();
    //if ( numChannels != 1 ) //only vertex attribute data allowed here
    //{
    //    return false;
    //}

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
	    
	HelperPixelShaderChannel::CloneRenderContext( m_psc, prev );
	auto ctx = m_psc->GetRendererContext();
    ctx->cullCtx->enabled = false;
	ctx->alphaCtx->blendEnabled = true;
	ctx->alphaCtx->srcBlendMode = model::AlphaContext::SrcBlendMode::SBM_SRC_ALPHA;
	ctx->alphaCtx->dstBlendMode = model::AlphaContext::DstBlendMode::DBM_ONE_MINUS_SRC_ALPHA;
	//HelperPixelShaderChannel::SetRendererContextUpdate( m_psc );
}

// *************************************
// 
DefaultAnimationPlugin::DefaultAnimationPlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin< IPlugin >( name, uid, prev, model )
    , m_psc( nullptr )
    , m_vsc( nullptr )
    , m_vaChannel( nullptr )
{
    m_psc = DefaultPixelShaderChannel::Create( model->GetPixelShaderChannelModel() );
    m_vsc = DefaultVertexShaderChannel::Create( model->GetVertexShaderChannelModel() );

    SetPrevPlugin( prev );

    m_texturesData = m_psc->GetTexturesDataImpl();
	
	//FIXME: 'reserve' required texture
	m_texturesData->SetAnimation( 0, DefaultAnimationDescriptor::CreateEmptyDesc( DefaultAnimationPluginDesc::TextureName(), m_pluginParamValModel->GetTimeEvaluator() ) );

    //Direct param state access (to bypass model querying)
    auto psModel = PixelShaderChannelModel();
    
    m_paramFrameNum         = QueryTypedParam< ParamFloatPtr >( psModel->GetParameter( "frameNum" ) );
    assert( m_paramFrameNum );
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
        AddAsset( animAssetDescr );

        //FIXME: use some better API to handle resources in general and textures in this specific case
        auto animDesc = DefaultAnimationDescriptor::LoadAnimation( animAssetDescr, DefaultAnimationPluginDesc::TextureName() );

        if( animDesc != nullptr )
        {
			animDesc->SetSamplerState( SamplerStateModel::Create( m_pluginParamValModel->GetTimeEvaluator() ) );
			
			auto txData = m_psc->GetTexturesDataImpl();
			txData->SetAnimation( 0, animDesc );

			HelperPixelShaderChannel::SetTexturesDataUpdate( m_psc );
            
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
IPixelShaderChannelPtr         DefaultAnimationPlugin::GetPixelShaderChannel       () const
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
	BasePlugin::Update( t );

    unsigned int frameNum = ( unsigned int )m_paramFrameNum->Evaluate(); // TODO: A to chyba juz nie potrzebne bo Update na modelu zrobiony
    m_texturesData->SetAnimationFrame( 0, frameNum ); // TODO: A to chyba juz nie potrzebne bo Update na modelu zrobiony

	HelperVertexAttributesChannel::PropagateAttributesUpdate( m_vaChannel, m_prevPlugin );
	if( HelperVertexAttributesChannel::PropagateTopologyUpdate( m_vaChannel, m_prevPlugin ) )
	{
		InitVertexAttributesChannel();
	}

	HelperPixelShaderChannel::PropagateUpdate( m_psc, m_prevPlugin );

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
	VertexAttributesChannelDescriptor vaChannelDesc( * static_cast< const VertexAttributesChannelDescriptor * >( prevGeomChannel->GetDescriptor() ) );
	if( !vaChannelDesc.GetAttrChannelDescriptor( AttributeSemantic::AS_TEXCOORD ) )
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

		auto posChannel = prevConnComp->GetAttrChannel( AttributeSemantic::AS_POSITION );
		if( posChannel && !prevConnComp->GetAttrChannel( AttributeSemantic::AS_TEXCOORD ) )
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

} // model
} // bv