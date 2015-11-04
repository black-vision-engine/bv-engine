#include "DefaultPrismPlugin.h"

#include "Engine/Models/Plugins/Channels/Geometry/Simple/PrismComponent.h"

#include "Engine/Models/Plugins/Channels/ChannelsFactory.h"
#include "Engine/Models/Plugins/Channels/Geometry/HelperVertexAttributesChannel.h"

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

    ParamIntPtr paramN             = ParametersFactory::CreateParameterInt( "n", timeEvaluator );
    ParamFloatPtr paramType          = ParametersFactory::CreateParameterFloat( "uv_type", timeEvaluator );

    vacModel->AddParameter( paramN );
    vacModel->AddParameter( paramType );

    model->SetVertexAttributesChannelModel( vacModel );

    //Set default parameters
    paramN->SetVal( 3, 0.f );
    paramType->SetVal( PrismComponent::PrismUVType::TEXTURED, 0.f );

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
PrismComponent::PrismUVType DefaultPrismPlugin::GetUVType()
{
    assert( m_uvTypeParam );
    auto t = m_uvTypeParam->Evaluate();

    return PrismComponent::PrismUVType( int( t ) ); // ;)
}

// *************************************
//
int DefaultPrismPlugin::GetN()
{
    assert( m_nParam );
    return int( m_nParam->Evaluate() );
}

// *************************************
//
DefaultPrismPlugin::DefaultPrismPlugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model )
    : BasePlugin< IPlugin >( name, uid, prev, model )
{
    m_uvTypeParam = QueryTypedParam< ParamFloatPtr >( m_pluginParamValModel->GetVertexAttributesChannelModel()->GetParameter( "uv_type" ) );
    assert( m_uvTypeParam );
    m_uvType = GetUVType();

    m_nParam = QueryTypedParam< ParamIntPtr >( m_pluginParamValModel->GetVertexAttributesChannelModel()->GetParameter( "n" ) );
    assert( m_nParam );
    m_lastN = GetN();

    InitGeometry( m_lastN, m_uvType );
}


DefaultPrismPlugin::~DefaultPrismPlugin(void)
{
}

IVertexAttributesChannelConstPtr    DefaultPrismPlugin::GetVertexAttributesChannel  () const
{
    return m_vaChannel;
}

void DefaultPrismPlugin::InitGeometry( int n, PrismComponent::PrismUVType t )
{
    auto prism = PrismComponent::Create( n, t );

    auto prism1 = std::get<0>(prism);
    auto prism2 = std::get<1>(prism);
    auto prism3 = std::get<2>(prism);

// FIXME: it would be more pretty to create empty VertexAttributesChannel in ctor and add a proper descriptor there
    if( m_vaChannel == NULL )
        m_vaChannel = ChannelsFactory::CreateVertexAttributesChannel( prism1, false );
    else
    {
        m_vaChannel->ClearAll();
        m_vaChannel->AddConnectedComponent( prism1 );
    }
	HelperVertexAttributesChannel::SetTopologyUpdate( m_vaChannel );

    m_vaChannel->AddConnectedComponent( prism2 );
    m_vaChannel->AddConnectedComponent( prism3 );

    m_lastN = n;
}

// *************************************
//
void                                DefaultPrismPlugin::Update                      ( TimeType t )
{
    { t; } // FIXME: suppress unused warning
    m_pluginParamValModel->Update();

    auto nParam = m_pluginParamValModel->GetVertexAttributesChannelModel()->GetParameter( "n" );
    assert( nParam );
    int n = m_nParam->Evaluate();

    auto uvType = GetUVType();

    if( n != m_lastN || uvType != m_uvType )
    {
        InitGeometry( n, uvType );

		//HelperVertexAttributesChannel::SetTopologyUpdate( m_vaChannel );
        //m_vaChannel->SetNeedsTopologyUpdate( true );

        m_lastN = n;
        m_uvType = uvType;
    }
    //else
    //{
    //	m_vaChannel->SetNeedsAttributesUpdate( false );
    //}
}

} }