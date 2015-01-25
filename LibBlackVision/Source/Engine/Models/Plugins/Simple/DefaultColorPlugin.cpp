#include "DefaultColorPlugin.h"

#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"
#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"


namespace bv { namespace model {

// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultColorPluginDesc::DefaultColorPluginDesc                          ()
    : BasePluginDescriptor( UID(), "solid color" )
{
}

// *******************************
//
IPluginPtr              DefaultColorPluginDesc::CreatePlugin                ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultColorPlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   DefaultColorPluginDesc::CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const
{
    DefaultPluginParamValModelPtr model  = std::make_shared< DefaultPluginParamValModel >();
    DefaultParamValModelPtr psModel      = std::make_shared< DefaultParamValModel >();
    SimpleVec4EvaluatorPtr evaluator     = ParamValEvaluatorFactory::CreateSimpleVec4Evaluator( "color", timeEvaluator );

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
    return "Assets/Shaders/Deprecated/solid.frag";
}


// ************************************************************************* PLUGIN ************************************************************************* 

// *******************************
//
DefaultColorPlugin::DefaultColorPlugin  ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin( name, uid, prev, std::static_pointer_cast< IPluginParamValModel >( model ) )
    , m_pixelShaderChannel( nullptr )
    , m_paramValModel( model )
{ 
    m_pixelShaderChannel = DefaultPixelShaderChannelPtr( DefaultPixelShaderChannel::Create( DefaultColorPluginDesc::PixelShaderSource(), model->GetPixelShaderChannelModel(), nullptr ) );
    m_pixelShaderChannel->GetRendererContext()->alphaCtx->blendEnabled = true;
}

// *************************************
//
DefaultColorPlugin::~DefaultColorPlugin ()
{
}

// *************************************
//
IPixelShaderChannelConstPtr         DefaultColorPlugin::GetPixelShaderChannel       () const
{
    return m_pixelShaderChannel;    
}

// *************************************
//
void                                DefaultColorPlugin::Update                      ( TimeType t )
{
    m_paramValModel->Update();
    m_pixelShaderChannel->PostUpdate();
}

} // model
} // bv
