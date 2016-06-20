#include "stdafx.h"

#include "DefaultFadePlugin.h"

#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine/Interfaces/IValue.h"

#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/HelperVertexShaderChannel.h"

#include "Assets/DefaultAssets.h"


namespace bv { namespace model {


const std::string        DefaultFadePlugin::PARAMS::POINT1             = "FadePoint1";
const std::string        DefaultFadePlugin::PARAMS::POINT2             = "FadePoint2";
const std::string        DefaultFadePlugin::PARAMS::POINT3             = "FadePoint3";
const std::string        DefaultFadePlugin::PARAMS::POINT4             = "FadePoint4";

const std::string        DefaultFadePlugin::PARAMS::POINT1_ALPHA        = "FadeAlpha1";
const std::string        DefaultFadePlugin::PARAMS::POINT2_ALPHA        = "FadeAlpha2";
const std::string        DefaultFadePlugin::PARAMS::POINT3_ALPHA        = "FadeAlpha3";
const std::string        DefaultFadePlugin::PARAMS::POINT4_ALPHA        = "FadeAlpha4";




// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultFadePluginDesc::DefaultFadePluginDesc                          ()
    : BasePluginDescriptor( UID(), "fade", "fade" )
{
}

// *******************************
//
IPluginPtr              DefaultFadePluginDesc::CreatePlugin              ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultFadePlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   DefaultFadePluginDesc::CreateDefaultModel( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper helper( timeEvaluator );

    //Create all models
    auto model  = helper.GetModel();
    DefaultParamValModelPtr vsModel      = std::make_shared< DefaultParamValModel >();

    //Create all parameters and evaluators
    SimpleTransformEvaluatorPtr trTxEvaluator    = ParamValEvaluatorFactory::CreateSimpleTransformEvaluator( "fadeMat", timeEvaluator );

    helper.CreatePluginModel();

    helper.CreatePSModel();
    helper.AddSimpleParam( DefaultFadePlugin::PARAMS::POINT1_ALPHA, 1.f, true );
    helper.AddSimpleParam( DefaultFadePlugin::PARAMS::POINT2_ALPHA, 1.f, true );
    helper.AddSimpleParam( DefaultFadePlugin::PARAMS::POINT3_ALPHA, 1.f, true );
    helper.AddSimpleParam( DefaultFadePlugin::PARAMS::POINT4_ALPHA, 1.f, true );

    helper.AddSimpleParam( DefaultFadePlugin::PARAMS::POINT1, glm::vec2( 0.0, 0.0 ), true );
    helper.AddSimpleParam( DefaultFadePlugin::PARAMS::POINT2, glm::vec2( 1.0, 0.0 ), true );
    helper.AddSimpleParam( DefaultFadePlugin::PARAMS::POINT3, glm::vec2( 0.0, 1.0 ), true );
    helper.AddSimpleParam( DefaultFadePlugin::PARAMS::POINT4, glm::vec2( 1.0, 1.0 ), true );

    //Register all parameters and evaloators in models
    vsModel->RegisterAll( trTxEvaluator );

    //Set models structure
    model->SetVertexShaderChannelModel( vsModel );

    //Set default values of all parameters
    trTxEvaluator->Parameter()->Transform().InitializeDefaultSRT();
    trTxEvaluator->Parameter()->Transform().SetCenter( glm::vec3( 0.5, 0.5, 0.0 ), 0.0f );

    return model;
}

// *******************************
//
std::string             DefaultFadePluginDesc::UID                       ()
{
    return "DEFAULT_FADE_PLUGIN";
}



// ************************************************************************* PLUGIN *************************************************************************

// *************************************
// 
void DefaultFadePlugin::SetPrevPlugin( IPluginPtr prev )
{
    BasePlugin::SetPrevPlugin( prev );

    HelperPixelShaderChannel::CloneRenderContext( m_psc, prev );
}

// *************************************
// 
DefaultFadePlugin::DefaultFadePlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin< IPlugin >( name, uid, prev, model )
    , m_psc( nullptr )
    , m_vsc( nullptr )
{
    m_psc = DefaultPixelShaderChannel::Create( model->GetPixelShaderChannelModel() );
    m_vsc = DefaultVertexShaderChannel::Create( model->GetVertexShaderChannelModel() );

    SetPrevPlugin( prev );
}

// *************************************
// 
DefaultFadePlugin::~DefaultFadePlugin         ()
{}

// *************************************
// 
bool							DefaultFadePlugin::IsValid     () const
{
    return ( m_prevPlugin->IsValid() );
}

// *************************************
// 
bool                            DefaultFadePlugin::LoadResource  ( AssetDescConstPtr assetDescr )
{
    return false;
}


// *************************************
// 
IPixelShaderChannelPtr              DefaultFadePlugin::GetPixelShaderChannel       () const
{
    return m_psc;
}

// *************************************
// 
IVertexShaderChannelConstPtr        DefaultFadePlugin::GetVertexShaderChannel      () const
{
    return m_vsc;
}

// *************************************
// 
void                                DefaultFadePlugin::Update                      ( TimeType t )
{
    BasePlugin::Update( t );

    HelperVertexShaderChannel::InverseTextureMatrix( m_pluginParamValModel, "fadeMat" );    
    HelperPixelShaderChannel::PropagateUpdate( m_psc, m_prevPlugin );

    m_vsc->PostUpdate();
    m_psc->PostUpdate();    
}




} // model
} // bv

