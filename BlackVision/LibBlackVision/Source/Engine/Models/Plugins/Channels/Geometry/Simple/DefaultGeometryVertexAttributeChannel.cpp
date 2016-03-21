#include "stdafx.h"
#include "DefaultGeometryVertexAttributeChannel.h"

#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"


namespace bv { namespace model {


// *********************************
//
DefaultGeometryVertexAttributeChannel::DefaultGeometryVertexAttributeChannel    ( PrimitiveType type, bool isReadOnly, bool isTimeInvariant  ) 
    : VertexAttributesChannel ( type, isReadOnly, isTimeInvariant )
    , m_compVertDesc( nullptr )
    , m_compNormDesc( nullptr )
    , m_compUVDesc( nullptr )
{
}

// *********************************
//
void    DefaultGeometryVertexAttributeChannel::AddAttributeChannelDescriptorsIfNeeded ( IGeometryGenerator & generator_ )
{
    if( m_desc.GetNumVertexChannels() > 0 )
        return;

    m_compVertDesc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );
    m_compNormDesc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_NORMAL, ChannelRole::CR_GENERATOR );

    m_desc.AddAttrChannelDesc( m_compVertDesc );
    m_desc.AddAttrChannelDesc( m_compNormDesc );

    switch( generator_.GetType() )
    {
        case IGeometryGenerator::Type::GEOMETRY_ONLY:
            break;

        case IGeometryGenerator::Type::GEOMETRY_AND_UVS:
            m_compUVDesc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_GENERATOR ); // possibly unused
            m_desc.AddAttrChannelDesc( m_compUVDesc );
            break;
                
        default:
            assert( false );
    }
}

// *********************************
//
void    DefaultGeometryVertexAttributeChannel::GenerateNormals                        ( Float3AttributeChannelPtr vertChannel, Float3AttributeChannelPtr normalChannel )
{
    auto vertsNum = vertChannel->GetNumEntries();
    auto vertices = vertChannel->GetVertices();

    auto normals = std::vector< glm::vec3 >( vertsNum, glm::vec3( 0.0f ) );
        
    auto normal = glm::vec3( 0.0f );

    //FIXME: assumption that Float3AttributeChannelPtr stores triangle strips
    for( UInt32 i = 0; i < vertsNum - 2; i += 2 )
    {
        //first triangle
        normal = glm::cross( vertices[ i + 2 ] - vertices[ i ], vertices[ i + 1 ] - vertices[ i ] );
        normals[ i ] += normal;
        normals[ i + 1 ] += normal;
        normals[ i + 2 ] += normal;

        //second triangle
        if( i + 3 < vertsNum )
        {
            normal = glm::cross( vertices[ i + 3 ] - vertices[ i + 2 ], vertices[ i + 1 ] - vertices[ i + 2 ] );
            normals[ i + 2 ] += normal;
            normals[ i + 1 ] += normal;
            normals[ i + 3 ] += normal;
        }
    }

    for( UInt32 i = 0; i < vertsNum; ++i )
    {
        //FIXME:
        if( normals[ i ] != glm::vec3( 0.0f ) )
        {
            normals[ i ] = glm::normalize( normals[ i ] ); 
        }

        normalChannel->AddAttribute( normals[ i ] );
    }
}

// *********************************
//
void    DefaultGeometryVertexAttributeChannel::GenerateAndAddConnectedComponent       ( IGeometryGenerator & generator )
{
    ConnectedComponentPtr comp = ConnectedComponent::Create();

    AddAttributeChannelDescriptorsIfNeeded( generator );

    auto vertArrtF3 = std::make_shared< Float3AttributeChannel >( m_compVertDesc, m_compVertDesc->SuggestedDefaultName( 0 ), false );
    auto vertArrtNorm = std::make_shared< Float3AttributeChannel >( m_compNormDesc, m_compNormDesc->SuggestedDefaultName( 0 ), false ); // possibly unused
        

    switch( generator.GetType() )
    {
        case IGeometryGenerator::Type::GEOMETRY_ONLY:
            static_cast< IGeometryOnlyGenerator & >( generator ).GenerateGeometry( vertArrtF3 );

            GenerateNormals( vertArrtF3, vertArrtNorm );

            comp->AddAttributeChannel( vertArrtF3 );
            comp->AddAttributeChannel( vertArrtNorm );
                
            break;

        case IGeometryGenerator::Type::GEOMETRY_AND_UVS:
        {
            Float2AttributeChannelPtr vertArrtUV = std::make_shared< Float2AttributeChannel >( m_compUVDesc, m_compUVDesc->SuggestedDefaultName( 0 ), false ); // possibly unused
            
            static_cast< IGeometryAndUVsGenerator & >( generator ).GenerateGeometryAndUVs( vertArrtF3, vertArrtUV );

            GenerateNormals( vertArrtF3, vertArrtNorm );

            comp->AddAttributeChannel( vertArrtF3 );
            comp->AddAttributeChannel( vertArrtNorm );
            comp->AddAttributeChannel( vertArrtUV );

            break;
        }
        default:
            assert( false );
    }

    AddConnectedComponent( comp );
}

} //bv
} //model