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


// *******************************
// 
std::string             DefaultMeshPluginDesc::MeshName                  ()
{
    return "Mesh0";
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
    , m_meshAsset( nullptr )
{
    m_psc = DefaultPixelShaderChannel::Create( model->GetPixelShaderChannelModel() );
    
    m_vaChannel = std::make_shared< VertexAttributesChannel >( PrimitiveType::PT_TRIANGLES );

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
        auto mesh = LoadTypedAsset< MeshAsset >( meshAssetDescr );
        if( mesh )
        {
            m_meshAsset = mesh->GetChild( meshAssetDescr->GetGroupName() );

            if( m_meshAsset )
            {
                InitVertexAttributesChannel( meshAssetDescr->IsRecursive() );
                SetAsset( 0, LAsset( DefaultMeshPluginDesc::MeshName(), assetDescr, nullptr ) );
                return true;
            }
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
bool		DefaultMeshPlugin::InitVertexAttributesChannel		( bool recursive )
{
    if( m_meshAsset )
    {
        auto cc = ConnectedComponent::Create();

        auto posDesc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );
        auto normDesc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_NORMAL, ChannelRole::CR_GENERATOR );
        auto uvDesc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_GENERATOR );
        
        auto pos = std::make_shared< Float3AttributeChannel >( posDesc, posDesc->SuggestedDefaultName( 0 ), false );
        auto norm = std::make_shared< Float3AttributeChannel >( normDesc, normDesc->SuggestedDefaultName( 0 ), false );
        auto uv = std::make_shared< Float2AttributeChannel >( uvDesc, uvDesc->SuggestedDefaultName( 0 ), false );

        AddGeometry( m_meshAsset, pos, norm, uv, recursive );

        VertexAttributesChannelDescriptor vacDesc;

        if( pos->GetNumEntries() > 0 )
        {
            vacDesc.AddAttrChannelDesc( posDesc );
            cc->AddAttributeChannel( pos );
        }
        else
        {
            return false;
        }

        if( norm->GetNumEntries() > 0 )
        {
            vacDesc.AddAttrChannelDesc( normDesc );
            cc->AddAttributeChannel( norm );
        }

        if( uv->GetNumEntries() > 0 )
        {
            vacDesc.AddAttrChannelDesc( uvDesc );
            cc->AddAttributeChannel( uv );
        }

        m_vaChannel->ClearAll();
        m_vaChannel->SetDescriptor( vacDesc );

        m_vaChannel->AddConnectedComponent( cc );

        HelperVertexAttributesChannel::SetTopologyUpdate( m_vaChannel );
        
        return true;
    }

    return false;
}

// *************************************
//
void		DefaultMeshPlugin::AddGeometry		                ( MeshAssetConstPtr meshAsset, Float3AttributeChannelPtr posChannel, Float3AttributeChannelPtr normChannel, Float2AttributeChannelPtr uvChannel, bool recursive )
{
    posChannel->AddAttributes( meshAsset->GetGeometry()->positions );
    normChannel->AddAttributes( meshAsset->GetGeometry()->normals );
    uvChannel->AddAttributes( meshAsset->GetGeometry()->uvs );

    if( recursive )
    {
        for( UInt32 i = 0; i < meshAsset->NumChildren(); ++i )
        {
            AddGeometry( meshAsset->GetChild( i ), posChannel, normChannel, uvChannel, recursive );
        }
    }
}

} // model
} // bv
