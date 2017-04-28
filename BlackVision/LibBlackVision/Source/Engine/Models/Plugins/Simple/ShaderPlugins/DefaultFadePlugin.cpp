#include "stdafx.h"

#include "DefaultFadePlugin.h"

#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine/Interfaces/IValue.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"

#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/HelperVertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/HelperVertexAttributesChannel.h"

#include "Engine/Models/Plugins/HelperUVGenerator.h"
#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"

#include "Assets/DefaultAssets.h"




#include "Memory/MemoryLeaks.h"



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
{}

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

    helper.SetOrCreatePluginModel();

    helper.SetOrCreatePSModel();
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

// ***********************
//
std::string             DefaultFadePluginDesc::TextureName               ()
{
    return "Fade0";
}

// ************************************************************************* PLUGIN *************************************************************************

// *************************************
// 
bool DefaultFadePlugin::SetPrevPlugin( IPluginPtr prev )
{
    if( BasePlugin::SetPrevPlugin( prev ) )
    {
        InitVertexAttributesChannel();
        HelperPixelShaderChannel::CloneRenderContext( m_psc, prev );
        return true;
    }
    else
        return false;

}

// *************************************
// 
DefaultFadePlugin::DefaultFadePlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
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
DefaultFadePlugin::~DefaultFadePlugin         ()
{}

// *************************************
// 
bool							DefaultFadePlugin::IsValid     () const
{
    return ( GetPrevPlugin()->IsValid() );
}

// *************************************
// 
bool                            DefaultFadePlugin::LoadResource  ( AssetDescConstPtr assetDescr )
{
    return false;
}

// *************************************
// 
IVertexAttributesChannelConstPtr    DefaultFadePlugin::GetVertexAttributesChannel  () const
{
    return m_vaChannel;
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
    HelperPixelShaderChannel::PropagateUpdate( m_psc, GetPrevPlugin() );

    HelperVertexAttributesChannel::PropagateAttributesUpdate( m_vaChannel, GetPrevPlugin() );
    if( HelperVertexAttributesChannel::PropagateTopologyUpdate( m_vaChannel, GetPrevPlugin() ) )
    {
        InitVertexAttributesChannel();
    }

    m_vsc->PostUpdate();
    m_psc->PostUpdate();    
}

// *************************************
//
void		                        DefaultFadePlugin::InitVertexAttributesChannel		()
{
    if( !( GetPrevPlugin() && GetPrevPlugin()->GetVertexAttributesChannel() ) )
    {
        m_vaChannel = nullptr;
        return;
    }

    auto prevGeomChannel = GetPrevPlugin()->GetVertexAttributesChannel();
    auto prevCC = prevGeomChannel->GetComponents();

    //Only one texture
    VertexAttributesChannelDescriptor vaChannelDesc( * static_cast< const VertexAttributesChannelDescriptor * >( prevGeomChannel->GetDescriptor() ) );
    if( !vaChannelDesc.GetAttrChannelDescriptor( AttributeSemantic::AS_TEXCOORD ) )
    {
        vaChannelDesc.AddAttrChannelDesc( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );
    }
    
    if( !m_vaChannel )
    {		
        m_vaChannel = std::make_shared< VertexAttributesChannel >( prevGeomChannel->GetPrimitiveType(), vaChannelDesc, true, prevGeomChannel->IsTimeInvariant() );
    }
    else
    {
        m_vaChannel->ClearAll();
        m_vaChannel->SetDescriptor( vaChannelDesc );
    }

    auto desc = std::make_shared< AttributeChannelDescriptor >( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_PROCESSOR );
    for( unsigned int i = 0; i < prevCC.size(); ++i )
    {
        auto connComp = ConnectedComponent::Create();

        auto prevConnComp = std::static_pointer_cast< const model::ConnectedComponent >( prevCC[ i ] );
        auto prevCompChannels = prevConnComp->GetAttributeChannelsPtr();

        for( auto prevCompCh : prevCompChannels )
        {
            connComp->AddAttributeChannel( prevCompCh );
        }

        auto posChannel = prevConnComp->GetAttrChannel( AttributeSemantic::AS_POSITION );
        if( posChannel && !prevConnComp->GetAttrChannel( AttributeSemantic::AS_TEXCOORD ) )
        {
            //FIXME: only one texture - convex hull calculations
            auto uvs = new model::Float2AttributeChannel( desc, DefaultFadePluginDesc::TextureName(), true );
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

