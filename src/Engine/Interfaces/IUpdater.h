#pragma once

#include "System/BasicTypes.h" //FIXME: store in PCH


namespace bv
{

class IUpdater
{
public:

    virtual         ~IUpdater   () {}

    virtual void    DoUpdate    ( TimeType t ) = 0;

};

} // bv
