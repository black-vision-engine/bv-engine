#pragma once

#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h"


namespace bv { namespace model {

class ConnectedComponent;

class GeometryChannelAnimatedVertices : public VertexAttributesChannel
{
private:

    ConnectedComponent  * m_reprCC;

    float   m_w;
    float   m_h;

    float   m_speedX;
    float   m_speedY;
    float   m_cyclesX;
    float   m_cyclesY;

private:

                    GeometryChannelAnimatedVertices     ( float w, float h, float speedX, float speedY, float cyclesX, float cyclesY );

public:

    virtual         ~GeometryChannelAnimatedVertices    ();

    virtual void    Update                              ( TimeType t );

    virtual bool    NeedsAttributesUpdate               ( TimeType t ) const;

    static GeometryChannelAnimatedVertices *    Create  ( float w, float h, float z, unsigned int numSegments, float speedX, float speedY, float cyclesX, float cyclesY, float sizeY, float sizeZ );

};

} //model
} //bv
