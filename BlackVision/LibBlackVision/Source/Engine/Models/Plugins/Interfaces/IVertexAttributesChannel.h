#pragma once

#include <vector>

#include "Engine/Models/Plugins/Interfaces/IChannel.h"
#include "Engine/Models/Plugins/Interfaces/IConnectedComponent.h"

#include "Engine/Types/Enums.h"


namespace bv { namespace model
{

class IPlugin;
class IConnectedComponent;
class IVertexAttributesChannelDescriptor;

class IVertexAttributesChannel;
DEFINE_PTR_TYPE(IVertexAttributesChannel)
DEFINE_CONST_PTR_TYPE(IVertexAttributesChannel)

class IVertexAttributesChannel : public IChannel
{
public:

    virtual const IVertexAttributesChannelDescriptor *  GetDescriptor               ()                              const   = 0;
    virtual PrimitiveType                               GetPrimitiveType            ()                              const   = 0;

    virtual int                                         GetNumPrimitives            ( IConnectedComponentPtr cc )   const   = 0;
    virtual std::vector< IConnectedComponentPtr >       GetComponents               ()                              const   = 0;

    //virtual void                                        AddConnectedComponent       ( IConnectedComponent * cc )   = 0;

    virtual bool                                        IsTimeInvariant             ()                              const   = 0;

    //virtual bool                                        NeedsAttributesUpdate       ()                              const   = 0;
    //virtual bool                                        NeedsTopologyUpdate         ()                              const   = 0;

	virtual UInt64										GetLastAttributeUpdateID	()								const	= 0;
	virtual UInt64										GetLastTopologyUpdateID		()								const	= 0;


    virtual unsigned int                                TotalNumVertices            ()                              const   = 0;

    //FIXME: implement via plugin when its interface is known
    //virtual bool                                      CanBeConnectedTo            ( IPlugin * plugin )            const = 0;
    virtual bool                                        CanBeConnectedTo            ( IVertexAttributesChannelPtr channel )  const = 0;

    virtual                                             ~IVertexAttributesChannel   (){};
};


} // model
} // bv
