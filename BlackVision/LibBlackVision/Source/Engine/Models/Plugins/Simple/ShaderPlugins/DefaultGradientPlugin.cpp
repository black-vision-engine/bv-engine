#include "stdafx.h"

#include "DefaultGradientPlugin.h"

#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"
#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"
#include "Engine/Models/Plugins/Channels/Geometry/HelperVertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/HelperVertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"

#include "Engine/Models/Plugins/HelperUVGenerator.h"
#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"




#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {


const std::string        DefaultGradientPlugin::PARAM_ALPHA         = "alpha";

const std::string        DefaultGradientPlugin::PARAM_POINT1        = "point1";
const std::string        DefaultGradientPlugin::PARAM_POINT2        = "point2";
const std::string        DefaultGradientPlugin::PARAM_COLOR1        = "color1";
const std::string        DefaultGradientPlugin::PARAM_COLOR2        = "color2";

const std::string        DefaultGradientPlugin::TX_MAT              = "txMat";


// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultGradientPluginDesc::DefaultGradientPluginDesc                          ()
    : BasePluginDescriptor( UID(), "linear_gradient", "lg" )
{}

// *******************************
//
IPluginPtr              DefaultGradientPluginDesc::CreatePlugin              ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultGradientPlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   DefaultGradientPluginDesc::CreateDefaultModel( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper helper( timeEvaluator );

    //Create all models
    auto model  = helper.GetModel();
    DefaultParamValModelPtr vsModel      = std::make_shared< DefaultParamValModel >();

    helper.SetOrCreatePluginModel();
    helper.AddSimpleParam( BlendHelper::PARAM::BLEND_ENABLE, true, true, true );
	helper.AddEnumParam( BlendHelper::PARAM::BLEND_MODE, BlendHelper::BlendMode::BM_Normal, true, true );

    helper.SetOrCreatePSModel();
    helper.AddSimpleParam( DefaultGradientPlugin::PARAM_ALPHA, 1.f, true );

    helper.AddSimpleParam( DefaultGradientPlugin::PARAM_POINT1, glm::vec2( 0.0f, 1.0f ), true );
    helper.AddSimpleParam( DefaultGradientPlugin::PARAM_POINT2, glm::vec2( 0.0f, 0.0f ), true );
    helper.AddSimpleParam( DefaultGradientPlugin::PARAM_COLOR1, glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f ), true );
    helper.AddSimpleParam( DefaultGradientPlugin::PARAM_COLOR2, glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f ), true );

    helper.SetOrCreateVSModel();
    helper.AddTransformParam( DefaultGradientPlugin::TX_MAT, true );

    auto param = helper.GetModel()->GetVertexShaderChannelModel()->GetParameter( DefaultGradientPlugin::TX_MAT );
    SetParameterCenterMass( param, 0.0f, glm::vec3( 0.5, 0.5, 0.0 ) );

    return model;
}

// *******************************
//
std::string             DefaultGradientPluginDesc::UID                       ()
{
    return "DEFAULT_LINEAR_GRADIENT";
}


std::string             DefaultGradientPluginDesc::TextureName               ()
{
    return "Grad0";
}

// ************************************************************************* PLUGIN *************************************************************************

// *************************************
// 
void					DefaultGradientPlugin::SetPrevPlugin				( IPluginPtr prev )
{
    BasePlugin::SetPrevPlugin( prev );

    InitVertexAttributesChannel();

    HelperPixelShaderChannel::CloneRenderContext( m_psc, prev );
    m_psc->GetRendererContext()->cullCtx->enabled = false;

	m_psc->GetRendererContext()->alphaCtx->blendEnabled = m_blendEnabled.GetParameter().Evaluate();
	BlendHelper::SetBlendRendererContext( m_psc, m_blendMode.GetParameter() );
}

// *************************************
//
DefaultGradientPlugin::DefaultGradientPlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin( name, uid, prev, model )
    , m_psc( nullptr )
    , m_vsc( nullptr )
    , m_vaChannel( nullptr )
{
    m_psc = DefaultPixelShaderChannel::Create( model->GetPixelShaderChannelModel() );
    m_vsc = DefaultVertexShaderChannel::Create( model->GetVertexShaderChannelModel() );

	m_blendEnabled = GetValueParamState< bool >( GetPluginParamValModel()->GetPluginModel().get(), BlendHelper::PARAM::BLEND_ENABLE );
	m_blendMode = GetValueParamState< BlendHelper::BlendMode >( GetPluginParamValModel()->GetPluginModel().get(), BlendHelper::PARAM::BLEND_MODE );

    SetPrevPlugin( prev );
}

