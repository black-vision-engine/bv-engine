#pragma once

#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"

namespace bv { namespace model {

class DefaultGeometryVertexAttributeChannel : public VertexAttributesChannel
{
private:

    AttributeChannelDescriptor * m_compDesc;

public:

    DefaultGeometryVertexAttributeChannel( PrimitiveType type, bool isReadOnly = false, bool isTimeInvariant = false ) 
        : VertexAttributesChannel ( type, isReadOnly, isTimeInvariant )  
    {
        AttributeChannelDescriptor * compDesc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );

        VertexAttributesChannelDescriptor vaDesc;

        vaDesc.AddAttrChannelDesc( static_cast< const AttributeChannelDescriptor * >( compDesc ) );

        m_compDesc = compDesc;

        m_desc = vaDesc;
    }

    void GenerateAndAddConnectedComponent( void (*GenerateConnectedComponent) (Float3AttributeChannelPtr) )
    {
        ConnectedComponentPtr comp = ConnectedComponent::Create();

        Float3AttributeChannelPtr vertArrtF3 = std::make_shared< Float3AttributeChannel >( m_compDesc, m_compDesc->SuggestedDefaultName( 0 ), false );

        GenerateConnectedComponent( vertArrtF3 );

        comp->AddAttributeChannel( vertArrtF3 );

        AddConnectedComponent( comp );
    }
};

class IGeometryGenerator
{
public:
    enum Type { GEOMETRY_ONLY, GEOMETRY_AND_UVS };

    virtual Type GetType() = 0;
};

class IGeometryOnlyGenerator : public IGeometryGenerator
{
public:
    
    virtual void GenerateGeometry( Float3AttributeChannelPtr ) = 0;
};

class IGeometryAndUVsGenerator : public IGeometryGenerator
{
public:
    
    virtual void GenerateGeometryAndUVs( Float3AttributeChannelPtr, Float2AttributeChannelPtr ) = 0;
};

class DefaultGeometryAndUVsVertexAttributeChannel : public VertexAttributesChannel
{
private:

    AttributeChannelDescriptor * m_compVertDesc;
    AttributeChannelDescriptor * m_compUVDesc;

public:

    DefaultGeometryAndUVsVertexAttributeChannel( PrimitiveType type, bool isReadOnly = false, bool isTimeInvariant = false ) 
        : VertexAttributesChannel ( type, isReadOnly, isTimeInvariant )  
    {
        VertexAttributesChannelDescriptor vaDesc;

        m_desc = vaDesc; // FIXME?
    }

    void AddAttributeChannelDescriptorsIfNeeded( IGeometryGenerator & generator_ )
    {
        if( m_desc.GetNumVertexChannels() > 0 )
            return;

        AttributeChannelDescriptor * compVertDesc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION, ChannelRole::CR_GENERATOR );
        AttributeChannelDescriptor * compUVDesc = new AttributeChannelDescriptor( AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD, ChannelRole::CR_GENERATOR ); // possibly unused

        m_compVertDesc = compVertDesc;
        m_compUVDesc = compUVDesc;

        switch( generator_.GetType() )
        {
            case IGeometryGenerator::Type::GEOMETRY_ONLY:
                m_desc.AddAttrChannelDesc( static_cast< const AttributeChannelDescriptor * >( compVertDesc ) );
                
                break;

            case IGeometryGenerator::Type::GEOMETRY_AND_UVS:
                m_desc.AddAttrChannelDesc( static_cast< const AttributeChannelDescriptor * >( compVertDesc ) );
                m_desc.AddAttrChannelDesc( static_cast< const AttributeChannelDescriptor * >( compUVDesc ) );

                break;
                
            default:
                assert( false );
        }
    }

    void GenerateAndAddConnectedComponent( IGeometryGenerator & generator_ )
    {
        ConnectedComponentPtr comp = ConnectedComponent::Create();

        AddAttributeChannelDescriptorsIfNeeded( generator_ );

        Float3AttributeChannelPtr vertArrtF3 = std::make_shared< Float3AttributeChannel >( m_compVertDesc, m_compVertDesc->SuggestedDefaultName( 0 ), false );
        Float2AttributeChannelPtr vertArrtUV = std::make_shared< Float2AttributeChannel >( m_compUVDesc, m_compUVDesc->SuggestedDefaultName( 0 ), false ); // possibly unused

        switch( generator_.GetType() )
        {
            case IGeometryGenerator::Type::GEOMETRY_ONLY:
                {IGeometryOnlyGenerator& generator = reinterpret_cast< IGeometryOnlyGenerator& >( generator_ );
                generator.GenerateGeometry( vertArrtF3 );}
                
                comp->AddAttributeChannel( vertArrtF3 );
                
                break;

            case IGeometryGenerator::Type::GEOMETRY_AND_UVS:
                {IGeometryAndUVsGenerator& generator = reinterpret_cast< IGeometryAndUVsGenerator& >( generator_ );
                generator.GenerateGeometryAndUVs( vertArrtF3, vertArrtUV );}

                comp->AddAttributeChannel( vertArrtF3 );
                comp->AddAttributeChannel( vertArrtUV );

                break;
                
            default:
                assert( false );
        }


        AddConnectedComponent( comp );
    }
};

} //model
} //bv