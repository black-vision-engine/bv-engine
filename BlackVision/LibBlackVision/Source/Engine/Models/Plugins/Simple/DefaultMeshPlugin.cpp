#include "stdafx.h"

#include "DefaultMeshPlugin.h"

#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"
#include "Engine/Models/Plugins/Channels/Geometry/HelperVertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/HelperVertexShaderChannel.h"

#include "Assets/Mesh/MeshAssetDescriptor.h"

#include "Assets/SVG/SVGAssetDescriptor.h"
#include "Assets/SVG/SVGAsset.h"
#include "Assets/Asset.h"



#include "Memory/MemoryLeaks.h"



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


    auto svgAssetDescr = QueryTypedDesc< SVGAssetDescriptorConstPtr >( assetDescr );

    if( svgAssetDescr )
    {
        auto mesh = LoadTypedAsset< SVGAsset >( svgAssetDescr );
        if( mesh )
        {
            m_vaChannel = std::make_shared< VertexAttributesChannel >( PrimitiveType::PT_LINES );

//            m_meshAsset = QueryTypedRes< SVGAssetConstPtr >( mesh ); //mesh->GetChild( svgAssetDescr->GetGroupName() );
            m_meshAsset = std::dynamic_pointer_cast< const SVGAsset >( mesh );

            if( m_meshAsset )
            {
                InitVertexAttributesChannel( true /*meshAssetDescr->IsRecursive()*/ );
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
        auto tangentDesc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT4, AttributeSemantic::AS_TANGENT, ChannelRole::CR_GENERATOR );
        
        auto pos = std::make_shared< Float3AttributeChannel >( posDesc, posDesc->SuggestedDefaultName( 0 ), false );
        auto norm = std::make_shared< Float3AttributeChannel >( normDesc, normDesc->SuggestedDefaultName( 0 ), false );
        auto uv = std::make_shared< Float2AttributeChannel >( uvDesc, uvDesc->SuggestedDefaultName( 0 ), false );
        auto tangent = std::make_shared< Float4AttributeChannel >( tangentDesc, tangentDesc->SuggestedDefaultName( 0 ), false );

        AddGeometry( m_meshAsset, pos, norm, uv, tangent, recursive );

        VertexAttributesChannelDescriptor vacDesc;

        if( pos->GetNumEntries() > 0 )
        {
            vacDesc.AddAttrChannelDesc( std::dynamic_pointer_cast< const AttributeChannelDescriptor >( pos->GetDescriptor() ) );
            cc->AddAttributeChannel( pos );

            assert( norm->GetNumEntries() > 0 );
            assert( uv->GetNumEntries() > 0 );
            assert( tangent->GetNumEntries() > 0 );

            vacDesc.AddAttrChannelDesc( std::dynamic_pointer_cast< const AttributeChannelDescriptor >( norm->GetDescriptor() ) );
            cc->AddAttributeChannel( norm );

            vacDesc.AddAttrChannelDesc( std::dynamic_pointer_cast< const AttributeChannelDescriptor >( uv->GetDescriptor() ) );
            cc->AddAttributeChannel( uv );

            vacDesc.AddAttrChannelDesc( std::dynamic_pointer_cast< const AttributeChannelDescriptor >( tangent->GetDescriptor() ) );
            cc->AddAttributeChannel( tangent );

            m_vaChannel->ClearAll();
            m_vaChannel->SetDescriptor( vacDesc );

            m_vaChannel->AddConnectedComponent( cc );

            HelperVertexAttributesChannel::SetTopologyUpdate( m_vaChannel );

            return true;
        }
    }

    return false;
}

// *************************************
//
void		DefaultMeshPlugin::AddGeometry		                ( MeshAssetConstPtr meshAsset, Float3AttributeChannelPtr posChannel, Float3AttributeChannelPtr normChannel, Float2AttributeChannelPtr uvChannel, Float4AttributeChannelPtr tangentChannel, bool recursive )
{
    auto geometry = meshAsset->GetGeometry();
    if( geometry )
    {
        posChannel->AddAttributes( geometry->positions );
        normChannel->AddAttributes( geometry->normals );
        uvChannel->AddAttributes( geometry->uvs );
        tangentChannel->AddAttributes( geometry->tangents );
    }

    if( recursive )
    {
        for( UInt32 i = 0; i < meshAsset->NumChildren(); ++i )
        {
            AddGeometry( meshAsset->GetChild( i ), posChannel, normChannel, uvChannel, tangentChannel, /*binormalChannel,*/ recursive );
        }
    }
}

} // model
} // bv