// *************************************
// 
DefaultGradientPlugin::~DefaultGradientPlugin         ()
{
}

// *************************************
// 
IVertexAttributesChannelConstPtr    DefaultGradientPlugin::GetVertexAttributesChannel  () const
{
    return m_vaChannel;
}

// *************************************
// 
IPixelShaderChannelPtr              DefaultGradientPlugin::GetPixelShaderChannel       () const
{
    return m_psc;
}

// *************************************
// 
IVertexShaderChannelConstPtr        DefaultGradientPlugin::GetVertexShaderChannel      () const
{
    return m_vsc;
}

// *************************************
// 
void                                DefaultGradientPlugin::Update                      ( TimeType t )
{
    BasePlugin::Update( t );

    HelperVertexShaderChannel::InverseTextureMatrix( m_pluginParamValModel, DefaultGradientPlugin::TX_MAT );

	BlendHelper::UpdateBlendState( m_psc, m_blendEnabled, m_blendMode );
    
    HelperVertexAttributesChannel::PropagateAttributesUpdate( m_vaChannel, m_prevPlugin );
    if( HelperVertexAttributesChannel::PropagateTopologyUpdate( m_vaChannel, m_prevPlugin ) )
    {
        InitVertexAttributesChannel();
    }

    HelperPixelShaderChannel::PropagateUpdate( m_psc, m_prevPlugin );

    m_vsc->PostUpdate();
    m_psc->PostUpdate();
}

// *************************************
//
void								DefaultGradientPlugin::InitVertexAttributesChannel	()
{
    if( !( m_prevPlugin && m_prevPlugin->GetVertexAttributesChannel() ) )
    {
        m_vaChannel = nullptr;
        return;
    }

    auto prevGeomChannel = m_prevPlugin->GetVertexAttributesChannel();
    
    //add gradient texture desc
    VertexAttributesChannelDescriptor vaChannelDesc( *static_cast<const VertexAttributesChannelDescriptor *>( prevGeomChannel->GetDescriptor() ) );
    vaChannelDesc.AddAttrChannelDesc( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );
    
    if( !m_vaChannel )
    {
        m_vaChannel = std::make_shared< VertexAttributesChannel >( prevGeomChannel->GetPrimitiveType(), vaChannelDesc, true, prevGeomChannel->IsTimeInvariant() );
    }
    else
    {
        m_vaChannel->ClearAll();
        m_vaChannel->SetDescriptor( vaChannelDesc );
    }

    auto desc = std::make_shared< AttributeChannelDescriptor >( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR ); // TOCHECK is that right?
    auto prevCC = prevGeomChannel->GetComponents();
    for( unsigned int i = 0; i < prevCC.size(); ++i )
    {
        auto connComp = ConnectedComponent::Create();
        auto prevConnComp = std::static_pointer_cast< const model::ConnectedComponent >( prevCC[ i ] );
        auto prevCompChannels = prevConnComp->GetAttributeChannelsPtr();

        for( auto prevCompCh : prevCompChannels )
        {
            connComp->AddAttributeChannel( prevCompCh );
        }

        //add gradient uv channel
        auto posChannel = prevConnComp->GetAttrChannel( AttributeSemantic::AS_POSITION );
        if( posChannel /*&& !prevConnComp->GetAttrChannel( AttributeSemantic::AS_TEXCOORD )*/ )
        {
            // Note: We generate uvs even if they exist. This is necessary in case of text plugin, but
            // other plugins could use it's own UVs.

            //FIXME: only one texture - convex hull calculations
            auto uvs = new model::Float2AttributeChannel( desc, DefaultGradientPluginDesc::TextureName(), true );
            auto uvsPtr = Float2AttributeChannelPtr( uvs );
            
            Helper::UVGenerator::GenerateUV( std::static_pointer_cast< Float3AttributeChannel >( posChannel ),
                                             uvsPtr, glm::vec3( 1.0, 0.0, 0.0 ), glm::vec3( 0.0, 1.0, 0.0 ), true );

            connComp->AddAttributeChannel( uvsPtr );
        }


        m_vaChannel->AddConnectedComponent( connComp );
    }
}


} // model
} // bv
