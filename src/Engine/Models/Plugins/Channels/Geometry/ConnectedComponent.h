#pragma once

#include "Engine/Models/Plugins/Interfaces/IConnectedComponent.h"
#include "Engine/Interfaces/IUpdatable.h"


namespace bv { namespace model
{

class IAttributeChannel;
class AttributeChannel;

class ConnectedComponent : public IConnectedComponent, public IUpdatable
{
public:

    std::vector< AttributeChannel * >           m_attributeChannels;

public:

    explicit                                    ConnectedComponent          ();
    virtual                                     ~ConnectedComponent         ();

    virtual void                                Update                      ( TimeType t );

    virtual std::vector< IAttributeChannel* >   GetAttributeChannels  () const;

    virtual unsigned int                        GetNumVertices              () const;
    virtual unsigned int                        GetNumPrimitives            () const;

};

} // model
} // bv
