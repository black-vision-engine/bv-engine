#include "stdafx.h"

#include "DefaultColorPlugin.h"

#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"
#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"



#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {

const std::string        DefaultColorPlugin::PARAM_BLEND_ENABLE   = "blend enable";
const std::string        DefaultColorPlugin::PARAM_COLOR          = "color";

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
    //START_MODEL( timeEvaluator )
    //    ADD_PS_EVAL_PARAM( "color", glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f ) )
    //    //START_COMPOSITE_PS_EVAL_PARAM( "color" )
    //    //    ADD_PS_PARAM( "r", 0.f )
    //    //END_COMPOSITE_PS_PARAM()
    //END_MODEL()

    ModelHelper helper( timeEvaluator );

    //Create all models
    auto model  = helper.GetModel();

    helper.SetOrCreatePluginModel();
    helper.AddSimpleParam( DefaultColorPlugin::PARAM_BLEND_ENABLE, true, true, true );

    helper.SetOrCreatePSModel();
    helper.AddSimpleParam( DefaultColorPlugin::PARAM_COLOR, glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f ), true );

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
bool DefaultColorPlugin::SetPrevPlugin( IPluginPtr prev )
{
    if( BasePlugin::SetPrevPlugin( prev ) )
    {
        HelperPixelShaderChannel::CloneRenderContext( m_pixelShaderChannel, prev );
        m_pixelShaderChannel->GetRendererContext()->alphaCtx->blendEnabled = true;
        m_pixelShaderChannel->GetRendererContext()->cullCtx->enabled = false;
        //HelperPixelShaderChannel::SetRendererContextUpdate( m_psc );
        return true;
    }
    else
        return false;
}


// *******************************
//
DefaultColorPlugin::DefaultColorPlugin  ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin( name, uid, prev, std::static_pointer_cast< IPluginParamValModel >( model ) )
    , m_pixelShaderChannel( nullptr )
    , m_paramValModel( model )
{ 
    m_pixelShaderChannel = DefaultPixelShaderChannel::Create( model->GetPixelShaderChannelModel() );

	m_blendEnabled = GetValueParamState< bool >( GetPluginParamValModel()->GetPluginModel().get(), PARAM_BLEND_ENABLE );

	SetPrevPlugin( prev );
}

// *************************************
//
DefaultColorPlugin::~DefaultColorPlugin ()
{
}

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

    if( m_blendEnabled.Changed() )
    {
        auto ctx = m_pixelShaderChannel->GetRendererContext();
        ctx->alphaCtx->blendEnabled = std::static_pointer_cast<ParamBool>( GetParameter( PARAM_BLEND_ENABLE ) )->Evaluate();

        HelperPixelShaderChannel::SetRendererContextUpdate( m_pixelShaderChannel );
    }

    m_pixelShaderChannel->PostUpdate();
}

} // model
} // bv
