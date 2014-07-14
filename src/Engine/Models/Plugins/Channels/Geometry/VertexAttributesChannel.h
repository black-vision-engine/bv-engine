#pragma once

#include <vector>
#include <memory>

#include "Engine/Models/Plugins/Channels/Geometry/VertexAttributesChannelDescriptor.h"
#include "Engine/Models/Plugins/Channels/Geometry/ConnectedComponent.h"

#include "Engine/Models/Plugins/Interfaces/IVertexAttributesChannel.h"
#include "Engine/Models/Plugins/Interfaces/IConnectedComponent.h"


namespace bv { namespace model
{

class IPlugin;
class ConnectedComponent;

class VertexAttributesChannel : public IVertexAttributesChannel
{
protected:

    VertexAttributesChannelDescriptor               m_desc;

    std::vector< ConnectedComponentPtr >            m_connectedComponents;
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
    virtual void                                        Update                  ( TimeType t ) override;
    virtual bool                                        IsReadOnly              () const override;

    //IVertexAttributesChannel
    virtual bool                                        IsTimeInvariant         () const override;
    virtual bool                                        NeedsAttributesUpdate   () const override;
    virtual bool                                        NeedsTopologyUpdate     () const override;

    void                                                SetNeedsAttributesUpdate( bool b );
    void                                                SetNeedsTopologyUpdate  ( bool b );

    virtual unsigned int                                TotalNumVertices        () const override;

    virtual const IVertexAttributesChannelDescriptor *  GetDescriptor           () const override;
    void                                                SetDescriptor           ( const VertexAttributesChannelDescriptor & desc );

    virtual PrimitiveType                               GetPrimitiveType        () const override;

    void                                                AddConnectedComponent   ( ConnectedComponentPtr cc );
    void                                                ClearConnectedComponent ( );

    virtual int                                         GetNumPrimitives        ( IConnectedComponentPtr cc ) const override;
    virtual std::vector< IConnectedComponentPtr >       GetComponents           () const override;

    //virtual bool                                      CanBeConnectedTo        ( IPlugin * plugin ) const;
    virtual bool                                        CanBeConnectedTo        ( IVertexAttributesChannelPtr channel ) const override;

    ConnectedComponentPtr                               GetConnectedComponent   ( unsigned int idx );

protected:

    virtual bool                                        CanBeConnectedTo        ( const VertexAttributesChannelDescriptor & desc ) const
    { 
        return true;
    }
};

DEFINE_PTR_TYPE(VertexAttributesChannel)

} // model
} // bv
