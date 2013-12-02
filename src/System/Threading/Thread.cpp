#include "Thread.h"

namespace bv
{

// *******************************
//
void     Thread::RunnerFunc  ( Thread * obj )
{
    obj->Run();
}

// *******************************
//
Thread::Thread  ()
{
}

// *******************************
//
Thread::~Thread ()
{
}

// *******************************
//
void Thread::Start   ()
{
    m_thread = boost::thread( &Thread::RunnerFunc, this );
}

// *******************************
//
void Thread::Join    ()
{
    m_thread.join();
}

} //bv
