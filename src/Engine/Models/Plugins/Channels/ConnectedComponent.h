#pragma once

#include "Engine\Models\Plugins\Interfaces\IConnectedComponent.h"

namespace bv { namespace model
{

class IVertexAttributeChannel;

class ConnectedComponent : public IConnectedComponent
{
    std::vector< IVertexAttributeChannel* >             m_VertexAttributeChannels;

public:

    virtual std::vector< IVertexAttributeChannel* >&    GetVertexAttributeChannels  () const;

    virtual int                                         GetNumVertices              () const;
    virtual int                                         GetNumPrimitives            () const;

    virtual                                             ~ConnectedComponent         (){};
    explicit                                            ConnectedComponent          ();
};

} // model
} // bv