#pragma once

#include "Threading/CriticalSection.h"


namespace bv
{
 
class ScopedCriticalSection
{
private:

    CriticalSection &   m_criticalSection;

private:

    //Disable copying
    ScopedCriticalSection               ( const ScopedCriticalSection & src );
    ScopedCriticalSection               ( const ScopedCriticalSection && src );

    ScopedCriticalSection & operator =  ( const ScopedCriticalSection & src );

public:

    explicit    ScopedCriticalSection   ( CriticalSection &  criticalSection );
	            ~ScopedCriticalSection  ();

};
 
} //bv
