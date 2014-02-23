#pragma once

#include "System/BasicTypes.h" //FIXME: store in PCH


namespace bv
{

class IUpdatable
{
public:

    virtual void    Update          ( TimeType t ) = 0;

    virtual         ~IUpdatable     () {}

};


} // bv
