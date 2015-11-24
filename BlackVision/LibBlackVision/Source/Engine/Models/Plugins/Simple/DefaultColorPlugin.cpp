#include "DefaultColorPlugin.h"

#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"
#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"

namespace bv { namespace model {

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
    START_MODEL( timeEvaluator )
        ADD_PS_EVAL_PARAM( "color", glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f ) )
        //START_COMPOSITE_PS_EVAL_PARAM( "color" )
        //    ADD_PS_PARAM( "r", 0.f )
        //END_COMPOSITE_PS_PARAM()
    END_MODEL()
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
    m_pixelShaderChannel->GetRendererContext()->alphaCtx->blendEnabled = true;
	//HelperPixelShaderChannel::SetRendererContextUpdate( m_psc );
}


// *******************************
//
DefaultColorPlugin::DefaultColorPlugin  ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin( name, uid, prev, std::static_pointer_cast< IPluginParamValModel >( model ) )
    , m_pixelShaderChannel( nullptr )
    , m_paramValModel( model )
{ 
    m_pixelShaderChannel = DefaultPixelShaderChannel::Create( model->GetPixelShaderChannelModel() );

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

    m_pixelShaderChannel->PostUpdate();
}

} // model
} // bv
