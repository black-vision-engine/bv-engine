#include "GeometryChannel.h"
#include "ConnectedComponent.h"

#include "Engine\Models\Plugins\Channels\Geometry\VertexAttributeChannel.h"

#include <cassert>

namespace bv { namespace model
{

// *********************************
//
GeometryChannel::GeometryChannel     ( PrimitiveType type )
    : m_primitiveType( type ) 
{
}

// *********************************
//
GeometryChannel::~GeometryChannel   ()
{}

// *********************************
//
void                                    GeometryChannel::Update              ( float t )
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
const IGeometryChannelDescriptor *      GeometryChannel::GetDescriptor       () const
{
    return &m_desc;
}

// *********************************
//
PrimitiveType                           GeometryChannel::GetPrimitiveType    () const
{
    return m_primitiveType;
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