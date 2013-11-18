#pragma once

#include <vector>

#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannelDescriptor.h"

#include "Engine/Models/Plugins/Interfaces/IVertexAttributesChannel.h"
#include "Engine/Models/Plugins/Interfaces/IConnectedComponent.h"


namespace bv { namespace model
{

class IPlugin;
class ConnectedComponent;

class VertexAttributesChannel : public IVertexAttributesChannel
{
protected:

    VertexAttributesChannelDescriptor                       m_desc;

    std::vector< ConnectedComponent* >              m_connectedComponents;
    PrimitiveType                                   m_primitiveType;

    bool                                            m_isReadOnly;
    bool                                            m_isTimeInvariant;
    bool                                            m_needsAttributesUpdate;
    bool                                            m_needsTopologyUpdate;

public:
                                                        VertexAttributesChannel         ( PrimitiveType type, bool isReadOnly = false, bool isTimeInvariant = false );
                                                        VertexAttributesChannel         ( PrimitiveType type, const VertexAttributesChannelDescriptor& desc, bool isReadOnly = false, bool isTimeInvariant = false );
    virtual                                             ~VertexAttributesChannel        ();

    //IChannel
    virtual void                                        Update                  ( TimeType t );
    virtual bool                                        IsReadOnly              () const;

    //IVertexAttributesChannel
    virtual bool                                        IsTimeInvariant         ()          const;
    virtual bool                                        NeedsAttributesUpdate   ( TimeType t ) const;
    virtual bool                                        NeedsTopologyUpdate     ( TimeType t ) const;

    void                                                SetNeedsAttributesUpdate( bool b );
    void                                                SetNeedsTopologyUpdate  ( bool b );
    virtual unsigned int                                TotalNumVertices        ()          const;

    virtual const IVertexAttributesChannelDescriptor *  GetDescriptor           () const;
    void                                                SetDescriptor           ( const VertexAttributesChannelDescriptor & desc );

    virtual PrimitiveType                               GetPrimitiveType        () const;

    virtual void                                        AddConnectedComponent   ( IConnectedComponent * cc );

    virtual int                                         GetNumPrimitives        ( IConnectedComponent * cc ) const;
    virtual std::vector< IConnectedComponent * >        GetComponents           () const;

    //virtual bool                                      CanBeConnectedTo        ( IPlugin * plugin ) const;
    virtual bool                                        CanBeConnectedTo        ( IVertexAttributesChannel * channel ) const;

protected:

    virtual bool                                    CanBeConnectedTo    ( const VertexAttributesChannelDescriptor & desc ) const { return true; }

};

} // model
} // bv
