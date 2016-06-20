#include "stdafx.h"

#include "DefaultGradientPlugin.h"

#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"
#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"
#include "Engine/Models/Plugins/Channels/Geometry/HelperVertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"

#include "Engine/Models/Plugins/Simple/TextPlugins/DefaultTextPlugin.h"
#include "Engine/Models/Plugins/Simple/TextPlugins/DefaultTimerPlugin.h"

namespace bv { namespace model {

const std::string        DefaultGradientPlugin::PARAM_BLEND_ENABLE  = "blend enable";
const std::string        DefaultGradientPlugin::PARAM_ALPHA         = "alpha";

const std::string        DefaultGradientPlugin::PARAM_POINT1        = "point1";
const std::string        DefaultGradientPlugin::PARAM_POINT2        = "point2";
const std::string        DefaultGradientPlugin::PARAM_COLOR1        = "color1";
const std::string        DefaultGradientPlugin::PARAM_COLOR2        = "color2";


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
    ModelHelper helper( timeEvaluator );

    //Create all models
    auto model  = helper.GetModel();
    DefaultParamValModelPtr vsModel      = std::make_shared< DefaultParamValModel >();

    //Create all parameters and evaluators
    SimpleTransformEvaluatorPtr trTxEvaluator    = ParamValEvaluatorFactory::CreateSimpleTransformEvaluator( "txMat", timeEvaluator );
    
    helper.CreatePluginModel();
    helper.AddSimpleParam( DefaultGradientPlugin::PARAM_BLEND_ENABLE, true, true, true );

    helper.CreatePSModel();
    helper.AddSimpleParam( DefaultGradientPlugin::PARAM_ALPHA, 1.f, true );

    helper.AddSimpleParam( DefaultGradientPlugin::PARAM_POINT1, glm::vec2( 0.0f, 1.0f ), true );
    helper.AddSimpleParam( DefaultGradientPlugin::PARAM_POINT2, glm::vec2( 0.0f, 0.0f ), true );
    helper.AddSimpleParam( DefaultGradientPlugin::PARAM_COLOR1, glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f ), true );
    helper.AddSimpleParam( DefaultGradientPlugin::PARAM_COLOR2, glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f ), true );

    //Register all parameters and evaloators in models
    vsModel->RegisterAll( trTxEvaluator );

    //Set models structure
    model->SetVertexShaderChannelModel( vsModel );

    //Set default values of all parameters
    trTxEvaluator->Parameter()->Transform().InitializeDefaultSRT();


    return model;
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

    InitVertexAttributesChannel();

	HelperPixelShaderChannel::CloneRenderContext( m_psc, prev );
    m_psc->GetRendererContext()->cullCtx->enabled = false;
}

// *************************************
//
DefaultGradientPlugin::DefaultGradientPlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin< IPlugin >( name, uid, prev, model )
    , m_psc( nullptr )
    , m_vsc( nullptr )
    , m_vaChannel( nullptr )
{
	m_psc = DefaultPixelShaderChannel::Create( model->GetPixelShaderChannelModel() );
	m_vsc = DefaultVertexShaderChannel::Create( model->GetVertexShaderChannelModel() );

	SetPrevPlugin( prev );
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
IPixelShaderChannelPtr              DefaultGradientPlugin::GetPixelShaderChannel       () const
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
	BasePlugin::Update( t );

    if( ParameterChanged( PARAM_BLEND_ENABLE ) )
    {
        auto ctx = m_psc->GetRendererContext();
        ctx->alphaCtx->blendEnabled = std::static_pointer_cast<ParamBool>( GetParameter( PARAM_BLEND_ENABLE ) )->Evaluate();

        HelperPixelShaderChannel::SetRendererContextUpdate( m_psc );
    }
	
	if( HelperVertexAttributesChannel::PropagateAttributesUpdate( m_vaChannel, m_prevPlugin ) )
	{
		RecalculateUVChannel();
	}

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
void								DefaultGradientPlugin::InitVertexAttributesChannel	()
{
	if( !( m_prevPlugin && m_prevPlugin->GetVertexAttributesChannel() ) )
	{
		m_vaChannel = nullptr;
		return;
	}

	auto prevGeomChannel = m_prevPlugin->GetVertexAttributesChannel();
	
    //add gradient texture desc
	VertexAttributesChannelDescriptor vaChannelDesc( * static_cast< const VertexAttributesChannelDescriptor * >( prevGeomChannel->GetDescriptor() ) );
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

	auto desc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR ); // TOCHECK is that right?
	auto prevCC = prevGeomChannel->GetComponents();
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
		auto posChannel = prevConnComp->GetAttrChannel( AttributeSemantic::AS_POSITION );
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
        auto prevConnComp = std::static_pointer_cast< const model::ConnectedComponent >( cc[ i ] );
		auto posChannel = prevConnComp->GetAttrChannel( AttributeSemantic::AS_POSITION );
		auto uvChannel = prevConnComp->GetAttrChannel( AttributeSemantic::AS_TEXCOORD, -1 );

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
