#pragma once

#include "Engine\Models\Plugins\Channels/Geometry\GeometryChannelDescriptor.h"

#include "Engine/Models/Plugins/Interfaces/IGeometryChannel.h"
#include "Engine/Models/Plugins/Interfaces/IConnectedComponent.h"

#include <vector>

namespace bv { namespace model
{

class IPlugin;
class ConnectedComponent;

class GeometryChannel : public IGeometryChannel
{
protected:

    GeometryChannelDescriptor                       m_desc;

    std::vector< ConnectedComponent* >              m_connectedComponents;
    PrimitiveType                                   m_primitiveType;

public:

                                                    GeometryChannel     ( PrimitiveType type );
    virtual                                         ~GeometryChannel    ();

    //IChannel
    virtual void                                    Update              ( float t );

    //IGeometryChannel
    virtual const IGeometryChannelDescriptor *      GetDescriptor       () const;
    virtual PrimitiveType                           GetPrimitiveType    () const;

    virtual int                                     GetNumPrimitives    ( IConnectedComponent * cc ) const;
    virtual std::vector< IConnectedComponent * >    GetComponents       () const;

    //virtual bool                                  CanBeConnectedTo    ( IPlugin * plugin ) const;
    virtual bool                                    CanBeConnectedTo    ( IGeometryChannel * channel ) const;

protected:

    virtual bool                                    CanBeConnectedTo    ( const GeometryChannelDescriptor & desc ) const = 0;

};

} // model
} // bv
