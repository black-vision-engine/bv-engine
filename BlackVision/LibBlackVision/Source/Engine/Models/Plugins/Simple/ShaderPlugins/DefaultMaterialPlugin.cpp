#include "stdafx.h"

#include "DefaultMaterialPlugin.h"

#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"
#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"



#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {

const std::string        DefaultMaterialPlugin::PARAM::DIFFUSE       = "mtlDiffuse";
const std::string        DefaultMaterialPlugin::PARAM::AMBIENT       = "mtlAmbient";
const std::string        DefaultMaterialPlugin::PARAM::SPECULAR      = "mtlSpecular";
const std::string        DefaultMaterialPlugin::PARAM::EMISSION      = "mtlEmission";
const std::string        DefaultMaterialPlugin::PARAM::SHININESS     = "mtlShininess";

// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultMaterialPluginDesc::DefaultMaterialPluginDesc                          ()
    : BasePluginDescriptor( UID(), "material",  "mtl" )
{
}

// *******************************
//
IPluginPtr              DefaultMaterialPluginDesc::CreatePlugin                ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultMaterialPlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   DefaultMaterialPluginDesc::CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper helper( timeEvaluator );

    //Create all models
    auto model  = helper.GetModel();

    helper.SetOrCreatePSModel();
    helper.AddSimpleParam( DefaultMaterialPlugin::PARAM::DIFFUSE, glm::vec4( 0.5f, 0.5f, 0.5f, 1.0f ), true );
    helper.AddSimpleParam( DefaultMaterialPlugin::PARAM::AMBIENT, glm::vec4( 0.2f, 0.2f, 0.2f, 1.0f ), true );
    helper.AddSimpleParam( DefaultMaterialPlugin::PARAM::SPECULAR, glm::vec4( 1.0f, 1.0f, 1.0f, 0.0f ), true );
    helper.AddSimpleParam( DefaultMaterialPlugin::PARAM::EMISSION, glm::vec4( 0.1f, 0.1f, 0.1f, 1.0f ), true );
    helper.AddSimpleParam( DefaultMaterialPlugin::PARAM::SHININESS, 128, true );

    return helper.GetModel();
}

// *******************************
//
std::string             DefaultMaterialPluginDesc::UID                         ()
{
    return "DEFAULT_MATERIAL";
}


// ************************************************************************* PLUGIN ************************************************************************* 


// *************************************
// 
void DefaultMaterialPlugin::SetPrevPlugin( IPluginPtr prev )
{
    BasePlugin::SetPrevPlugin( prev );

	HelperPixelShaderChannel::CloneRenderContext( m_pixelShaderChannel, prev );
    m_pixelShaderChannel->GetRendererContext()->cullCtx->enabled = false;
	//HelperPixelShaderChannel::SetRendererContextUpdate( m_psc );
}


// *******************************
//
DefaultMaterialPlugin::DefaultMaterialPlugin  ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin( name, uid, prev, std::static_pointer_cast< IPluginParamValModel >( model ) )
    , m_pixelShaderChannel( nullptr )
    , m_paramValModel( model )
{ 
    m_pixelShaderChannel = DefaultPixelShaderChannel::Create( model->GetPixelShaderChannelModel() );
	SetPrevPlugin( prev );
}

// *************************************
//
DefaultMaterialPlugin::~DefaultMaterialPlugin ()
{}

// *************************************
//
IPixelShaderChannelPtr              DefaultMaterialPlugin::GetPixelShaderChannel       () const
{
    return m_pixelShaderChannel;    
}

// *************************************
//
void                                DefaultMaterialPlugin::Update                      ( TimeType t )
{
	BasePlugin::Update( t );

    HelperPixelShaderChannel::PropagateUpdate( m_pixelShaderChannel, m_prevPlugin );
    m_pixelShaderChannel->PostUpdate();
}

} // model
} // bv
