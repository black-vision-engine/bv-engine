#pragma once

#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"

namespace bv { namespace model {

class Float3VertexAttributeChannel;

class VariableTopologyStripComponent: public ConnectedComponent
{
private:

    float   m_size;
    float   m_speed;
    float   m_oscilationSpeed;
    float   m_segmentDeltaTime;
    float   m_activeDuration;

    int     numSegments;
    int     m_activeSegment;
    bool    m_topologyChanged;

    float   m_startX;
    float   m_startY;
    float   m_posZ;


    Float3VertexAttributeChannel * m_positions;

private:

                        VariableTopologyStripComponent  ( float size, float speed, float duration, int numSegments, float oscilationSpeed, float startX, float startY, float posZ );
                        ~VariableTopologyStripComponent ();

public:

    bool                IsActive                        ( float t ) const;
    bool                TopologyChanged                 ( float t ) const;

    virtual void        Update                          ( float t );


    static  VariableTopologyStripComponent *    Create  ( float size, float speed, float duration, int numSegments, float oscilationSpeed, float startX, float startY, float posZ );

};

} //model
} //bv
