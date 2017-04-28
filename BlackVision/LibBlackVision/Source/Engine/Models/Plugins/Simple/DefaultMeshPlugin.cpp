#include "stdafx.h"

#include "DefaultMeshPlugin.h"

#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"
#include "Engine/Models/Plugins/Channels/Geometry/HelperVertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/HelperVertexShaderChannel.h"
#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"

#include "Assets/Mesh/MeshAssetDescriptor.h"

#include "Assets/SVG/SVGAssetDescriptor.h"
#include "Assets/SVG/SVGAsset.h"
#include "Assets/Asset.h"


#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>


#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {


const std::string        DefaultMeshPlugin::PARAMS::FlipU    = "FlipU";
const std::string        DefaultMeshPlugin::PARAMS::FlipV    = "FlipV";


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
    helper.SetOrCreatePSModel();

    helper.AddSimpleStatedParam( DefaultMeshPlugin::PARAMS::FlipU, false );
    helper.AddSimpleStatedParam( DefaultMeshPlugin::PARAMS::FlipV, false );

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
    : BasePlugin( name, uid, prev, model )
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
    return ( m_vaChannel && GetPrevPlugin()->IsValid() );
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

    if( ParameterChanged( PARAMS::FlipU ) ||
        ParameterChanged( PARAMS::FlipV ) )
    {
        InitVertexAttributesChannel( true );
    }

    m_psc->PostUpdate();    
}

// *************************************
//
bool		DefaultMeshPlugin::InitVertexAttributesChannel		( bool recursive )
{
    if( m_meshAsset )
    {
        auto cc = ConnectedComponent::Create();

        auto posDesc = std::make_shared< AttributeChannelDescriptor >( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );
        auto normDesc = std::make_shared< AttributeChannelDescriptor >( AttributeType::AT_FLOAT3, AttributeSemantic::AS_NORMAL, ChannelRole::CR_GENERATOR );
        auto uvDesc = std::make_shared< AttributeChannelDescriptor >( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_GENERATOR );
        auto tangentDesc = std::make_shared< AttributeChannelDescriptor >( AttributeType::AT_FLOAT4, AttributeSemantic::AS_TANGENT, ChannelRole::CR_GENERATOR );
        
        auto pos = std::make_shared< Float3AttributeChannel >( posDesc, posDesc->SuggestedDefaultName( 0 ), false );
        auto norm = std::make_shared< Float3AttributeChannel >( normDesc, normDesc->SuggestedDefaultName( 0 ), false );
        auto uv = std::make_shared< Float2AttributeChannel >( uvDesc, uvDesc->SuggestedDefaultName( 0 ), false );
        auto tangent = std::make_shared< Float4AttributeChannel >( tangentDesc, tangentDesc->SuggestedDefaultName( 0 ), false );

        glm::mat4 identity = glm::mat4( 1 );
        AddGeometry( m_meshAsset, pos, norm, uv, tangent, identity, recursive );

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
            HelperVertexAttributesChannel::SetAttributesUpdate( m_vaChannel );

            return true;
        }
    }

    return false;
}

// *************************************
//
void		DefaultMeshPlugin::AddGeometry		                ( MeshAssetConstPtr meshAsset, Float3AttributeChannelPtr posChannel, Float3AttributeChannelPtr normChannel, Float2AttributeChannelPtr uvChannel, Float4AttributeChannelPtr tangentChannel, glm::mat4 & transform, bool recursive )
{
    bool flipU = QueryTypedParam< ParamBoolPtr >( GetParameter( PARAMS::FlipU ) )->Evaluate();
    bool flipV = QueryTypedParam< ParamBoolPtr >( GetParameter( PARAMS::FlipV ) )->Evaluate();

    auto geometry = meshAsset->GetGeometry();
    if( geometry )
    {
        if( transform == glm::mat4( 1.0 ) )
        {
            posChannel->AddAttributes( geometry->positions );
            normChannel->AddAttributes( geometry->normals );
            AddUVChannel( uvChannel, geometry->uvs, flipU, flipV );
            tangentChannel->AddAttributes( geometry->tangents );
        }
        else
        {
            glm::mat4 normalMat = glm::transpose( glm::inverse( transform ) );

            posChannel->GetVertices().reserve( posChannel->GetVertices().size() + geometry->positions.size() );
            normChannel->GetVertices().reserve( normChannel->GetVertices().size() + geometry->positions.size() );
            tangentChannel->GetVertices().reserve( tangentChannel->GetVertices().size() + geometry->positions.size() );

            
            for( int i = 0; i < geometry->positions.size(); ++i )
                posChannel->AddAttribute( glm::vec3( transform * glm::vec4( geometry->positions[ i ], 1.0 ) ) );

            for( int i = 0; i < geometry->positions.size(); ++i )
                normChannel->AddAttribute( glm::vec3( normalMat * glm::vec4( geometry->normals[ i ], 0.0 ) ) );

            for( int i = 0; i < geometry->positions.size(); ++i )
                tangentChannel->AddAttribute( normalMat * geometry->tangents[ i ] );

            AddUVChannel( uvChannel, geometry->uvs, flipU, flipV );
        }
    }

    if( recursive )
    {
        for( UInt32 i = 0; i < meshAsset->NumChildren(); ++i )
        {
            glm::mat4 childTransform = transform * ComputeTransform( meshAsset->GetChild( i ) );
            AddGeometry( meshAsset->GetChild( i ), posChannel, normChannel, uvChannel, tangentChannel, childTransform, recursive );
        }
    }
}

// ***********************
//
glm::mat4   DefaultMeshPlugin::ComputeTransform                 ( MeshAssetConstPtr meshAsset )
{
    auto transform = meshAsset->GetTransform();
    
    if( transform )
    {
        return transform->transform;

        //glm::mat4 translate = glm::translate( transform->translation );
        //glm::mat4 rotate = glm::yawPitchRoll( transform->rotation.x, transform->rotation.y, transform->rotation.z );
        //glm::mat4 scale = glm::scale( transform->scale );

        //return translate * glm::translate( transform->center ) * rotate * scale * glm::translate( -transform->center );
    }
    else
        return glm::mat4( 1.0 );
}

// ***********************
//
void        DefaultMeshPlugin::AddUVChannel                     ( Float2AttributeChannelPtr & uvChannel, const std::vector< glm::vec2 > & uvs, bool flipU, bool flipV )
{
    if( !flipU && !flipV )
    {
        uvChannel->AddAttributes( uvs );
    }
    else
    {
        uvChannel->GetVertices().reserve( uvChannel->GetVertices().size() + uvs.size() );

        for( int i = 0; i < uvs.size(); ++i )
        {
            glm::vec2 newUV = uvs[ i ];
            
            if( flipU )
                newUV.x = 1.0f - newUV.x;
            if( flipV )
                newUV.y = 1.0f - newUV.y;
            
            uvChannel->AddAttribute( newUV );
        }
    }
}




} // model
} // bv
