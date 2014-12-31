#include "DefaultPieChartPlugin.h"

#include "Engine/Models/Plugins/Channels/Geometry/Simple/VertexAttributesChannelVariableTopology.h"

#include "Mathematics/defines.h"

namespace bv { namespace model {

	class DefaultGeometryVertexAttributeChannel : public VertexAttributesChannel
	{
		AttributeChannelDescriptor * m_compDesc;

	public:
		DefaultGeometryVertexAttributeChannel( PrimitiveType type, bool isReadOnly = false, bool isTimeInvariant = false ) 
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

	class DefaultGeometryAndUVsVertexAttributeChannel : public VertexAttributesChannel
	{
		AttributeChannelDescriptor * m_compVertDesc;
		AttributeChannelDescriptor * m_compUVDesc;

	public:
		DefaultGeometryAndUVsVertexAttributeChannel( PrimitiveType type, bool isReadOnly = false, bool isTimeInvariant = false ) 
			: VertexAttributesChannel ( type, isReadOnly, isTimeInvariant )  
		{
			VertexAttributesChannelDescriptor vaDesc;

			AttributeChannelDescriptor * compVertDesc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );
			vaDesc.AddAttrChannelDesc( static_cast< const AttributeChannelDescriptor * >( compVertDesc ) );

			AttributeChannelDescriptor * compUVDesc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_GENERATOR );
			vaDesc.AddAttrChannelDesc( static_cast< const AttributeChannelDescriptor * >( compUVDesc ) );

			m_compVertDesc = compVertDesc;
			m_compUVDesc = compUVDesc;

			m_desc = vaDesc;
		}

