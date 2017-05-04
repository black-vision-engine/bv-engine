#include "stdafx.h"

#include "ExpertPlugin.h"

#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine/Interfaces/IValue.h"

#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/HelperVertexShaderChannel.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"

#include "Assets/DefaultAssets.h"



DEFINE_ENUM_PARAMETER_CREATOR( bv::model::FillContext::Mode );

namespace bv {
namespace model {


const std::string        ExpertPlugin::PARAMS::CULLING_ENABLE           = "enable culling";
const std::string        ExpertPlugin::PARAMS::CC_CULL_ORDER            = "cc order";
const std::string        ExpertPlugin::PARAMS::ENABLE_DEPTH_TEST        = "enable depth test";
const std::string        ExpertPlugin::PARAMS::ENABLE_DEPTH_WRITE       = "enable depth write";
const std::string        ExpertPlugin::PARAMS::FILL_MODE                = "fill mode";

const std::string        ExpertPlugin::PARAMS::RESET_SETTINGS           = "reset settings";





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
    helper.AddEnumParam( ExpertPlugin::PARAMS::FILL_MODE, FillContext::Mode::M_POLYGONS, true, true );

    helper.AddSimpleParam( ExpertPlugin::PARAMS::RESET_SETTINGS, false, true, true );

    return model;
}

// *******************************
//
std::string             ExpertPluginDesc::UID                       ()
{
    return "EXPERT";
}


// ************************************************************************* PLUGIN *************************************************************************


// *************************************
// 
ExpertPlugin::ExpertPlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin( name, uid, prev, model )
    , m_psc( nullptr )
    , m_firstAttach( true )
{
    m_psc = DefaultPixelShaderChannel::Create();
    auto ctx = m_psc->GetRendererContext();

    m_blendEnabled      = GetValueParamState< bool >( GetPluginParamValModel()->GetPluginModel().get(), BlendHelper::PARAM::BLEND_ENABLE );
    m_colorBlendMode    = GetValueParamState< BlendHelper::BlendMode >( GetPluginParamValModel()->GetPluginModel().get(), BlendHelper::PARAM::COLOR_BLEND_MODE );
    m_alphaBlendMode    = GetValueParamState< BlendHelper::BlendMode >( GetPluginParamValModel()->GetPluginModel().get(), BlendHelper::PARAM::ALPHA_BLEND_MODE );

    m_cullingEnabled    = GetValueParamState< bool >( GetPluginParamValModel()->GetPluginModel().get(), PARAMS::CULLING_ENABLE );
    m_ccCullOrder       = GetValueParamState< bool >( GetPluginParamValModel()->GetPluginModel().get(), PARAMS::CC_CULL_ORDER );
    m_enableDepthTest   = GetValueParamState< bool >( GetPluginParamValModel()->GetPluginModel().get(), PARAMS::ENABLE_DEPTH_TEST );
    m_enableDepthWrite  = GetValueParamState< bool >( GetPluginParamValModel()->GetPluginModel().get(), PARAMS::ENABLE_DEPTH_WRITE );

    m_fillMode          = GetValueParamState< FillContext::Mode >( GetPluginParamValModel()->GetPluginModel().get(), PARAMS::FILL_MODE );

    m_resetSettings     = GetValueParamState< bool >( GetPluginParamValModel()->GetPluginModel().get(), PARAMS::RESET_SETTINGS );

    SetPrevPlugin( prev );
}

// *************************************
// 
ExpertPlugin::~ExpertPlugin         ()
{}


// *************************************
// 
bool                ExpertPlugin::SetPrevPlugin( IPluginPtr prev )
{
    if( BasePlugin::SetPrevPlugin( prev ) )
    {
        HelperPixelShaderChannel::CloneRenderContext( m_psc, prev );
        return true;
    }
    return false;
}

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

    if( m_resetSettings.GetValue() )
    {
        ApplyFromPrevious();
        SetParameter( GetParameter( PARAMS::RESET_SETTINGS ), 0.0f, false );
    }
    else
    {
        UpdateContext();        
    }

    HelperPixelShaderChannel::PropagateUpdate( m_psc, GetPrevPlugin() );
    m_psc->PostUpdate();
}

// ***********************
//
void                                ExpertPlugin::ApplyFromPrevious()
{
    auto prevPlugin = GetPrevPlugin();
    auto ctx = prevPlugin->GetRendererContext();

    SetParameter( GetParameter( PARAMS::CULLING_ENABLE ), 0.0f, ctx->cullCtx->enabled );
    SetParameter( GetParameter( PARAMS::CC_CULL_ORDER ), 0.0f, ctx->cullCtx->isCCWOrdered );
    SetParameter( GetParameter( PARAMS::ENABLE_DEPTH_TEST ), 0.0f, ctx->depthCtx->enabled );
    SetParameter( GetParameter( PARAMS::ENABLE_DEPTH_WRITE ), 0.0f, ctx->depthCtx->writable );
    SetParameter( GetParameter( PARAMS::FILL_MODE ), 0.0f, ctx->fillCtx->fillMode );

    SetParameter( GetParameter( BlendHelper::PARAM::BLEND_ENABLE ), 0.0f, ctx->alphaCtx->blendEnabled );

    SetParameter( GetParameter( BlendHelper::PARAM::COLOR_BLEND_MODE ), 0.0f, BlendHelper::ContextToColorBlendMode( ctx ) );
    SetParameter( GetParameter( BlendHelper::PARAM::ALPHA_BLEND_MODE ), 0.0f, BlendHelper::ContextToAlphaBlendMode( ctx ) );
}

// ***********************
//
void                                ExpertPlugin::UpdateContext()
{
    bool changed = false;
    auto ctx = m_psc->GetRendererContext();

    BlendHelper::UpdateBlendState( m_psc, m_blendEnabled, m_colorBlendMode, m_alphaBlendMode );

    if( m_cullingEnabled.Changed() )
    {
        ctx->cullCtx->enabled = m_cullingEnabled.GetValue();
        changed = true;
    }

    if( m_ccCullOrder.Changed() )
    {
        ctx->cullCtx->isCCWOrdered = m_ccCullOrder.GetValue();
        changed = true;
    }

    if( m_enableDepthTest.Changed() )
    {
        ctx->depthCtx->enabled = m_enableDepthTest.GetValue();
        changed = true;
    }

    if( m_enableDepthWrite.Changed() )
    {
        ctx->depthCtx->writable = m_enableDepthWrite.GetValue();
        changed = true;
    }

    if( m_fillMode.Changed() )
    {
        ctx->fillCtx->fillMode = m_fillMode.GetValue();
        changed = true;
    }

    if( changed )
    {
        HelperPixelShaderChannel::SetRendererContextUpdate( m_psc );
    }
    
}





} // model
} // bv

