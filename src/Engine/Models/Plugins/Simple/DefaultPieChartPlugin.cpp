#include "DefaultPieChartPlugin.h"

#include "Engine/Models/Plugins/Channels/Geometry/Simple/VertexAttributesChannelVariableTopology.h"

#include "Mathematics/defines.h"

namespace bv { namespace model {

	class DefaultVertexAttributeChannel : public VertexAttributesChannel
	{
		AttributeChannelDescriptor * m_compDesc;

	public:
		DefaultVertexAttributeChannel( PrimitiveType type, bool isReadOnly = false, bool isTimeInvariant = false ) 
			: VertexAttributesChannel ( type, isReadOnly, isTimeInvariant )  
		{
			AttributeChannelDescriptor * compDesc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );

			VertexAttributesChannelDescriptor vaDesc;

			vaDesc.AddAttrChannelDesc( static_cast< const AttributeChannelDescriptor * >( compDesc ) );

			m_compDesc = compDesc;

			m_desc = vaDesc;
		}

		void GenerateAndAddConnectedComponent( void (*GenerateConnectedComponent) (Float3AttributeChannelPtr) )
		{
			ConnectedComponentPtr comp = ConnectedComponent::Create();

			Float3AttributeChannelPtr vertArrtF3 = std::make_shared< Float3AttributeChannel >( m_compDesc, m_compDesc->SuggestedDefaultName( 0 ), false );

			GenerateConnectedComponent( vertArrtF3 );

			comp->AddAttributeChannel( vertArrtF3 );

			AddConnectedComponent( comp );
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

double angleStart, angleEnd, dangle;

void GenerateSomething( Float3AttributeChannelPtr verts )
{
				double angle = angleStart;

				for( ; angle < angleEnd; angle += dangle )
				{
					verts->AddAttribute( glm::vec3( cos( angle ), 0.5, sin( angle ) ) );

					if( verts->GetNumEntries()%2 == 1 )
					{
						verts->AddAttribute( glm::vec3( 0, 0.5, 0 ) );
					}
				}
				angle = angleEnd;
}

void DefaultPieChartPlugin::InitGeometry( int n )
{
	float percents[] = { 10.f, 20.f, 50.f };

	auto channel = std::make_shared< DefaultVertexAttributeChannel >( PrimitiveType::PT_TRIANGLE_STRIP );

	angleStart = PI/4; angleEnd = 3*PI/4; dangle = PI/100;
	channel->GenerateAndAddConnectedComponent( GenerateSomething );

	m_vaChannel = channel;
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
}

} }