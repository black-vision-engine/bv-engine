#pragma once

#include "Engine/Models/Plugins/Channels/Geometry/GeometryChannel.h"

#include "Engine\Models\Plugins\Channels/Geometry\GeometryChannelDescriptor.h"

#include "Engine/Models/Plugins/Interfaces/IGeometryChannel.h"
#include "Engine/Models/Plugins/Interfaces/IConnectedComponent.h"

#include <vector>

namespace bv { namespace model
{

//FIXME: wha?
class GeometryChannelStaticRectTextured : public GeometryChannel
{
private:

                                                GeometryChannelStaticRectTextured   ( float w, float h );

public:

    virtual bool                                IsReadOnly                          () const { return false; }
    virtual bool                                IsTimeInvariant                     () const { return true; }

protected:

    virtual bool                                CanBeConnectedTo                    ( const GeometryChannelDescriptor & desc ) const;

public:

    static GeometryChannelStaticRectTextured *  Create                              ( float w = 1.f, float h = 1.f );

};

} // model
} // bv
