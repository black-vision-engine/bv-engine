#include "stdafx.h"

#include "DefaultColorPlugin.h"

#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"
#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"



#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {

const std::string        DefaultColorPlugin::PARAM::COLOR          = "color";

// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultColorPluginDesc::DefaultColorPluginDesc                          ()
    : BasePluginDescriptor( UID(), "solid color",  "col" )
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
    ModelHelper helper( timeEvaluator );

    //Create all models
    auto model  = helper.GetModel();

    helper.SetOrCreatePluginModel();
    helper.AddSimpleParam( BlendHelper::PARAM::BLEND_ENABLE, true, true, true );
	helper.AddEnumParam( BlendHelper::PARAM::BLEND_MODE, BlendHelper::BlendMode::BM_Normal, true, true );

    helper.SetOrCreatePSModel();
    helper.AddSimpleParam( DefaultColorPlugin::PARAM::COLOR, glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f ), true );

    return helper.GetModel();
}

// *******************************
//
std::string             DefaultColorPluginDesc::UID                         ()
{
    return "DEFAULT_COLOR";
}


// ************************************************************************* PLUGIN ************************************************************************* 


// *************************************
// 
void DefaultColorPlugin::SetPrevPlugin( IPluginPtr prev )
{
    BasePlugin::SetPrevPlugin( prev );

	HelperPixelShaderChannel::CloneRenderContext( m_pixelShaderChannel, prev );
	auto ctx = m_pixelShaderChannel->GetRendererContext();
    ctx->cullCtx->enabled = false;

    ctx->alphaCtx->blendEnabled = m_blendEnabled.GetParameter().Evaluate();
	BlendHelper::SetBlendRendererContext( m_pixelShaderChannel, m_blendMode.GetParameter() );
}


// *******************************
//
DefaultColorPlugin::DefaultColorPlugin  ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin( name, uid, prev, std::static_pointer_cast< IPluginParamValModel >( model ) )
    , m_pixelShaderChannel( nullptr )
    , m_paramValModel( model )
{ 
    m_pixelShaderChannel = DefaultPixelShaderChannel::Create( model->GetPixelShaderChannelModel() );

	m_blendEnabled = GetValueParamState< bool >( GetPluginParamValModel()->GetPluginModel().get(), BlendHelper::PARAM::BLEND_ENABLE );
	m_blendMode = GetValueParamState< BlendHelper::BlendMode >( GetPluginParamValModel()->GetPluginModel().get(), BlendHelper::PARAM::BLEND_MODE );

	SetPrevPlugin( prev );
}

// *************************************
//
DefaultColorPlugin::~DefaultColorPlugin ()
{}

// *************************************
//
IPixelShaderChannelPtr              DefaultColorPlugin::GetPixelShaderChannel       () const
{
    return m_pixelShaderChannel;    
}

// *************************************
//
void                                DefaultColorPlugin::Update                      ( TimeType t )
{
	BasePlugin::Update( t );

	BlendHelper::UpdateBlendState( m_pixelShaderChannel, m_blendEnabled, m_blendMode );

    m_pixelShaderChannel->PostUpdate();
}

} // model
} // bv
