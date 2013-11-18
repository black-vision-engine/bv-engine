#pragma once

#include "Engine/Models/Plugins/Interfaces/IConnectedComponent.h"


namespace bv { namespace model
{

class IVertexAttributeChannel;
class VertexAttributeChannel;

class ConnectedComponent : public IConnectedComponent
{
public:

    std::vector< VertexAttributeChannel * >             m_vertexAttributeChannels;

public:

    virtual void                                        Update( TimeType t );

    virtual std::vector< IVertexAttributeChannel* >     GetVertexAttributeChannels  () const;

    virtual unsigned int                                GetNumVertices              () const;
    virtual unsigned int                                GetNumPrimitives            () const;

    virtual                                             ~ConnectedComponent         (){};
    explicit                                            ConnectedComponent          ();
};

} // model
} // bv