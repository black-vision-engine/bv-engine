#pragma once

#include "Engine\Models\Plugins\Channels\Geometry\GeometryChannel.h"

namespace bv { namespace model {

class ConnectedComponent;

class GeometryChannelAnimatedVertices : public GeometryChannel
{
private:

    ConnectedComponent  * m_reprCC;

private:

                    GeometryChannelAnimatedVertices     ();

public:

    virtual         ~GeometryChannelAnimatedVertices    ();

    virtual bool    NeedsPositionsUpdate                ( float t ) const;

    static GeometryChannelAnimatedVertices *    Create  ( float w, float h, float z, unsigned int numSegments );

};

} //model
} //bv
