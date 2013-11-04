#pragma once

#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"
#include <glm/glm.hpp>

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
    float   m_mainScale;

    int     numSegments;
    int     m_activeSegment;
    bool    m_topologyChanged;

    float   m_startX;
    float   m_startY;
    float   m_posZ;

    Float3VertexAttributeChannel * m_positions;

private:

                        VariableTopologyStripComponent  ( float size, float speed, float duration, int numSegments, float oscilationSpeed, float mainScale, float startX, float startY, float posZ );
                        ~VariableTopologyStripComponent ();

public:

    bool                IsActive                        ( float t ) const;
    bool                TopologyChanged                 ( float t ) const;

    virtual void        Update                          ( float t );

    float               ComponentDuration               () const;

private:

    glm::vec3           EvaluateFunction                ( float t ) const;
    glm::vec3           EvaluateVelocity                ( float t ) const;
    glm::vec3           EvaluateNormal                  ( const glm::vec3 & tangent ) const;

    glm::vec3           TopPosition                     ( const glm::vec3 & val, const glm::vec3 & normal ) const;
    glm::vec3           BottomPosition                  ( const glm::vec3 & val, const glm::vec3 & normal ) const;

public:

    static  VariableTopologyStripComponent *    Create  ( float size, float speed, float duration, int numSegments, float oscilationSpeed, float mainScale, float startX, float startY, float posZ );

};

} //model
} //bv
