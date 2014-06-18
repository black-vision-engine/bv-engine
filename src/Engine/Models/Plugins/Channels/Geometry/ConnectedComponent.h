#pragma once

#include <memory>

#include "Engine/Models/Plugins/Interfaces/IConnectedComponent.h"
#include "Engine/Interfaces/IUpdatable.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannel.h"

namespace bv { namespace model
{

class ConnectedComponent : public IConnectedComponent, public IUpdatable
{
private:

    std::vector< AttributeChannelPtr >          m_attributeChannels;

public:

    explicit                                    ConnectedComponent          ();
    virtual                                     ~ConnectedComponent         ();

    virtual void                                Update                      ( TimeType t );

    virtual std::vector< IAttributeChannelPtr > GetAttributeChannels        () const;

    virtual unsigned int                        GetNumVertices              () const;
    virtual unsigned int                        GetNumPrimitives            () const;

    const std::vector< AttributeChannelPtr > &  GetAttributeChannelsPtr     () const;

    void                                        AddAttributeChannel         ( const AttributeChannelPtr& attr );
};

DEFINE_PTR_TYPE(ConnectedComponent)
DEFINE_CONST_PTR_TYPE(ConnectedComponent)

} // model
} // bv
