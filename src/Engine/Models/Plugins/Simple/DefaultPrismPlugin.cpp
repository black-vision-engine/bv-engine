#include "DefaultPrismPlugin.h"

#include "Engine/Models/Plugins/Channels/Geometry/Simple/PrismComponent.h"

#include "Engine/Models/Plugins/Channels/ChannelsFactory.h"

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

    ParamFloatPtr paramWidth             = ParametersFactory::CreateParameterFloat( "width", timeEvaluator );
    ParamFloatPtr paramHeight            = ParametersFactory::CreateParameterFloat( "height", timeEvaluator );

    vacModel->AddParameter( paramWidth );
    vacModel->AddParameter( paramHeight );

    model->SetVertexAttributesChannelModel( vacModel );

    //Set default parameters
    paramWidth->SetVal( 1.f, 0.f );
    paramHeight->SetVal( 1.f, 0.f );

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
	auto prism = PrismComponent::Create( 40 );
	auto prism_main = prism.first;
	auto prism_up = prism.second;
	auto channel = ChannelsFactory::CreateVertexAttributesChannel( prism_main, false );
	channel->AddConnectedComponent( prism_up );
	channel = ChannelsFactory::CreateVertexAttributesChannel( prism_up, false );
	m_vaChannel = channel;
}


DefaultPrismPlugin::~DefaultPrismPlugin(void)
{
}

IVertexAttributesChannelConstPtr    DefaultPrismPlugin::GetVertexAttributesChannel  () const
{
    return m_vaChannel;
}

// *************************************
//
void                                DefaultPrismPlugin::Update                      ( TimeType t )
{
}

} }