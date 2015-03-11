#pragma once

#include "CoreDEF.h" //FIXME: store in PCH


namespace bv
{

class IUpdatable
{
public:

    virtual void    Update          ( TimeType t ) = 0;

    virtual         ~IUpdatable     () {}

};


} // bv
