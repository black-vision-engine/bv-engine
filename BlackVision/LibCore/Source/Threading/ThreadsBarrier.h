#pragma once
/**
@file ThreadsBarrier.h
@author nieznanysprawiciel
Implementation based on https://github.com/nieznanysprawiciel/swCommonLib/blob/master/Common/Multithreading/ThreadsBarrier.h
*/

#include "CoreDEF.h"


#include <mutex>
#include <condition_variable>


namespace bv
{



/**@brief Can block multiple threads and wait for signal.
ThreadsBarrier is reusable.

Implementation based on http://stackoverflow.com/questions/24465533/implementing-boostbarrier-in-c11

@ingroup Helpers*/
class ThreadsBarrier
{
private:

    std::mutex                  m_lock;
    std::condition_variable     m_condVariable;
    SizeType                    m_generation;
    const UInt32                m_numThreads;
    UInt32                      m_remain;

protected:
public:
    explicit        ThreadsBarrier      ( UInt16 numThreads );
                    ~ThreadsBarrier     () = default;

    void            ArriveAndWait       ();
};


}    // sw

