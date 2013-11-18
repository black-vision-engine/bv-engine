#pragma once

#include "Engine/Models/Plugins/Interfaces/IConnectedComponent.h"
#include "Engine/Interfaces/IUpdatable.h"


namespace bv { namespace model
{

class IVertexAttributeChannel;
class VertexAttributeChannel;

class ConnectedComponent : public IConnectedComponent, public IUpdatable
{
public:

    std::vector< VertexAttributeChannel * >             m_vertexAttributeChannels;

public:

    explicit                                            ConnectedComponent          ();
    virtual                                             ~ConnectedComponent         ();

    virtual void                                        Update                      ( TimeType t );

    virtual std::vector< IVertexAttributeChannel* >     GetVertexAttributeChannels  () const;

    virtual unsigned int                                GetNumVertices              () const;
    virtual unsigned int                                GetNumPrimitives            () const;

};

} // model
} // bv
