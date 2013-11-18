#include "GeometryChannel.h"

#include <cassert>

#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"

#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelDescriptor.h"


namespace bv { namespace model
{

// *********************************
//
GeometryChannel::GeometryChannel     ( PrimitiveType type, bool isReadOnly, bool isTimeInvariant )
    : m_primitiveType( type )
    , m_isReadOnly( isReadOnly )
    , m_isTimeInvariant( isTimeInvariant )
    , m_needsAttributesUpdate( false )
    , m_needsTopologyUpdate( false )
{
}

// *********************************
//
GeometryChannel::GeometryChannel     ( PrimitiveType type, const GeometryChannelDescriptor& desc, bool isReadOnly, bool isTimeInvariant )
    : m_primitiveType( type )
    , m_desc( desc )
    , m_isReadOnly( isReadOnly )
    , m_isTimeInvariant( isTimeInvariant )
    , m_needsAttributesUpdate( false )
    , m_needsTopologyUpdate( false )
{
}

// *********************************
//
GeometryChannel::~GeometryChannel   ()
{
    for( auto cc : m_connectedComponents )
    {
        delete cc;
    }
}

// *********************************
//
void                                    GeometryChannel::Update              ( TimeType t )
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
bool                                    GeometryChannel::IsReadOnly          () const
{
    return m_isReadOnly;
}

// *********************************
//
bool                                    GeometryChannel::IsTimeInvariant     () const
{
    return m_isTimeInvariant;
}

// *********************************
//
bool                                    GeometryChannel::NeedsAttributesUpdate( TimeType t ) const
{
    return m_needsAttributesUpdate;
}

// *********************************
//
bool                                    GeometryChannel::NeedsTopologyUpdate ( TimeType t ) const
{
    return m_needsTopologyUpdate;
}

// *********************************
//
void                                    GeometryChannel::SetNeedsAttributesUpdate( bool b )
{
    m_needsAttributesUpdate = b;
}

// *********************************
//
void                                    GeometryChannel::SetNeedsTopologyUpdate( bool b )
{
    m_needsTopologyUpdate = b;
}

// *********************************
//
unsigned int                            GeometryChannel::TotalNumVertices    ()          const
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
const IGeometryChannelDescriptor *      GeometryChannel::GetDescriptor       () const
{
    return &m_desc;
}

// *********************************
//
void                                    GeometryChannel::SetDescriptor       ( const GeometryChannelDescriptor & desc )
{
    m_desc = desc;
}

// *********************************
//
PrimitiveType                           GeometryChannel::GetPrimitiveType    () const
{
    return m_primitiveType;
}

void                                    GeometryChannel::AddConnectedComponent( IConnectedComponent * cc )
{
    auto attrChannel = cc->GetAttributeChannels();

    assert( !attrChannel.empty() );
    assert( attrChannel.size() == m_desc.GetNumVertexChannels() );

    int i = 0;

    for( auto attr : attrChannel )
    {
        assert( attr->GetDescriptor()->GetType() == m_desc.GetAttrChannelDescriptor( i++ )->GetType() );
    }

    m_connectedComponents.push_back( static_cast< ConnectedComponent * >( cc ) );
}

// *********************************
//
int                                     GeometryChannel::GetNumPrimitives   ( IConnectedComponent* connComp ) const
{
    int vertNum = connComp->GetNumVertices();

    switch(GetPrimitiveType())
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
std::vector< IConnectedComponent* >     GeometryChannel::GetComponents       () const
{
    return std::vector< IConnectedComponent* >( m_connectedComponents.begin(), m_connectedComponents.end() );
}

//// *********************************
////
//bool                                    GeometryChannel::CanBeConnectedTo    ( IPlugin * plugin ) const
//{
//    return CanBeConnectedTo( plugin->GetGeometryChannel() );
//}

// *********************************
//
bool                                    GeometryChannel::CanBeConnectedTo    ( IGeometryChannel * channel ) const
{
    return CanBeConnectedTo( *static_cast< const GeometryChannelDescriptor * >( channel->GetDescriptor() ) );
}

} // model
} // bv
