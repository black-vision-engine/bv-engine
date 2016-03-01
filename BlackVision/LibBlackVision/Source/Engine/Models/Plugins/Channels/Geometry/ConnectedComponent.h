#pragma once

#include <memory>

#include "Engine/Models/Plugins/Interfaces/IConnectedComponent.h"
#include "Engine/Interfaces/IUpdatable.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannel.h"

namespace bv { namespace model
{

class ConnectedComponent;
DEFINE_PTR_TYPE(ConnectedComponent)
DEFINE_CONST_PTR_TYPE(ConnectedComponent)

class ConnectedComponent : public IConnectedComponent, public IUpdatable
{
private:

    std::vector< AttributeChannelPtr >          m_attributeChannels;

protected:
    explicit                                    ConnectedComponent          ();

public:
    virtual                                     ~ConnectedComponent         ();

    virtual void                                Update                      ( TimeType t );

    virtual std::vector< IAttributeChannelPtr > GetAttributeChannels        () const;

    virtual unsigned int                        GetNumVertices              () const;
    virtual unsigned int                        GetNumPrimitives            () const;

    const std::vector< AttributeChannelPtr > &  GetAttributeChannelsPtr     () const;

    void                                        AddAttributeChannel         ( const AttributeChannelPtr& attr );

    static ConnectedComponentPtr                Create                      ();

    //@param[ occuranceIdx ] for occuranceIdx < 0 function will return the last attrChannel
    AttributeChannelPtr							GetAttrChannel				( AttributeSemantic semantic, Int32 occuranceIdx = 0 ) const;
};

} // model
} // bv
