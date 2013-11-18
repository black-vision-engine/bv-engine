#pragma once

#include <glm/glm.hpp>

#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"


namespace bv { namespace model {

class Float3VertexAttributeChannel;

class VariableTopologyStripComponent: public ConnectedComponent
{
private:

    float           m_size;
    float           m_speed;
    float           m_oscilationSpeed;
    TimeType        m_segmentDeltaTime;
    TimeType        m_activeDuration;
    float           m_mainScale;

    int             numSegments;
    int             m_activeSegment;
    mutable bool    m_topologyChanged;

    float           m_startX;
    float           m_startY;
    float           m_posZ;

    Float3VertexAttributeChannel * m_positions;

private:

                        VariableTopologyStripComponent  ( float size, float speed, TimeType duration, int numSegments, float oscilationSpeed, float mainScale, float startX, float startY, float posZ );
                        ~VariableTopologyStripComponent ();

public:

    bool                IsActive                        ( TimeType t ) const;
    bool                TopologyChanged                 ( TimeType t ) const;

    virtual void        Update                          ( TimeType t );

    TimeType            ComponentDuration               () const;

private:

    glm::vec3           EvaluateFunction                ( TimeType t ) const;
    glm::vec3           EvaluateVelocity                ( TimeType t ) const;
    glm::vec3           EvaluateNormal                  ( const glm::vec3 & tangent ) const;

    glm::vec3           TopPosition                     ( const glm::vec3 & val, const glm::vec3 & normal ) const;
    glm::vec3           BottomPosition                  ( const glm::vec3 & val, const glm::vec3 & normal ) const;

public:

    static  VariableTopologyStripComponent *    Create  ( float size, float speed, TimeType duration, int numSegments, float oscilationSpeed, float mainScale, float startX, float startY, float posZ );

};

} //model
} //bv
