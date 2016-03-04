#pragma once

//#include "Mathematics/glm_inc.h"

#include "Mathematics/Box.h"

namespace bv { 

namespace model {

class VertexAttributesChannel;

class IConnectedComponent;
DEFINE_PTR_TYPE( IConnectedComponent );

class BoundingVolume {
    mathematics::Box            m_box;
    //glm::mat4               m_transform; // apply unit cube to get bounding box :)

    VertexAttributesChannel *   m_vac;
    UInt64                      m_lastTopologyID;
    UInt64                      m_lastAttribuetesID;

public:
                                BoundingVolume          ( VertexAttributesChannel * vac );

    //void                        SetVAC                  ( VertexAttributesChannel * vac );

    const mathematics::Box *    GetBoundingBox          () const;

    IConnectedComponentPtr      BuildConnectedComponent () const;

    void                        Update                  ();
};

} // model

} // bv
