#include "stdafx.h"
#include "DefaultGeometryVertexAttributeChannel.h"




#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {


// *********************************
//
DefaultGeometryVertexAttributeChannel::DefaultGeometryVertexAttributeChannel            ( PrimitiveType type, bool isReadOnly, bool isTimeInvariant  ) 
    : VertexAttributesChannel ( type, isReadOnly, isTimeInvariant )
    , m_compVertDesc( nullptr )
    , m_compNormDesc( nullptr )
    , m_compUVDesc( nullptr )
{
}

// *********************************
//
void    DefaultGeometryVertexAttributeChannel::AddAttributeChannelDescriptors   ( IGeometryGenerator & generator )
{
    if( m_desc.GetNumVertexChannels() > 0 )
    {
        return;
    }

    auto type = generator.GetType();

    // always generate positions
    m_compVertDesc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );
    m_desc.AddAttrChannelDesc( m_compVertDesc );

    // add normals
    if( type >= IGeometryGenerator::GEOMETRY_NORMALS )
    {
        m_compNormDesc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_NORMAL, ChannelRole::CR_GENERATOR );
        m_desc.AddAttrChannelDesc( m_compNormDesc );
    }

    // add uvs
    if( type >= IGeometryGenerator::GEOMETRY_NORMALS_UVS )
    {
        m_compUVDesc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_GENERATOR ); // possibly unused
        m_desc.AddAttrChannelDesc( m_compUVDesc );
    }
}

// *********************************
//
void    DefaultGeometryVertexAttributeChannel::GenerateAndAddConnectedComponent       ( IGeometryGenerator & generator )
{
    auto comp = ConnectedComponent::Create();

    AddAttributeChannelDescriptors( generator );
    
    auto vertChannel = std::make_shared< Float3AttributeChannel >( m_compVertDesc, m_compVertDesc->SuggestedDefaultName( 0 ), false );
    comp->AddAttributeChannel( vertChannel );
    
    Float3AttributeChannelPtr normChannel = nullptr;
    if( m_compNormDesc )
    {
        normChannel = std::make_shared< Float3AttributeChannel >( m_compNormDesc, m_compNormDesc->SuggestedDefaultName( 0 ), false ); // possibly unused
        comp->AddAttributeChannel( normChannel );
    }

    Float2AttributeChannelPtr uvChannel = nullptr;
    if( m_compUVDesc )
    {
        uvChannel = std::make_shared< Float2AttributeChannel >( m_compUVDesc, m_compUVDesc->SuggestedDefaultName( 0 ), false ); // possibly unused
        comp->AddAttributeChannel( uvChannel );
    }
 
    switch( generator.GetType() )
    {
        case IGeometryGenerator::GEOMETRY_ONLY:
            static_cast< IGeometryOnlyGenerator & >( generator ).GenerateGeometry( vertChannel );
            break;
        case IGeometryGenerator::GEOMETRY_NORMALS:
            static_cast< IGeometryNormalsGenerator & >( generator ).GenerateGeometryNormals( vertChannel, normChannel );
            break;
        case IGeometryGenerator::GEOMETRY_NORMALS_UVS:
            static_cast< IGeometryNormalsUVsGenerator & >( generator ).GenerateGeometryNormalsUVs( vertChannel, normChannel, uvChannel );
            break;
        default:
            assert( false );
    }

    AddConnectedComponent( comp );
}

} //bv
} //model