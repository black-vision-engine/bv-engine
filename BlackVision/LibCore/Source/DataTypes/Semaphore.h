#pragma once

#include <cassert>
#include <queue>
#include <condition_variable>

#include "CoreDEF.h"


namespace bv {

class Semaphore
{
private:

    std::mutex                  m_mtx;
    std::condition_variable     m_condVar;
    SizeType                    m_count;

public:

    explicit        Semaphore( SizeType initCount )
        : m_count( initCount )
    {}

    void    Down    ();
    bool    TryDown ();
    void    Up      ();

};


// ***********************
//
inline void        Semaphore::Down()
{
    std::unique_lock< std::mutex > lock( m_mtx );
    while( m_count <= 0 )
        m_condVar.wait( lock );

    m_count--;
}

// ***********************
//
inline bool        Semaphore::TryDown()
{
    std::unique_lock< std::mutex > lock( m_mtx );
        
    if( m_count > 0 )
    {
        m_count--;
        return true;
    }
    
    return false;
}

// ***********************
//
inline void        Semaphore::Up()
{
    std::unique_lock< std::mutex > lock( m_mtx );
    m_count++;

    m_condVar.notify_one();
}

} // bv
