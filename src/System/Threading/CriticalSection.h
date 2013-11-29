#pragma once

#include <windows.h>


namespace bv
{
 
class CriticalSection
{
protected:

    mutable CRITICAL_SECTION    m_criticalSection;

private:

    //Cannot be copied
    CriticalSection                 ( const CriticalSection & src );
    CriticalSection                 ( const CriticalSection && src );

    CriticalSection & operator =    ( const CriticalSection & src );

public:

    explicit    CriticalSection     ();
                ~CriticalSection    ();
 
 
    void        Lock                ();
    void        Unlock              ();

};
 
} //bv
