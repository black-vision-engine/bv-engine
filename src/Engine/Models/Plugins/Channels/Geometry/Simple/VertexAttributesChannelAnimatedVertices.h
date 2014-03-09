#pragma once

#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannel.h"


namespace bv { namespace model {

class ConnectedComponent;

class VertexAttributesChannelAnimatedVertices : public VertexAttributesChannel
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

                    VertexAttributesChannelAnimatedVertices     ( float w, float h, float speedX, float speedY, float cyclesX, float cyclesY );

public:

    virtual         ~VertexAttributesChannelAnimatedVertices    ();

    virtual void    Update                              ( TimeType t ) override;

    virtual bool    NeedsAttributesUpdate               () const override;

    static VertexAttributesChannelAnimatedVertices *    Create  ( float w, float h, float z, unsigned int numSegments, float speedX, float speedY, float cyclesX, float cyclesY, float sizeY, float sizeZ );

};

} //model
} //bv
