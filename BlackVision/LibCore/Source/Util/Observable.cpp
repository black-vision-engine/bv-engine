#include "Observable.h"

namespace bv
{

//**************************************
//
void        Observable::NotifyObservers     (  const std::string & arg  ) const
{
    for( auto obs : m_observers )
    {
        obs->HandleEvent( this, arg );
    }
}

//**************************************
//
void        Observable::AddObserver         ( IObserver * observer )
{
    m_observers.insert( observer );
}

//**************************************
//
void        Observable::RemoveObserver      ( IObserver * observer )
{
    m_observers.erase( observer );
}

//**************************************
//
Observable::~Observable         ()
{}

} // bc

