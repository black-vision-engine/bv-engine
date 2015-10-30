#include "DefaultGradientPlugin.h"

#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"
#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"
#include "Engine/Models/Plugins/Channels/Geometry/HelperVertexAttributesChannel.h"

#include "Engine/Models/Plugins/Simple/DefaultTextPlugin.h"
#include "Engine/Models/Plugins/Simple/DefaultTimerPlugin.h"

namespace bv { namespace model {


// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultGradientPluginDesc::DefaultGradientPluginDesc                          ()
    : BasePluginDescriptor( UID(), "linear_gradient", "lg" )
{
}

// *******************************
//
IPluginPtr              DefaultGradientPluginDesc::CreatePlugin              ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultGradientPlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   DefaultGradientPluginDesc::CreateDefaultModel( ITimeEvaluatorPtr timeEvaluator ) const
{
    //Create all models
    DefaultPluginParamValModelPtr model  = std::make_shared< DefaultPluginParamValModel >();
    DefaultParamValModelPtr psModel      = std::make_shared< DefaultParamValModel >();
    DefaultParamValModelPtr vsModel      = std::make_shared< DefaultParamValModel >();

    //Create all parameters and evaluators
    SimpleTransformEvaluatorPtr trTxEvaluator    = ParamValEvaluatorFactory::CreateSimpleTransformEvaluator( "txMat", timeEvaluator );

	SimpleVec4EvaluatorPtr		color1Evaluator	 = ParamValEvaluatorFactory::CreateSimpleVec4Evaluator("color1", timeEvaluator );
	SimpleVec4EvaluatorPtr		color2Evaluator	 = ParamValEvaluatorFactory::CreateSimpleVec4Evaluator("color2", timeEvaluator );

	SimpleVec2EvaluatorPtr		point1Evaluator	 = ParamValEvaluatorFactory::CreateSimpleVec2Evaluator("point1", timeEvaluator );
	SimpleVec2EvaluatorPtr		point2Evaluator	 = ParamValEvaluatorFactory::CreateSimpleVec2Evaluator("point2", timeEvaluator );

    //Register all parameters and evaloators in models
    vsModel->RegisterAll( trTxEvaluator );
	psModel->RegisterAll( color1Evaluator );
	psModel->RegisterAll( color2Evaluator );
	psModel->RegisterAll( point1Evaluator );
	psModel->RegisterAll( point2Evaluator );

    //Set models structure
    model->SetVertexShaderChannelModel( vsModel );
    model->SetPixelShaderChannelModel( psModel );

    //Set default values of all parameters
    trTxEvaluator->Parameter()->Transform().InitializeDefaultSRT();
	color1Evaluator->Parameter()->SetVal( glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f ), TimeType( 0.0 ) );
	color2Evaluator->Parameter()->SetVal( glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f ), TimeType( 0.0 ) );
	point1Evaluator->Parameter()->SetVal( glm::vec2( 0.0f, 1.0f ), TimeType( 0.0 ) );
	point2Evaluator->Parameter()->SetVal( glm::vec2( 0.0f, 0.0f ), TimeType( 0.0 ) );

    return model;
}

// *******************************
//
bool                   DefaultGradientPluginDesc::CanBeAttachedTo     ( IPluginConstPtr plugin ) const
{
    if ( plugin == nullptr )
    {
        return false;
    }

    //auto  vac = plugin->GetVertexAttributesChannel();
    //if ( vac == nullptr )
    //{
    //    return false;
    //}

    //auto numChannels = vac->GetDescriptor()->GetNumVertexChannels();
    //if ( numChannels != 1 ) //only vertex attribute data allowed here
    //{
    //    return false;
    //}

 //   auto uid = plugin->GetTypeUid();

	//if ( uid != DefaultRectPluginDesc::UID() && uid != DefaultTextPluginDesc::UID() && uid != DefaultTransformPluginDesc::UID() && uid != DefaultTimerPluginDesc::UID() && uid != DefaultPrismPluginDesc::UID() && uid != DefaultPieChartPluginDesc::UID() )
	//{
	//	assert( false && "not one of my favourite plugins you are" );
 //       return false;
 //   }

    return true;
}

// *******************************
//
std::string             DefaultGradientPluginDesc::UID                       ()
{
    return "DEFAULT_LINEAR_GRADIENT";
}


std::string             DefaultGradientPluginDesc::TextureName               ()
{
    return "Grad0";
}

// ************************************************************************* PLUGIN *************************************************************************

// *************************************
// 
void					DefaultGradientPlugin::SetPrevPlugin				( IPluginPtr prev )
{
    BasePlugin::SetPrevPlugin( prev );

	////FIXME
	//if( prev && ( prev->GetTypeUid() == DefaultTextPluginDesc::UID() 
	//	|| prev->GetTypeUid() == DefaultTimerPluginDesc::UID() ) )
	//{
	//	m_texLocation = 1; //FIXME: ugly fixed location
	//}

    InitVertexAttributesChannel();
}

// *************************************
//
DefaultGradientPlugin::DefaultGradientPlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin< IPlugin >( name, uid, prev, std::static_pointer_cast< IPluginParamValModel >( model ) )
    , m_psc( nullptr )
    , m_vsc( nullptr )
    , m_vaChannel( nullptr )
    , m_paramValModel( model )
	, m_texLocation( 0 )
{
	m_psc = DefaultPixelShaderChannel::Create( model->GetPixelShaderChannelModel() );
	m_vsc = DefaultVertexShaderChannel::Create( model->GetVertexShaderChannelModel() );

	SetPrevPlugin( prev );

    InitVertexAttributesChannel();
}

// *************************************
// 
DefaultGradientPlugin::~DefaultGradientPlugin         ()
{
}

