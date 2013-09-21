#pragma once

namespace bv { namespace model
{

class IChannel
{
public:
    virtual void        Update( float t ) = 0;

    virtual             ~IChannel();
};

} // model
} // bv