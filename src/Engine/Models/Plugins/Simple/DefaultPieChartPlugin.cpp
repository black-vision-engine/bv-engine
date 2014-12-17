#include "DefaultPieChartPlugin.h"

#include "Engine/Models/Plugins/Channels/Geometry/Simple/VertexAttributesChannelVariableTopology.h"

#include "Mathematics/defines.h"

namespace bv { namespace model {

	class DefaultVariableTopologyStripComponent : public VariableTopologyStripComponent
	{
	public:
		DefaultVariableTopologyStripComponent( float angleStart, float angleEnd, float dangle )
			: VariableTopologyStripComponent( 1, 1, 1, 1, 1, 1, 1, 1, 1 ) 
		{
			const AttributeChannelDescriptor * desc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );
			Float3AttributeChannelPtr vertArrtF3 = std::make_shared< Float3AttributeChannel >( desc, desc->SuggestedDefaultName( 0 ), false );

			float angle = angleStart;
			//double dangle = 2*PI / fragmentsNum;

			//int size = sizeof( percents ) / sizeof( *percents );

			//for( int p = 0; p < size; p++ )
			//{
				for( ; angle < angleEnd; angle += dangle )
				{
					vertArrtF3->AddAttribute( glm::vec3( cos( angle ), 0.5, sin( angle ) ) );

					if( vertArrtF3->GetNumEntries()%2 == 1 )
					{
						vertArrtF3->AddAttribute( glm::vec3( 0, 0.5, 0 ) );
					}
				}
				angle = angleEnd;
			//}

			//for( int i = 0; i <= fragmentsNum; i++ )
			//{
			//	double angle = 2*PI * i / fragmentsNum;

			//	vertArrtF3->AddAttribute( glm::vec3( cos( angle ), 0.5, sin( angle ) ) );

			//	if( i%2 == 1 )
			//	{
			//		vertArrtF3->AddAttribute( glm::vec3( 0, 0.5, 0 ) );
			//	}
			//}


			AddAttributeChannel( AttributeChannelPtr( vertArrtF3 ) );
		}
	};

