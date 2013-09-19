#pragma once

namespace bv
{

class IChannel
{
public:
    virtual void        Update( float t ) = 0;

    virtual             ~IGeometryChannel();
};

} // bv