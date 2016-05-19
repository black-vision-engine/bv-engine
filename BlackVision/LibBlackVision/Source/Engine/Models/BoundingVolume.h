#pragma once

//#include "Mathematics/glm_inc.h"

#include "Mathematics/Box.h"

namespace bv { 

namespace model {

class VertexAttributesChannel;
class IVertexAttributesChannel;

class IConnectedComponent;
DEFINE_PTR_TYPE( IConnectedComponent );

class ParamTransform;

class BoundingVolume;
DEFINE_PTR_TYPE(BoundingVolume)
DEFINE_CONST_PTR_TYPE(BoundingVolume)


class BoundingVolume {
    mathematics::Box                    m_box;
    glm::vec3                           m_center;
    //glm::mat4               m_transform; // apply unit cube to get bounding box :)

    mutable VertexAttributesChannel *   m_vac;
    UInt64                              m_lastTopologyID;
    UInt64                              m_lastAttribuetesID;

    mutable const ParamTransform *      m_param;

public:
                                        BoundingVolume          ( VertexAttributesChannel * vac, ParamTransform * param );

    void                                UpdateVAC               ( const IVertexAttributesChannel * vac ) const;
    void                                UpdateParam             ( const ParamTransform * param ) const;

    const mathematics::Box *            GetBoundingBox          () const;

    IConnectedComponentPtr              BuildBoxRepresentation  () const;
    IConnectedComponentPtr              BuildCenterRepresentation () const;

    void                                Update                  ();

};

} // model

} // bv
