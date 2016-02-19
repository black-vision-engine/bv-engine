#pragma once

#include <mutex>          // std::mutex, std::unique_lock, std::defer_lock


namespace bv
{
 
class CriticalSection
{
    std::mutex                      m_mtx;

protected:
    std::unique_lock<std::mutex>    m_criticalSection;

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