	class DefaultVertexAttributesChannelVariableTopology : public VertexAttributesChannelVariableTopology
	{
	public:
		DefaultVertexAttributesChannelVariableTopology( std::vector< float > percents, int fragmentsNum ) : VertexAttributesChannelVariableTopology( 0, 1, 0, 1, 1 ) 
		{
			std::vector< DefaultVariableTopologyStripComponent* > comps;
			//auto connComp = std::make_shared<DefaultVariableTopologyStripComponent>( percents, 100 );

			float angle = 0;
			double dangle = 2*PI / fragmentsNum;

			for( float percent : percents )
			{
				float angleEnd = angle + 2*PI*percent/100;

				//comps.push_back( std::make_shared< DefaultVariableTopologyStripComponent >( angle, angle+percent, dangle ) );
				comps.push_back( new DefaultVariableTopologyStripComponent( angle, angleEnd, dangle ) );

				angle = angleEnd;
			}

			auto connComp = comps[0];

			VertexAttributesChannelDescriptor desc;

			for( auto compDesc : connComp->GetAttributeChannels() )
			{
				desc.AddAttrChannelDesc( static_cast< const AttributeChannelDescriptor * >( compDesc->GetDescriptor() ) );
			}

			//VertexAttributesChannelPtr ret = std::make_shared< model::VertexAttributesChannel >( PrimitiveType::PT_TRIANGLE_STRIP, desc, false, false );
			m_desc = desc;
			m_primitiveType = PrimitiveType::PT_TRIANGLE_STRIP;
			m_isReadOnly = false;
			m_isTimeInvariant = false;

			for( auto comp : comps )
				AddVTConnectedComponent( VariableTopologyStripComponentPtr( (VariableTopologyStripComponent*) comp ) );
		}
	};

// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultPieChartPluginDesc::DefaultPieChartPluginDesc                                ()
    : BasePluginDescriptor( UID(), "piechart" )
{
}

// *******************************
//
bool                            DefaultPieChartPluginDesc::CanBeAttachedTo      ( IPluginConstPtr plugin )  const
{
    if( !BasePluginDescriptor::CanBeAttachedTo( plugin ) )
    {
        return false;
    }

    //Geometry generator cannot be attached to a plugin which generates geometry itself
    if( plugin && plugin->GetVertexAttributesChannel() )
    {
        return false;
    }

    return true;
}

// *******************************
//
IPluginPtr                      DefaultPieChartPluginDesc::CreatePlugin         ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultPieChartPlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   DefaultPieChartPluginDesc::CreateDefaultModel   ( ITimeEvaluatorPtr timeEvaluator ) const
{
    DefaultPluginParamValModelPtr   model       = std::make_shared< DefaultPluginParamValModel >();
    DefaultParamValModelPtr         vacModel    = std::make_shared< DefaultParamValModel >();

    ParamFloatPtr paramN             = ParametersFactory::CreateParameterFloat( "n", timeEvaluator );

    vacModel->AddParameter( paramN );

    model->SetVertexAttributesChannelModel( vacModel );

    //Set default parameters
    paramN->SetVal( 5.f, 0.f ); // FIXME: this is f*)*(&ing ridiculous!

    return model;
}

// *******************************
//
std::string                     DefaultPieChartPluginDesc::UID                  ()
{
    return "DEFAULT_PIECHART";
}


// ************************************************************************* PLUGIN *************************************************************************

// *************************************
//
DefaultPieChartPlugin::DefaultPieChartPlugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model )
	: BasePlugin< IPlugin >( name, DefaultPieChartPluginDesc::UID(), prev, model )
{
	auto nParam = m_pluginParamValModel->GetVertexAttributesChannelModel()->GetParameter( "n" );

	assert( nParam );

	float nVal = QueryTypedParam< ParamFloatPtr > ( nParam )->Evaluate();

	InitGeometry( int( nVal ) );
}

DefaultPieChartPlugin::~DefaultPieChartPlugin(void)
{
}

IVertexAttributesChannelConstPtr    DefaultPieChartPlugin::GetVertexAttributesChannel  () const
{
    return m_vaChannel;
}

void DefaultPieChartPlugin::InitGeometry( int n )
{
	float percents[] = { 10.f, 20.f, 50.f };
	m_vaChannel = std::make_shared< DefaultVertexAttributesChannelVariableTopology > ( std::vector< float > ( percents, percents+3 ), 100 );
//	auto prism = PieChartComponent::Create( n );
//	auto prism_main = std::get<0>(prism);
//	auto prism_up = std::get<1>(prism);
//	auto prism_down = std::get<2>(prism);
//
//// FIXME: it would be more pretty to create empty VertexAttributesChannel in ctor and add a proper descriptor there
//	if( m_vaChannel == NULL )
//		m_vaChannel = ChannelsFactory::CreateVertexAttributesChannel( prism_main, false );
//	else
//	{
//		m_vaChannel->ClearAll();
//		m_vaChannel->AddConnectedComponent( prism_main );
//	}
//
//	m_vaChannel->AddConnectedComponent( prism_up );
//	m_vaChannel->AddConnectedComponent( prism_down );
//
//	m_lastN = n;
}

// *************************************
//
void                                DefaultPieChartPlugin::Update                      ( TimeType t )
{
	m_pluginParamValModel->Update();

	auto nParam = m_pluginParamValModel->GetVertexAttributesChannelModel()->GetParameter( "n" );
	assert( nParam );
	float nVal = QueryTypedParam< ParamFloatPtr > ( nParam )->Evaluate();
	int n = int( nVal );

	if( n != m_lastN )
	{
		InitGeometry( n );
		m_vaChannel->SetNeedsTopologyUpdate( true );
		m_lastN = n;
	}
	//else
	//{
	//	m_vaChannel->SetNeedsAttributesUpdate( false );
	//}
}


} }