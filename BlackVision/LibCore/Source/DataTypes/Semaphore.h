#pragma once


#include <cassert>
#include <queue>
#include <condition_variable>

#include "CoreDEF.h"



namespace bv
{

class Semaphore
{
    std::mutex                  m_mtx;
    std::condition_variable     m_condVar;
    SizeType                    m_count;


    explicit        Semaphore( SizeType initCount )
        : m_count( initCount )
    {}

    void    Down    ();
    void    Up      ();

};


// ***********************
//
void        Semaphore::Down()
{
    std::unique_lock< std::mutex > lock( m_mtx );
    while( m_count > 0 )
        m_condVar.wait( lock );

    m_count--;
}

// ***********************
//
void        Semaphore::Up()
{
    std::unique_lock< std::mutex > lock( m_mtx );
    m_count++;
}



}   // bv
