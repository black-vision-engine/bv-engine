#include "DefaultPrismPlugin.h"

#include "Engine/Models/Plugins/Channels/Geometry/Simple/PrismComponent.h"

#include "Engine/Models/Plugins/Channels/ChannelsFactory.h"

#include <tuple>

namespace bv { namespace model {

// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultPrismPluginDesc::DefaultPrismPluginDesc                                ()
    : BasePluginDescriptor( UID(), "prism" )
{
}

// *******************************
//
bool                            DefaultPrismPluginDesc::CanBeAttachedTo      ( IPluginConstPtr plugin )  const
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
IPluginPtr                      DefaultPrismPluginDesc::CreatePlugin         ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultPrismPlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   DefaultPrismPluginDesc::CreateDefaultModel   ( ITimeEvaluatorPtr timeEvaluator ) const
{
    DefaultPluginParamValModelPtr   model       = std::make_shared< DefaultPluginParamValModel >();
    DefaultParamValModelPtr         vacModel    = std::make_shared< DefaultParamValModel >();

    ParamFloatPtr paramN             = ParametersFactory::CreateParameterFloat( "n", timeEvaluator );

    vacModel->AddParameter( paramN );

    model->SetVertexAttributesChannelModel( vacModel );

    //Set default parameters
    paramN->SetVal( 4.f, 0.f );

    return model;
}

// *******************************
//
std::string                     DefaultPrismPluginDesc::UID                  ()
{
    return "DEFAULT_PRISM";
}


// ************************************************************************* PLUGIN *************************************************************************

// *************************************
//
DefaultPrismPlugin::DefaultPrismPlugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model )
	: BasePlugin< IPlugin >( name, DefaultPrismPluginDesc::UID(), prev, model )
{
	auto nParam = m_pluginParamValModel->GetVertexAttributesChannelModel()->GetParameter( "n" );

	assert( nParam );

	float nVal = QueryTypedParam< ParamFloatPtr > ( nParam )->Evaluate();

	InitGeometry( int( nVal ) );
}


DefaultPrismPlugin::~DefaultPrismPlugin(void)
{
}

IVertexAttributesChannelConstPtr    DefaultPrismPlugin::GetVertexAttributesChannel  () const
{
    return m_vaChannel;
}

void DefaultPrismPlugin::InitGeometry( int n )
{
	auto prism = PrismComponent::Create( n );
	auto prism_main = std::get<0>(prism);
	auto prism_up = std::get<1>(prism);
	auto prism_down = std::get<2>(prism);
	
	if( m_vaChannel == NULL )
		m_vaChannel = ChannelsFactory::CreateVertexAttributesChannel( prism_main, false );
	else
	{
		m_vaChannel->ClearAll();
		m_vaChannel->AddConnectedComponent( prism_main );
	}

	m_vaChannel->AddConnectedComponent( prism_up );
	m_vaChannel->AddConnectedComponent( prism_down );

	m_lastN = n;
}

// *************************************
//
void                                DefaultPrismPlugin::Update                      ( TimeType t )
{
	m_pluginParamValModel->Update();

	// just for fun
	int n = int(t)+3;

	if( n != m_lastN )
	{
		InitGeometry( n );
		m_vaChannel->SetNeedsTopologyUpdate( true );
		//m_vaChannel->SetNeedsAttributesUpdate( true );
	}
	else
	{
		m_vaChannel->SetNeedsAttributesUpdate( false );
	}
}

} }