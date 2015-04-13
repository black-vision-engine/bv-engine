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
    : BasePluginDescriptor( UID(), "rectangle" )
{
}

// *******************************
//
bool                            DefaultRectPluginDesc::CanBeAttachedTo      ( IPluginConstPtr plugin )  const
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
IPluginPtr                      DefaultRectPluginDesc::CreatePlugin         ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultRectPlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   DefaultRectPluginDesc::CreateDefaultModel   ( ITimeEvaluatorPtr timeEvaluator ) const
{
   // START_MODEL( timeEvaluator )
   //     ADD_VAC_PARAM( "width", 1.f );
   //     ADD_VAC_PARAM( "height", 1.f );
   //     ADD_VAC_PARAM_VAL( "param0", glm::vec2( .4f, 0.f ) );
   //     START_COMPOSITE_PARAM( vec4(0,0,0,0) )
   //         ADD_VAC_PARAM( "r", 1.f );
   //         ADD_VAC_PARAM( "g", 1.f );
   //         ADD_VAC_PARAM( "b", 1.f );
   //         ADD_VAC_PARAM( "a", 1.f );
   //     END_COMPOSITE_PARAM()
   //END_MODEL()

    ModelHelper h( timeEvaluator );
    
    h.CreateVacModel();
    h.AddSimpleParam( "width", 1.f );
    h.AddSimpleParam( "height", 1.f );

    return h.GetModel();
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
DefaultRectPlugin::DefaultRectPlugin    ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model, bool timeInvariantDimensions )
    : BasePlugin( name, uid, prev, std::static_pointer_cast< IPluginParamValModel >( model ) )
    , m_vaChannel( nullptr )
    , m_paramValModel( model )
    , m_widthParam( nullptr )
    , m_heightParam( nullptr )
{
    auto params = VertexAttributesChannelModel()->GetParameters();

    assert( params.size() == 2 );

    m_widthParam    = QueryTypedParam< ParamFloatPtr >( params[ 0 ] );
    m_heightParam   = QueryTypedParam< ParamFloatPtr >( params[ 1 ] );

    m_lastW = m_widthParam->Evaluate();
    m_lastH = m_heightParam->Evaluate();

    auto rect   = RectComponent::Create( m_lastW, m_lastH );
    m_vaChannel = ChannelsFactory::CreateVertexAttributesChannel( rect, timeInvariantDimensions );

    m_rct = rect;
}

// *************************************
//
DefaultRectPlugin::~DefaultRectPlugin   ()
{
}

// *************************************
//
IVertexAttributesChannelConstPtr    DefaultRectPlugin::GetVertexAttributesChannel  () const
{
    return m_vaChannel;
}

// *************************************
//
void                                DefaultRectPlugin::Update                      ( TimeType t )
{
    { t; } // FIXME: suppress unused warning
    //FIXME: reimplement va channel (no time, no explicit update and so on)
    m_paramValModel->Update();

    //This code has to be executed in a plugin as only plugin knows how to translate its state to geometry representation
    float w = m_widthParam->Evaluate();
    float h = m_heightParam->Evaluate();

    auto needssAttrsUpdate = false;

    if( ( fabs( m_lastW - w ) + fabs( m_lastH - h ) ) > 0.001f )
    {
        m_rct->SetRectSize( w, h );

        needssAttrsUpdate = true;

        m_lastW = w;
        m_lastH = h;
    }

    m_vaChannel->SetNeedsAttributesUpdate( needssAttrsUpdate );
}

} // model
} // bv
