#pragma once

#include "Engine/Models/Plugins/Interfaces/IGeometryChannel.h"
#include "Engine/Models/Plugins/Interfaces/IConnectedComponent.h"

#include <vector>

namespace bv { namespace model
{

class GeometryChannel : public IGeometryChannel
{
    std::vector< IConnectedComponent* >             m_components;

public:
    virtual std::vector< AttrType >                 GetType             () const;
    virtual std::vector< AttrSemantic >             GetSemantic         () const;
    virtual PrimitiveType                           GetPrimitiveType    () const;

    virtual std::vector< IConnectedComponent* >&    GetComponents       () const;

    virtual bool                                    IsTimeInvariant     () const;

    virtual                                         ~GeometryChannel   (){};
};

} // model
} // bv
