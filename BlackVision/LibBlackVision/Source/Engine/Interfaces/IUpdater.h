#pragma once

#include "CoreDEF.h" //FIXME: store in PCH


namespace bv
{

class IUpdater
{
public:

    virtual         ~IUpdater   () {}

    virtual void    DoUpdate    () = 0;
    virtual void    DoUpdateTM  () = 0;

};

DEFINE_PTR_TYPE(IUpdater)
DEFINE_CONST_PTR_TYPE(IUpdater)

} // bv
