#pragma once

#include <memory>

#include "Engine/Models/Plugins/Interfaces/IConnectedComponent.h"
#include "Engine/Interfaces/IUpdatable.h"


namespace bv { namespace model
{

class IAttributeChannel;
class AttributeChannel;

DEFINE_PTR_TYPE(AttributeChannel)

class ConnectedComponent : public IConnectedComponent, public IUpdatable
{
private:

    std::vector< AttributeChannelPtr >          m_attributeChannels;

public:

    explicit                                    ConnectedComponent          ();
    virtual                                     ~ConnectedComponent         ();

    virtual void                                Update                      ( TimeType t );

    virtual std::vector< IAttributeChannel* >   GetAttributeChannels        () const;

    virtual unsigned int                        GetNumVertices              () const;
    virtual unsigned int                        GetNumPrimitives            () const;

    const std::vector< AttributeChannelPtr > &  GetAttributeChannelsPtr     () const;

    void                                        AddAttributeChannel         ( const AttributeChannelPtr& attr );
};

} // model
} // bv
