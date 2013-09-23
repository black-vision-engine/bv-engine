#pragma once

#include "Engine\Models\Plugins\Channels\GeometryChannel.h"

#include "Engine\Models\Plugins\Channels\GeometryChannelDescriptor.h"

#include "Engine/Models/Plugins/Interfaces/IGeometryChannel.h"
#include "Engine/Models/Plugins/Interfaces/IConnectedComponent.h"

#include <vector>

namespace bv { namespace model
{

//FIXME: wha?
class GeometryChannelStaticRect : public GeometryChannel
{
private:

                                        GeometryChannelStaticRect   ( float w, float h );

public:

    virtual bool                        IsReadOnly                  () const { return false; }
    virtual bool                        IsTimeInvariant             () const { return true; }

protected:

    virtual bool                        CanBeConnectedTo            ( const GeometryChannelDescriptor & desc ) const;

public:

    static GeometryChannelStaticRect *  Create                      ( float w = 1.f, float h = 1.f );

};

//FIXME: wha?
class GeometryChannelStaticRectTextured : public GeometryChannel
{
private:

                                                GeometryChannelStaticRectTextured   ( const std::string& texturePath, float w, float h );

public:

    virtual bool                                IsReadOnly                          () const { return false; }
    virtual bool                                IsTimeInvariant                     () const { return true; }

protected:

    virtual bool                                CanBeConnectedTo                    ( const GeometryChannelDescriptor & desc ) const;

public:

    static GeometryChannelStaticRectTextured *  Create                              ( const std::string& texturePath, float w = 1.f, float h = 1.f );

};

} // model
} // bv
