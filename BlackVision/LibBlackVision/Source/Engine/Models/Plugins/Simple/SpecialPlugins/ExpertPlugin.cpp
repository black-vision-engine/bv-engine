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


const std::string        ExpertPlugin::PARAMS::COLOR_BLENDING_MODE      = "alpha2";
const std::string        ExpertPlugin::PARAMS::ALPHA_BLENDING_MODE      = "alpha2";
const std::string        ExpertPlugin::PARAMS::BLEND_ENABLE             = "alpha2";

const std::string        ExpertPlugin::PARAMS::CULLING_ENABLE           = "alpha2";
const std::string        ExpertPlugin::PARAMS::CC_CULL_ORDER            = "alpha2";
const std::string        ExpertPlugin::PARAMS::ENABLE_DEPTH_TEST        = "alpha2";
const std::string        ExpertPlugin::PARAMS::ENABLE_DEPTH_WRITE       = "alpha2";
const std::string        ExpertPlugin::PARAMS::FILL_MODE                = "alpha2";



// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
ExpertPluginDesc::ExpertPluginDesc                          ()
    : BasePluginDescriptor( UID(), "expert" )
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

    // Create all models
    auto model = helper.GetModel();
    helper.SetOrCreatePluginModel();

    helper.AddSimpleParam( BlendHelper::PARAM::BLEND_ENABLE, true, true, true );
    helper.AddEnumParam( BlendHelper::PARAM::COLOR_BLEND_MODE, BlendHelper::BlendMode::BM_Alpha, true, true );
    helper.AddEnumParam( BlendHelper::PARAM::ALPHA_BLEND_MODE, BlendHelper::BlendMode::BM_None, true, true );

    helper.AddSimpleParam( ExpertPlugin::PARAMS::CULLING_ENABLE, true, true, true );
    helper.AddSimpleParam( ExpertPlugin::PARAMS::ENABLE_DEPTH_TEST, true, true, true );
    helper.AddSimpleParam( ExpertPlugin::PARAMS::ENABLE_DEPTH_WRITE, true, true, true );
    helper.AddSimpleParam( ExpertPlugin::PARAMS::CC_CULL_ORDER, true, true, true );
    //helper.AddSimpleParam( ExpertPlugin::PARAMS::ENABLE_DEPTH_TEST, true, true, true );

    return model;
}

// *******************************
//
std::string             ExpertPluginDesc::UID                       ()
{
    return "EXPERT_PLUGIN";
}


// ************************************************************************* PLUGIN *************************************************************************

// *************************************
// 
bool ExpertPlugin::SetPrevPlugin( IPluginPtr prev )
{
    if( BasePlugin::SetPrevPlugin( prev ) )
    {
        HelperPixelShaderChannel::CloneRenderContext( m_psc, prev );

        auto ctx = m_psc->GetRendererContext();

        SetParameter( GetParameter( PARAMS::CULLING_ENABLE ), 0.0f, ctx->cullCtx->enabled );
        SetParameter( GetParameter( PARAMS::CC_CULL_ORDER ), 0.0f, ctx->cullCtx->isCCWOrdered );
        SetParameter( GetParameter( PARAMS::ENABLE_DEPTH_TEST ), 0.0f, ctx->depthCtx->enabled );
        SetParameter( GetParameter( PARAMS::ENABLE_DEPTH_WRITE ), 0.0f, ctx->depthCtx->writable );

        SetParameter( GetParameter( BlendHelper::PARAM::BLEND_ENABLE ), 0.0f, ctx->alphaCtx->blendEnabled );

        BlendHelper::SetBlendRendererContext( m_psc, m_colorBlendMode.GetParameter(), m_alphaBlendMode.GetParameter() );

        return true;
    }
    return false;
}

// *************************************
// 
ExpertPlugin::ExpertPlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin( name, uid, prev, model )
    , m_psc( nullptr )
{
    m_blendEnabled      = GetValueParamState< bool >( GetPluginParamValModel()->GetPluginModel().get(), BlendHelper::PARAM::BLEND_ENABLE );
    m_colorBlendMode    = GetValueParamState< BlendHelper::BlendMode >( GetPluginParamValModel()->GetPluginModel().get(), BlendHelper::PARAM::COLOR_BLEND_MODE );
    m_alphaBlendMode    = GetValueParamState< BlendHelper::BlendMode >( GetPluginParamValModel()->GetPluginModel().get(), BlendHelper::PARAM::ALPHA_BLEND_MODE );

    m_cullingEnabled    = GetValueParamState< bool >( GetPluginParamValModel()->GetPluginModel().get(), PARAMS::CULLING_ENABLE );
    m_ccCullOrder       = GetValueParamState< bool >( GetPluginParamValModel()->GetPluginModel().get(), PARAMS::CC_CULL_ORDER );
    m_enableDepthTest   = GetValueParamState< bool >( GetPluginParamValModel()->GetPluginModel().get(), PARAMS::ENABLE_DEPTH_TEST );
    m_enableDepthWrite  = GetValueParamState< bool >( GetPluginParamValModel()->GetPluginModel().get(), PARAMS::ENABLE_DEPTH_WRITE );

    m_fillMode          = GetValueParamState< FillContext::Mode >( GetPluginParamValModel()->GetPluginModel().get(), PARAMS::FILL_MODE );

    SetPrevPlugin( prev );
}

// *************************************
// 
ExpertPlugin::~ExpertPlugin         ()
{}

// *************************************
// 
bool							    ExpertPlugin::IsValid     () const
{
    return ( GetPrevPlugin()->IsValid() );
}

// *************************************
// 
bool                                ExpertPlugin::LoadResource  ( AssetDescConstPtr assetDescr )
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
    return GetPrevPlugin()->GetVertexShaderChannel();
}

// *************************************
// 
void                                ExpertPlugin::Update                      ( TimeType t )
{
    BasePlugin::Update( t );

    BlendHelper::UpdateBlendState( m_psc, m_blendEnabled, m_colorBlendMode, m_alphaBlendMode );


    HelperPixelShaderChannel::PropagateUpdate( m_psc, GetPrevPlugin() );

    m_psc->PostUpdate();
}




} // model
} // bv

