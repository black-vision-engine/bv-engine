#include "stdafx.h"

#include "ExpertPlugin.h"

#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine/Interfaces/IValue.h"

#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/HelperVertexShaderChannel.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"

#include "Assets/DefaultAssets.h"


namespace bv {
namespace model {


const std::string        ExpertPlugin::PARAMS::ALPHA = "alpha2";



// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
ExpertPluginDesc::ExpertPluginDesc                          ()
    : BasePluginDescriptor( UID(), "EXPERT_PLUGIN", "expert" )
{
}

// *******************************
//
IPluginPtr              ExpertPluginDesc::CreatePlugin              ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< ExpertPlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   ExpertPluginDesc::CreateDefaultModel( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper helper( timeEvaluator );

    //Create all models
    auto model = helper.GetModel();
    DefaultParamValModelPtr vsModel = std::make_shared< DefaultParamValModel >();

    //Create all parameters and evaluators
    SimpleTransformEvaluatorPtr trTxEvaluator = ParamValEvaluatorFactory::CreateSimpleTransformEvaluator( "txBlendMat", timeEvaluator );

    helper.SetOrCreatePluginModel();

    helper.SetOrCreatePSModel();
    helper.AddSimpleParam( ExpertPlugin::PARAMS::ALPHA, 1.f, true );

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
std::string             ExpertPluginDesc::UID                       ()
{
    return "PluginUID";
}

// *******************************
// 
std::string             ExpertPluginDesc::TextureName               ()
{
    return "TextureName";
}



// ************************************************************************* PLUGIN *************************************************************************

// *************************************
// 
void ExpertPlugin::SetPrevPlugin( IPluginPtr prev )
{
    BasePlugin::SetPrevPlugin( prev );

    HelperPixelShaderChannel::CloneRenderContext( m_psc, prev );
}

// *************************************
// 
ExpertPlugin::ExpertPlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin( name, uid, prev, model )
    , m_psc( nullptr )
    , m_vsc( nullptr )
{
    m_psc = DefaultPixelShaderChannel::Create( model->GetPixelShaderChannelModel() );
    m_vsc = DefaultVertexShaderChannel::Create( model->GetVertexShaderChannelModel() );

    SetPrevPlugin( prev );
}

// *************************************
// 
ExpertPlugin::~ExpertPlugin         ()
{}

// *************************************
// 
bool							ExpertPlugin::IsValid     () const
{
    return ( m_prevPlugin->IsValid() );
}

// *************************************
// 
bool                            ExpertPlugin::LoadResource  ( AssetDescConstPtr assetDescr )
{    return false;  }


// *************************************
// 
IPixelShaderChannelPtr              ExpertPlugin::GetPixelShaderChannel       () const
{
    return m_psc;
}

// *************************************
// 
IVertexShaderChannelConstPtr        ExpertPlugin::GetVertexShaderChannel      () const
{
    return m_vsc;
}

// *************************************
// 
void                                ExpertPlugin::Update                      ( TimeType t )
{
    BasePlugin::Update( t );

    HelperVertexShaderChannel::InverseTextureMatrix( m_pluginParamValModel, "txBlendMat" );
    HelperPixelShaderChannel::PropagateUpdate( m_psc, m_prevPlugin );

    m_vsc->PostUpdate();
    m_psc->PostUpdate();
}




} // model
} // bv

