#pragma once

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
    std::vector< ConnectedComponent* >              m_components;
    std::vector< AttrType >                         m_type;
    std::vector< AttrSemantic >                     m_semantic;
    PrimitiveType                                   m_primitiveType;

public:

    void                                            Update              (float t);

    virtual const std::vector< AttrType >&          GetType             () const;
    virtual const std::vector< AttrSemantic >&      GetSemantic         () const;
    virtual PrimitiveType                           GetPrimitiveType    () const;

    virtual int                                     GetNumPrimitives    ( IConnectedComponent* connComp ) const;
    virtual std::vector< IConnectedComponent* >     GetComponents       () const;

    virtual                                         ~GeometryChannel    ();

                                                    GeometryChannel     (const IPlugin* pl);
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
