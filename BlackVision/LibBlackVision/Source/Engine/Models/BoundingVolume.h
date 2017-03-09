#pragma once

//#include "Mathematics/glm_inc.h"

#include "Mathematics/Box.h"

namespace bv { 

namespace model {

class VertexAttributesChannel;
class IVertexAttributesChannel;
DEFINE_CONST_PTR_TYPE( IVertexAttributesChannel );

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

    mathematics::Box                    m_childrenBox;
    //BoundingVolume *                    m_parent;

    UInt64                              m_lastTopologyID;
    UInt64                              m_lastAttribuetesID;

    const ParamTransform *              m_param;

public:
                                        BoundingVolume          ( const VertexAttributesChannel * vac, ParamTransform * param );

    void                                UpdateVAC               ( const IVertexAttributesChannel * vac );
    void                                UpdateParam             ( const ParamTransform * param );

    const mathematics::Box *            GetBoundingBox          () const;
    const mathematics::Box *            GetChildrenBox          () const;

    IConnectedComponentPtr              BuildBoxRepresentation  () const;
    IConnectedComponentPtr              BuildCenterRepresentation () const;

    void                                UpdateOwnBox            ( const IVertexAttributesChannelConstPtr & vac );
    void                                IncludeChildrenBox      ( const mathematics::Box & box );
};

} // model

} // bv
