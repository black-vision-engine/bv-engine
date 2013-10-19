#include "GeometryChannelAnimatedVertices.h"

#include "Engine\Models\Plugins\Channels\Geometry\ConnectedComponent.h"

namespace bv { namespace model {

// ******************************
//
GeometryChannelAnimatedVertices::GeometryChannelAnimatedVertices     ( GeometryChannelDescriptor & desc )
    : GeometryChannel( PrimitiveType::PT_TRIANGLE_STRIP, desc )
{
}

// ******************************
//
GeometryChannelAnimatedVertices::~GeometryChannelAnimatedVertices    ()
{
}

// ******************************
//
bool    GeometryChannelAnimatedVertices::NeedsPositionsUpdate        ( float t ) const
{
    return true;
}

// ******************************
//
GeometryChannelAnimatedVertices *   GeometryChannelAnimatedVertices::Create  ()
{
    GeometryChannelDescriptor desc;
//    desc.AddVertexAttrChannelDesc( desc );
    return nullptr;
}

} //model
} //bv
