#pragma once

#include "Engine\Models\Plugins\Channels\GeometryChannelDescriptor.h"

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

    virtual bool                                    CanBeConnectedTo    ( IPlugin * plugin ) const;

protected:

    virtual bool                                    CanBeConnectedTo    ( const GeometryChannelDescriptor & desc ) const = 0;

};

class GeometryChannelStaticRect : public GeometryChannel
{

    GeometryChannelStaticRect(float w, float h);
public:
    static GeometryChannelStaticRect*             Create(float w = 1.f, float h = 1.f);
};

class GeometryChannelStaticRectTextured : public GeometryChannel
{

    GeometryChannelStaticRectTextured(const std::string& texturePath, float w, float h );
public:

    virtual bool                                          IsTimeInvariant     () const { return true; }

    static GeometryChannelStaticRectTextured*             Create(const std::string& texturePath, float w = 1.f, float h = 1.f );
};

} // model
} // bv