		void GenerateAndAddConnectedComponent( void (*GenerateConnectedComponent) (Float3AttributeChannelPtr, Float2AttributeChannelPtr) )
		{
			ConnectedComponentPtr comp = ConnectedComponent::Create();

			Float3AttributeChannelPtr vertArrtF3 = std::make_shared< Float3AttributeChannel >( m_compVertDesc, m_compVertDesc->SuggestedDefaultName( 0 ), false );
			Float2AttributeChannelPtr vertArrtUV = std::make_shared< Float2AttributeChannel >( m_compUVDesc, m_compUVDesc->SuggestedDefaultName( 0 ), false );

			GenerateConnectedComponent( vertArrtF3, vertArrtUV );

			comp->AddAttributeChannel( vertArrtF3 );
			comp->AddAttributeChannel( vertArrtUV );

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

	//ParamFloatPtr paramN             = ParametersFactory::CreateParameterFloat( "n", timeEvaluator );
	ParamFloatPtr paramAngleStart	 = ParametersFactory::CreateParameterFloat( "angleStart", timeEvaluator );
	ParamFloatPtr paramAngleEnd		 = ParametersFactory::CreateParameterFloat( "angleEnd", timeEvaluator );

	//vacModel->AddParameter( paramN );
	vacModel->AddParameter( paramAngleStart );
	vacModel->AddParameter( paramAngleEnd );

	model->SetVertexAttributesChannelModel( vacModel );

	//Set default parameters
	//paramN->SetVal( 5.f, 0.f ); // FIXME: this is f*)*(&ing ridiculous!
	paramAngleStart->SetVal( float( PI/4 ), 0 );
	paramAngleEnd->SetVal( float( 3*PI/4 ), 0 );

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
	auto asParam = m_pluginParamValModel->GetVertexAttributesChannelModel()->GetParameter( "angleStart" );
	assert( asParam );
	float asVal = QueryTypedParam< ParamFloatPtr > ( asParam )->Evaluate();

	auto aeParam = m_pluginParamValModel->GetVertexAttributesChannelModel()->GetParameter( "angleEnd" );
	assert( aeParam );
	float aeVal = QueryTypedParam< ParamFloatPtr > ( aeParam )->Evaluate();

	InitGeometry( asVal, aeVal );
}

DefaultPieChartPlugin::~DefaultPieChartPlugin(void)
{
}

IVertexAttributesChannelConstPtr    DefaultPieChartPlugin::GetVertexAttributesChannel  () const
{
	return m_vaChannel;
}

// geometry generators

double angleStart, angleEnd, dangle, z;

void GenerateBase( Float3AttributeChannelPtr verts )
{
	double angle = angleStart;

	for( ; angle < angleEnd; angle += dangle )
	{
		verts->AddAttribute( glm::vec3( cos( angle ), z, sin( angle ) ) );

		if( verts->GetNumEntries()%2 == 1 )
		{
			verts->AddAttribute( glm::vec3( 0, z, 0 ) );
		}
	}
	angle = angleEnd;
}

double angle, z1, z2;

void GenerateSide( Float3AttributeChannelPtr verts )
{
	verts->AddAttribute( glm::vec3( cos( angle ), z1, sin( angle ) ) );
	verts->AddAttribute( glm::vec3( cos( angle ), z2, sin( angle ) ) );
	verts->AddAttribute( glm::vec3( 0, z1, 0 ) );
	verts->AddAttribute( glm::vec3( 0, z2, 0 ) );
}

// geometry&uv generators

void GenerateBaseUV( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs )
{
	double angle = angleStart;

	for( ; angle < angleEnd; angle += dangle )
	{
		verts->AddAttribute( glm::vec3( cos( angle ), z, sin( angle ) ) );
		uvs->AddAttribute( glm::vec2( angle, z ) );

		if( verts->GetNumEntries()%2 == 1 )
		{
			verts->AddAttribute( glm::vec3( 0, z, 0 ) );
			uvs->AddAttribute( glm::vec2( 0, z ) );
		}
	}
	angle = angleEnd;
	verts->AddAttribute( glm::vec3( cos( angle ), z, sin( angle ) ) );
	uvs->AddAttribute( glm::vec2( angle, z ) );
}

void GenerateSideUV( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs )
{
	verts->AddAttribute( glm::vec3( cos( angle ), z1, sin( angle ) ) );
	verts->AddAttribute( glm::vec3( cos( angle ), z2, sin( angle ) ) );
	verts->AddAttribute( glm::vec3( 0, z1, 0 ) );
	verts->AddAttribute( glm::vec3( 0, z2, 0 ) );

	uvs->AddAttribute( glm::vec2( angle, z1 ) );
	uvs->AddAttribute( glm::vec2( angle, z2 ) );
	uvs->AddAttribute( glm::vec2( 0, z1 ) );
	uvs->AddAttribute( glm::vec2( 0, z2 ) );
}

void GenerateRoundSideUV( Float3AttributeChannelPtr verts, Float2AttributeChannelPtr uvs )
{
	double angle = angleStart;

	for( ; angle <= angleEnd; angle += dangle )
	{
		verts->AddAttribute( glm::vec3( cos( angle ), z1, sin( angle ) ) );
		uvs->AddAttribute( glm::vec2( angle, z1 ) );

		verts->AddAttribute( glm::vec3( cos( angle ), z2, sin( angle ) ) );
		uvs->AddAttribute( glm::vec2( angle, z2 ) );
	}
	angle = angleEnd;

	verts->AddAttribute( glm::vec3( cos( angle ), z1, sin( angle ) ) );
	uvs->AddAttribute( glm::vec2( angle, z1 ) );

	verts->AddAttribute( glm::vec3( cos( angle ), z2, sin( angle ) ) );
	uvs->AddAttribute( glm::vec2( angle, z2 ) );
}

void DefaultPieChartPlugin::InitGeometry( float angleStart_, float angleEnd_ )
{
	DefaultGeometryAndUVsVertexAttributeChannel* channel;
	if( m_vaChannel==NULL )
	{
		channel = new DefaultGeometryAndUVsVertexAttributeChannel( PrimitiveType::PT_TRIANGLE_STRIP );
		m_vaChannel = VertexAttributesChannelPtr( (VertexAttributesChannel*) channel );
	} else
	{
		channel = (DefaultGeometryAndUVsVertexAttributeChannel*) m_vaChannel.get();
		channel->ClearAll();
	}

	z1 = 0; z2 = 1; // FIXME: variable?

	angleStart = angleStart_;
	angleEnd = angleEnd_; 
	dangle = PI/120; // FIXME: variable? 
	
	z = z1;
	channel->GenerateAndAddConnectedComponent( GenerateBaseUV );

	z = z2;
	channel->GenerateAndAddConnectedComponent( GenerateBaseUV );
	
	angle = angleStart;
	channel->GenerateAndAddConnectedComponent( GenerateSideUV );

	angle = angleEnd;
	channel->GenerateAndAddConnectedComponent( GenerateSideUV );

	channel->GenerateAndAddConnectedComponent( GenerateRoundSideUV );
}

// *************************************
//
void                                DefaultPieChartPlugin::Update                      ( TimeType t )
{
	m_pluginParamValModel->Update();

	auto asParam = m_pluginParamValModel->GetVertexAttributesChannelModel()->GetParameter( "angleStart" );
	assert( asParam );
	float asVal = QueryTypedParam< ParamFloatPtr > ( asParam )->Evaluate();

	auto aeParam = m_pluginParamValModel->GetVertexAttributesChannelModel()->GetParameter( "angleEnd" );
	assert( aeParam );
	float aeVal = QueryTypedParam< ParamFloatPtr > ( aeParam )->Evaluate();

	if( asVal != m_angleStart || aeVal != m_angleEnd )
	{
		InitGeometry( asVal, aeVal );
		m_vaChannel->SetNeedsTopologyUpdate( true );
		m_angleStart = asVal; m_angleEnd = aeVal;
	}
}

} }