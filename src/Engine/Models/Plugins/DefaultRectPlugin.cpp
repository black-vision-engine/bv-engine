#include "DefaultRectPlugin.h"

#include "Engine/Models/Plugins/PluginsFactory.h"
#include "Engine/Models/Plugins/Channels/ChannelsFactory.h"
#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"
#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"
#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"


namespace bv { namespace model {

// ***************************** PLUGIN ********************************** 

// *************************************
//
DefaultRectPlugin::DefaultRectPlugin    ( const IPlugin * prev, DefaultPluginParamValModelPtr model )
    : BasePlugin( prev,  std::static_pointer_cast< IPluginParamValModel >( model ) )
    , m_vaChannel( nullptr )
    , m_paramValModel( model )
    , m_widthParam( nullptr )
    , m_heightParam( nullptr )
{
    auto params = model->GetPluginModel()->GetParameters();

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


// ********************************************************* DESCRIPTOR *********************************************************

// *************************************
//
DefaultPluginParamValModel * DefaultRectPluginDesc::CreateModel ( bool setDefaultValues )
{
    DefaultPluginParamValModel * model          = new DefaultPluginParamValModel();
    DefaultParamValModel * pluginModel          = new DefaultParamValModel();

    ParamFloat * paramWidth                     = ParametersFactory::CreateParameterFloat( "width" );
    ParamFloat * paramHeight                    = ParametersFactory::CreateParameterFloat( "height" );

    pluginModel->AddParameter( paramWidth );
    pluginModel->AddParameter( paramHeight );

    model->SetTransformChannelModel( pluginModel );

    if ( setDefaultValues )
    {
        paramWidth->SetVal( 0.f, 1.f );
        paramHeight->SetVal( 0.f, 1.f );
    }

    return model;
}

// *************************************
//
DefaultRectPlugin *          DefaultRectPluginDesc::CreatePlugin( const IPlugin * prev, bool setDefaultValues )
{
    return new DefaultRectPlugin( prev, DefaultPluginParamValModelPtr( DefaultRectPluginDesc::CreateModel( setDefaultValues ) ) );    
}

} // model
} // bv
