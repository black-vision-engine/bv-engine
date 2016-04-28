#include "stdafx.h"

#include "DefaultMeshPlugin.h"

#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"
#include "Engine/Models/Plugins/Channels/Geometry/HelperVertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/HelperVertexShaderChannel.h"

#include "Assets/Mesh/MeshAssetDescriptor.h"


namespace bv { namespace model {



// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultMeshPluginDesc::DefaultMeshPluginDesc                          ()
    : BasePluginDescriptor( UID(), "mesh" )
{
}

// *******************************
//
IPluginPtr              DefaultMeshPluginDesc::CreatePlugin              ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultMeshPlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   DefaultMeshPluginDesc::CreateDefaultModel( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper helper( timeEvaluator );
    helper.CreatePSModel();
    return helper.GetModel();
}

// *******************************
//
std::string             DefaultMeshPluginDesc::UID                       ()
{
    return "DEFAULT_MESH";
}


// ************************************************************************* PLUGIN *************************************************************************

// *************************************
// 
void DefaultMeshPlugin::SetPrevPlugin( IPluginPtr prev )
{
    BasePlugin::SetPrevPlugin( prev );

    HelperPixelShaderChannel::CloneRenderContext( m_psc, prev );
    auto ctx = m_psc->GetRendererContext();
    ctx->cullCtx->enabled = false;
}

// *************************************
// 
DefaultMeshPlugin::DefaultMeshPlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin< IPlugin >( name, uid, prev, model )
    , m_psc( nullptr )
    , m_vaChannel( nullptr )
    , m_meshAsset( nullptr )
{
    m_psc = DefaultPixelShaderChannel::Create( model->GetPixelShaderChannelModel() );

    SetPrevPlugin( prev );
}

// *************************************
// 
DefaultMeshPlugin::~DefaultMeshPlugin         ()
{
}

// *************************************
// 
bool							DefaultMeshPlugin::IsValid     () const
{
    return ( m_vaChannel && m_prevPlugin->IsValid() );
}

// *************************************
// 
bool                            DefaultMeshPlugin::LoadResource  ( AssetDescConstPtr assetDescr )
{
    m_meshAsset = nullptr;

    auto meshAssetDescr = QueryTypedDesc< MeshAssetDescConstPtr >( assetDescr );
    
    if ( meshAssetDescr )
    {
        m_meshAsset = LoadTypedAsset< MeshAsset >( meshAssetDescr );

        if( m_meshAsset )
        {
            InitVertexAttributesChannel();
            return true;
        }

    }
    return false;
}

// *************************************
// 
IVertexAttributesChannelConstPtr    DefaultMeshPlugin::GetVertexAttributesChannel  () const
{
    return m_vaChannel;
}

// *************************************
// 
IPixelShaderChannelPtr              DefaultMeshPlugin::GetPixelShaderChannel       () const
{
    return m_psc;
}

// *************************************
// 
void                                DefaultMeshPlugin::Update                      ( TimeType t )
{
    BasePlugin::Update( t );
    m_psc->PostUpdate();    
}

// *************************************
//
void		DefaultMeshPlugin::InitVertexAttributesChannel		()
{
    m_vaChannel = nullptr;

    if( m_meshAsset )
    {
        auto cc = ConnectedComponent::Create();

        auto posDesc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );
        auto pos = std::make_shared< Float3AttributeChannel >( posDesc, posDesc->SuggestedDefaultName( 0 ), false );

        AddGeometry( m_meshAsset, pos );

        VertexAttributesChannelDescriptor vacDesc;
        vacDesc.AddAttrChannelDesc( posDesc );

        m_vaChannel = std::make_shared< VertexAttributesChannel >( PrimitiveType::PT_TRIANGLES, vacDesc );

        cc->AddAttributeChannel( pos );

        m_vaChannel->AddConnectedComponent( cc );
    }
}

// *************************************
//
void		DefaultMeshPlugin::AddGeometry		                ( MeshAssetConstPtr meshAsset, Float3AttributeChannelPtr posChannel )
{
    posChannel->AddAttributes( meshAsset->GetGeometry()->positions );
    for( UInt32 i = 0; i < meshAsset->NumChildren(); ++i )
    {
        AddGeometry( meshAsset->GetChild( i ), posChannel );
    }
}

} // model
} // bv
