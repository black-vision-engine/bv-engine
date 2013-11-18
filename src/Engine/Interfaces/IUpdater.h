#pragma once

#include "System/BasicTypes.h" //FIXME: store in PCH


namespace bv
{

class IUpdater
{
public:

    virtual         ~IUpdater   () {}

    virtual void    Update      ( TimeType t ) = 0;

};

} // bv
