#include "stdafx.h"

#include "VertexAttributesChannel.h"

#include <cassert>

#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"

#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"


namespace bv { namespace model
{

// *********************************
//
VertexAttributesChannel::VertexAttributesChannel     ( PrimitiveType type, bool isReadOnly, bool isTimeInvariant )
    : m_primitiveType( type )
    , m_isReadOnly( isReadOnly )
    , m_isTimeInvariant( isTimeInvariant )
    , m_attributesUpdateID( 0 )
    , m_topologyUpdateID( 0 )
{
}

// *********************************
//
VertexAttributesChannel::VertexAttributesChannel     ( PrimitiveType type, const VertexAttributesChannelDescriptor & desc, bool isReadOnly, bool isTimeInvariant )
    : m_primitiveType( type )
    , m_desc( desc )
    , m_isReadOnly( isReadOnly )
    , m_isTimeInvariant( isTimeInvariant )
    , m_attributesUpdateID( 0 )
    , m_topologyUpdateID( 0 )
{
}

// *********************************
//
VertexAttributesChannel::~VertexAttributesChannel   ()
{
}

// *********************************
//
void                                    VertexAttributesChannel::Update                 ( TimeType t )
{
    //FIXME: not necesarrily like this but it has to be thought over
    //FIXME: it is best to assume that Update is called from the first to last plugin and state is well defined and each plugin just has to read predcessor's state (without calling Update)
    //FIXME: and only calculate its state accordingly
    for( auto comp : m_connectedComponents )
    {
        comp->Update( t );
    }
}


// *********************************
//
bool                                    VertexAttributesChannel::IsReadOnly             () const
{
    return m_isReadOnly;
}

// *********************************
//
bool                                    VertexAttributesChannel::IsTimeInvariant        () const
{
    return m_isTimeInvariant;
}

// *********************************
//
UInt64                                VertexAttributesChannel::GetAttributesUpdateID	() const
{
    return m_attributesUpdateID;
}

// *********************************
//
UInt64                                VertexAttributesChannel::GetTopologyUpdateID		() const
{
    return m_topologyUpdateID;
}

// *********************************
//
void                                VertexAttributesChannel::SetAttributesUpdateID	( UInt64 updateID )
{
    m_attributesUpdateID = updateID;
}

// *********************************
//
void                                VertexAttributesChannel::SetTopologyUpdateID	( UInt64 updateID )
{
    m_topologyUpdateID = updateID;
}

// ***********************
//
void                                                VertexAttributesChannel::UpdateBoundingBox       ()
{
    m_boundingBox.m_empty = true;

    for( auto comp : GetComponents() )
    {
        for( auto channel : comp->GetAttributeChannels() )
        {
            auto desc = channel->GetDescriptor();
            if( desc->GetSemantic() == AttributeSemantic::AS_POSITION )
            {
                assert( desc->GetType() == AttributeType::AT_FLOAT3 );
                
                const glm::vec3 * data = reinterpret_cast< const glm::vec3 * >( channel->GetData() );

                for( UInt32 i = 0; i < channel->GetNumEntries(); i++ )
                    m_boundingBox.Include( data[ i ] );
            }
        }
    }
}

// ***********************
//
mathematics::Box                                    VertexAttributesChannel::GetBoundingBox          () const
{
    return m_boundingBox;
}

// *********************************
//
unsigned int                            VertexAttributesChannel::TotalNumVertices       ()          const
{
    unsigned int total = 0;

    for( auto cc : m_connectedComponents )
    {
        total += cc->GetNumVertices();
    }

    return total;
}

// *********************************
//
const IVertexAttributesChannelDescriptor *      VertexAttributesChannel::GetDescriptor  () const
{
    return &m_desc;
}

// *********************************
//
void                                    VertexAttributesChannel::SetDescriptor          ( const VertexAttributesChannelDescriptor & desc )
{
    m_desc = desc;
}

// *********************************
//
PrimitiveType                           VertexAttributesChannel::GetPrimitiveType       () const
{
    return m_primitiveType;
}

// *********************************
//
void                                    VertexAttributesChannel::AddConnectedComponent  ( ConnectedComponentPtr cc )
{
    auto attrChannel = cc->GetAttributeChannels();

    assert( !attrChannel.empty() );
    assert( attrChannel.size() == m_desc.GetNumVertexChannels() );

    int i = 0;
    { i; } // FIXME: suppress unused warning

    for( auto attr : attrChannel )
    {
        assert( attr->GetDescriptor()->GetType() == m_desc.GetAttrChannelDescriptor( i++ )->GetType() );
    }

    m_connectedComponents.push_back( cc );
}

// *********************************
//
int                                     VertexAttributesChannel::GetNumPrimitives   ( IConnectedComponentPtr connComp ) const
{
    int vertNum = connComp->GetNumVertices();

    switch( GetPrimitiveType() )
    {
        case PrimitiveType::PT_POINTS:
            return vertNum;
        case PrimitiveType::PT_LINES:
            assert(vertNum % 2 == 0);
            return vertNum / 2;
        case PrimitiveType::PT_LINE_STRIP:
            return vertNum - 1;
        case PrimitiveType::PT_LINE_LOOP:
            return vertNum;
        case PrimitiveType::PT_TRIANGLES:
            assert(vertNum % 3 == 0);
            return vertNum / 3;
        case PrimitiveType::PT_TRIANGLE_STRIP:
            return vertNum - 2;
        case PrimitiveType::PT_TRIANGLE_FAN:
            return vertNum - 2;
        case PrimitiveType::PT_QUADS:
            assert(vertNum % 4 == 0);
            return vertNum / 4;
        case PrimitiveType::PT_QUAD_STRIP:
            return vertNum - 3;
        case PrimitiveType::PT_POLYGON:
            return 1;
        default:
            assert( false );
            return 0;
    }
}

// *********************************
//
std::vector< IConnectedComponentPtr >     VertexAttributesChannel::GetComponents       () const
{
    return std::vector< IConnectedComponentPtr >( m_connectedComponents.begin(), m_connectedComponents.end() );
}

//// *********************************
////
//bool                                    VertexAttributesChannel::CanBeConnectedTo    ( IPlugin * plugin ) const
//{
//    return CanBeConnectedTo( plugin->GetVertexAttributesChannel() );
//}

// *********************************
//
bool                                    VertexAttributesChannel::CanBeConnectedTo    ( IVertexAttributesChannelPtr channel ) const
{
    return CanBeConnectedTo( *static_cast< const VertexAttributesChannelDescriptor * >( channel->GetDescriptor() ) );
}

// *********************************
//
ConnectedComponentPtr                   VertexAttributesChannel::GetConnectedComponent   ( unsigned int idx )
{
    assert( idx < m_connectedComponents.size() );

    return m_connectedComponents[ idx ];
}

// *********************************
//
void                                    VertexAttributesChannel::ClearAll                ()
{
    m_connectedComponents.clear();
}

// *********************************
//
void                                    VertexAttributesChannel::Initialize              ( PrimitiveType type, const VertexAttributesChannelDescriptor& desc, bool isReadOnly, bool isTimeInvariant )
{
    m_primitiveType = type;
    m_desc = desc;
    m_isReadOnly = isReadOnly;
    m_isTimeInvariant = isTimeInvariant;
}

} // model
} // bv
