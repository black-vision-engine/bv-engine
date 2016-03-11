#pragma once

#include "IObservable.h"

#include <set>

namespace bv
{

class Observable : public IObservable
{
private:
    std::set< IObserverPtr >    m_observers;

protected:

    virtual void        NotifyObservers     (  const std::string & arg  ) const override;
    virtual void        AddObserver         ( const IObserverPtr & observer ) override;
    virtual void        RemoveObserver      ( const IObserverPtr & observer ) override;

public:

    virtual             ~Observable         ();
};

DEFINE_CONST_PTR_TYPE( Observable )
DEFINE_PTR_TYPE( Observable )

} // bv