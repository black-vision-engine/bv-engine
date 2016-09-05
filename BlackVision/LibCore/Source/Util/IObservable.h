#pragma once

#include "IObserver.h"

namespace bv
{

class IObservable
{
protected:

    virtual void        NotifyObservers     ( const std::string & arg ) const = 0;

public:

    virtual void        AddObserver         ( IObserver * observer ) = 0;
    virtual void        RemoveObserver      ( IObserver * observer ) = 0;


    virtual             ~IObservable        () {};
};

DEFINE_CONST_PTR_TYPE( IObservable )
DEFINE_PTR_TYPE( IObservable )

} // bv