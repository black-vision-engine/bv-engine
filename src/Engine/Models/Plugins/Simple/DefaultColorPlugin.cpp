#include "DefaultColorPlugin.h"

#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"
#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"


namespace bv { namespace model {

// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultColorPluginDesc::DefaultColorPluginDesc                          ()
    : BasePluginDescriptor( UID() )    
{
}

// *******************************
//
IPlugin *               DefaultColorPluginDesc::CreatePlugin                ( const std::string & name, const IPlugin * prev ) const
{
    return CreatePluginTyped< DefaultColorPlugin >( name, prev );
}

// *******************************
//
DefaultPluginParamValModel *    DefaultColorPluginDesc::CreateDefaultModel  () const
{
    DefaultPluginParamValModel * model  = new DefaultPluginParamValModel();
    DefaultParamValModel * psModel      = new DefaultParamValModel();
    SimpleVec4Evaluator * evaluator     = ParamValEvaluatorFactory::CreateSimpleVec4Evaluator( "color" );

    psModel->RegisterAll( evaluator );
    model->SetPixelShaderChannelModel( psModel );

    //Set default values
    evaluator->Parameter()->SetVal( glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f ), TimeType( 0.0 ) );

    return model;
}

// *******************************
//
std::string             DefaultColorPluginDesc::UID                         ()
{
    return "DEFAULT_COLOR";
}

// *******************************
//
std::string             DefaultColorPluginDesc::PixelShaderSource           ()
{
    return "../dep/media/shaders/solid.frag";
}


// ************************************************************************* PLUGIN ************************************************************************* 

// *******************************
//
DefaultColorPlugin::DefaultColorPlugin  ( const std::string & name, const std::string & uid, const IPlugin * prev, DefaultPluginParamValModelPtr model )
    : BasePlugin( name, uid, prev, std::static_pointer_cast< IPluginParamValModel >( model ) )
    , m_pixelShaderChannel( nullptr )
    , m_vertexShaderChannel( nullptr )
    , m_paramValModel( model )
{ 
    m_pixelShaderChannel = DefaultPixelShaderChannelPtr( DefaultPixelShaderChannel::Create( DefaultColorPluginDesc::PixelShaderSource(), model->GetPixelShaderChannelModel(), false ) );

    if( prev == nullptr || prev->GetVertexShaderChannel() == nullptr )
    {
        m_vertexShaderChannel = DefaultVertexShaderChannelPtr( DefaultVertexShaderChannel::Create() );
    }
}

// *************************************
//
DefaultColorPlugin::~DefaultColorPlugin ()
{
}

// *************************************
//
const IPixelShaderChannel *         DefaultColorPlugin::GetPixelShaderChannel       () const
{
    return m_pixelShaderChannel.get();
}

// *************************************
//
const IVertexShaderChannel *        DefaultColorPlugin::GetVertexShaderChannel      () const
{
    if( m_vertexShaderChannel.get() == nullptr )
    {
        return BasePlugin::GetVertexShaderChannel();
    }

    return m_vertexShaderChannel.get();
}

// *************************************
//
void                                DefaultColorPlugin::Update                      ( TimeType t )
{
    m_paramValModel->Update( t );
    m_pixelShaderChannel->PostUpdate();
}

} // model
} // bv
