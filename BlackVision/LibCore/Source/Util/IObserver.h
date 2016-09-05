#pragma once

#include "CoreDEF.h"

namespace bv
{

class IObservable;

class IObserver
{
public:

    virtual void        HandleEvent     ( const IObservable * observable, const std::string & arg ) = 0;

    virtual             ~IObserver      () {};
};

} // bv