// *************************************
// 
IVertexAttributesChannelConstPtr    DefaultGradientPlugin::GetVertexAttributesChannel  () const
{
    return m_vaChannel;
}

// *************************************
// 
IPixelShaderChannelConstPtr         DefaultGradientPlugin::GetPixelShaderChannel       () const
{
    return m_psc;
}

// *************************************
// 
IVertexShaderChannelConstPtr        DefaultGradientPlugin::GetVertexShaderChannel      () const
{
    return m_vsc;
}

// *************************************
// 
void                                DefaultGradientPlugin::Update                      ( TimeType t )
{
    { t; } // FIXME: suppress unused warning
    m_paramValModel->Update();
	
	if( HelperVertexAttributesChannel::PropagateAttributesUpdate( m_vaChannel, m_prevPlugin ) )
	{
		RecalculateUVChannel();
	}

	if( HelperVertexAttributesChannel::PropagateTopologyUpdate( m_vaChannel, m_prevPlugin ) )
	{
		InitVertexAttributesChannel();
	}


	//bool hasPrevVAC = m_prevPlugin && m_prevPlugin->GetVertexAttributesChannel();
 //   
	//if( m_vaChannel )
	//{
	//	if( hasPrevVAC && m_prevPlugin->GetVertexAttributesChannel()->NeedsAttributesUpdate() )
	//	{
	//		RecalculateUVChannel();
	//		m_vaChannel->SetNeedsAttributesUpdate( true );
	//	}
	//	else
	//	{
	//		m_vaChannel->SetNeedsAttributesUpdate( false );
	//	}

	//	if( hasPrevVAC && m_prevPlugin->GetVertexAttributesChannel()->NeedsTopologyUpdate() ) //FIXME: additionalna hackierka
	//	{
	//		m_vaChannel->ClearAll();
	//		InitVertexAttributesChannel();	
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
void								DefaultGradientPlugin::InitVertexAttributesChannel	()
{
	if( !( m_prevPlugin && m_prevPlugin->GetVertexAttributesChannel() ) )
	{
		m_vaChannel = nullptr;
		return;
	}

	auto prevGeomChannel = m_prevPlugin->GetVertexAttributesChannel();
	auto prevCC = prevGeomChannel->GetComponents();
    
    //add gradient texture desc
	//FIXME: is it possible that CC is empty?
	auto vaChannelDesc = HelperVertexAttributesChannel::CreateVertexAttributesChannelDescriptor( prevCC[ 0 ]->GetAttributeChannels() );
	vaChannelDesc.AddAttrChannelDesc( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );

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

	auto desc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR ); // TOCHECK is that right?
    for( unsigned int i = 0; i < prevCC.size(); ++i )
    {
        auto connComp = ConnectedComponent::Create();
        auto prevConnComp = std::static_pointer_cast< const model::ConnectedComponent >( prevCC[ i ] );
        auto prevCompChannels = prevConnComp->GetAttributeChannelsPtr();

        for( auto prevCompCh : prevCompChannels )
        {
            connComp->AddAttributeChannel( prevCompCh );
        }

		//add gradient uv channel
		connComp->AddAttributeChannel( std::make_shared< Float2AttributeChannel >( desc, DefaultGradientPluginDesc::TextureName(), true ) );

        m_vaChannel->AddConnectedComponent( connComp );
    }

	RecalculateUVChannel();
}

// *************************************
//
void									DefaultGradientPlugin::RecalculateUVChannel         ()
{
	//FIXME: only one texture - convex hull calculations
    float minX = 100000.0f, minY = 100000.0f;
    float maxX = 0.0f, maxY = 0.0f;

	auto cc = m_vaChannel->GetComponents();
    for( unsigned int i = 0; i < cc.size(); ++i )
    {
        auto prevConnComp = std::static_pointer_cast< const model::ConnectedComponent >( cc[ i ] );
		auto posChannel = AttributeChannel::GetAttrChannel( prevConnComp->GetAttributeChannels(), AttributeSemantic::AS_POSITION );
		if( posChannel )
		{
			for( unsigned int j = 0; j < posChannel->GetNumEntries(); ++j )
			{
				const glm::vec3 * pos = reinterpret_cast< const glm::vec3 * >( posChannel->GetData() );
				minX = std::min( minX, pos[ j ].x );
				minY = std::min( minY, pos[ j ].y );
				maxX = std::max( maxX, pos[ j ].x );
				maxY = std::max( maxY, pos[ j ].y );
			}
		}
	}

	for( unsigned int i = 0; i < cc.size(); ++i )
    {
		auto compChannels = cc[ i ]->GetAttributeChannels();
		auto posChannel = AttributeChannel::GetAttrChannel( compChannels, AttributeSemantic::AS_POSITION );
		auto uvChannel = AttributeChannel::GetAttrChannel( compChannels, AttributeSemantic::AS_TEXCOORD, m_texLocation );

		if( posChannel && uvChannel )
		{
			auto pos = std::static_pointer_cast< Float3AttributeChannel >( posChannel );
			auto uvs = std::static_pointer_cast< Float2AttributeChannel >( uvChannel );
			
			auto & uvVerts = uvs->GetVertices();
			if( uvVerts.size() < posChannel->GetNumEntries() )
			{
				uvVerts.resize( posChannel->GetNumEntries() );
			}

			auto & posVerts = pos->GetVertices();
			for( unsigned int j = 0; j < posChannel->GetNumEntries(); ++j )
			{
				uvVerts[ j ] = glm::vec2( ( posVerts[ j ].x - minX ) / ( maxX - minX ), 
					( posVerts[ j ].y - minY ) / ( maxY - minY ) );
			}
		}
    }
}

} // model
} // bv
