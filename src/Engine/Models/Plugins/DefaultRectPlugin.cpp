#include "DefaultRectPlugin.h"

#include "Engine/Models/Plugins/PluginsFactory.h"
#include "Engine/Models/Plugins/Channels/ChannelsFactory.h"
#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"
#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"
#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"


namespace bv { namespace model {


// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultRectPluginDesc::DefaultRectPluginDesc                                ()
    : BasePluginDescriptor( UID() )
{
}

// *******************************
//
bool                            DefaultRectPluginDesc::CanBeAttachedTo      ( const IPlugin * plugin )  const
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
IPlugin *                       DefaultRectPluginDesc::CreatePlugin         ( const std::string & name, const IPlugin * prev ) const
{
    return CreatePluginTyped< DefaultRectPlugin >( name, prev );
}

// *******************************
//
DefaultPluginParamValModel *    DefaultRectPluginDesc::CreateDefaultModel   () const
{
    DefaultPluginParamValModel * model  = new DefaultPluginParamValModel();
    DefaultParamValModel * vacModel     = new DefaultParamValModel();

    ParamFloat * paramWidth             = ParametersFactory::CreateParameterFloat( "width" );
    ParamFloat * paramHeight            = ParametersFactory::CreateParameterFloat( "height" );

    vacModel->AddParameter( paramWidth );
    vacModel->AddParameter( paramHeight );

    model->SetVertexAttributesChannelModel( vacModel );

    //Set default parameters
    paramWidth->SetVal( 0.f, 1.f );
    paramHeight->SetVal( 0.f, 1.f );

    return model;
}

// *******************************
//
std::string                     DefaultRectPluginDesc::UID                  ()
{
    return "DEFAULT_RECTANGLE";
}


// ************************************************************************* PLUGIN *************************************************************************

// *************************************
//
DefaultRectPlugin::DefaultRectPlugin    ( const std::string & name, const std::string & uid, const IPlugin * prev, DefaultPluginParamValModelPtr model )
    : BasePlugin( name, uid, prev, std::static_pointer_cast< IPluginParamValModel >( model ) )
    , m_vaChannel( nullptr )
    , m_paramValModel( model )
    , m_widthParam( nullptr )
    , m_heightParam( nullptr )
{
    auto params = VertexAttributesChannelModel()->GetParameters();

    assert( params.size() == 2 );

    m_widthParam    = QueryTypedParam< ParamFloat >( params[ 0 ] );
    m_heightParam   = QueryTypedParam< ParamFloat >( params[ 1 ] );

    m_lastW = m_widthParam->Evaluate( 0.f );
    m_lastH = m_heightParam->Evaluate( 0.f );

    RectComponent * rect = RectComponent::Create( m_lastW, m_lastH );
    m_vaChannel = ChannelsFactory::CreateVertexAttributesChannel( rect );

    m_rct = rect;
}

// *************************************
//
DefaultRectPlugin::~DefaultRectPlugin   ()
{
    delete m_vaChannel;
}

// *************************************
//
const IVertexAttributesChannel *    DefaultRectPlugin::GetVertexAttributesChannel  () const
{
    return m_vaChannel;
}

// *************************************
//
void                                DefaultRectPlugin::Update                      ( TimeType t )
{
    //FIXME: reimplement va channel (no time, no explicit update and so on)
    m_paramValModel->Update( t );

    //This code has to be executed in a plugin as only plugin knows how to translate its state to geometry representation
    float w = m_widthParam->Evaluate( t );
    float h = m_heightParam->Evaluate( t );

    if( ( fabs( m_lastW - w ) + fabs( m_lastH - h ) ) > 0.001f )
    {
        m_rct->SetRectSize( w, h );
        m_vaChannel->SetNeedsAttributesUpdate( true );
    }

    m_lastW = w;
    m_lastH = h;
}

} // model
} // bv
