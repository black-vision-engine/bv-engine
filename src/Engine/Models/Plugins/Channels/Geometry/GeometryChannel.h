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
    bool                                            m_isReadOnly;
    bool                                            m_isTimeInvariant;
    bool                                            m_needsPositionUpdate;
    bool                                            m_needsTopologyUpdate;

public:
                                                    GeometryChannel     ( PrimitiveType type, bool isReadOnly = false, bool isTimeInvariant = false );
                                                    GeometryChannel     ( PrimitiveType type, const GeometryChannelDescriptor& desc, bool isReadOnly = false, bool isTimeInvariant = false );
    virtual                                         ~GeometryChannel    ();

    //IChannel
    virtual void                                    Update              ( float t );
    virtual bool                                    IsReadOnly          () const;

    //IGeometryChannel
    virtual bool                                    IsTimeInvariant     ()          const;
    virtual bool                                    NeedsAttributesUpdate( float t ) const;
    virtual bool                                    NeedsTopologyUpdate ( float t ) const;

    void                                            SetNeedsAttributesUpdate( bool b );
    void                                            SetNeedsTopologyUpdate  ( bool b );
    virtual unsigned int                            TotalNumVertices    ()          const;

    virtual const IGeometryChannelDescriptor *      GetDescriptor       () const;
    void                                            SetDescriptor       ( const GeometryChannelDescriptor & desc );

    virtual PrimitiveType                           GetPrimitiveType    () const;

    virtual void                                    AddConnectedComponent( IConnectedComponent * cc );

    virtual int                                     GetNumPrimitives    ( IConnectedComponent * cc ) const;
    virtual std::vector< IConnectedComponent * >    GetComponents       () const;

    //virtual bool                                  CanBeConnectedTo    ( IPlugin * plugin ) const;
    virtual bool                                    CanBeConnectedTo    ( IGeometryChannel * channel ) const;

protected:

    virtual bool                                    CanBeConnectedTo    ( const GeometryChannelDescriptor & desc ) const { return true; }
};

} // model
} // bv
