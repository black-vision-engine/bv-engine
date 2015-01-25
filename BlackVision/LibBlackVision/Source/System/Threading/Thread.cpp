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
    Join();
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

namespace bv
{

class ThTestSleep : public Thread
{
public:

    //@see: http://antonym.org/2009/05/threading-with-boost---part-i-creating-threads.html
    virtual void Run()
    {
        boost::posix_time::seconds workTime( 3 );
        printf( "LTH: Thread running\n" );
        boost::this_thread::sleep( workTime );
        printf( "LTH: Thread joined\n" );
    }

};

namespace
{

void tth()
{
    bv::ThTestSleep ts;

    printf( "MTH: Starting Sleep Thread\n" );
    ts.Start();
    printf( "MTH: Sleep Thread Started\n" );
    ts.Join();
    printf( "MTH: Sleep Thread Finished\n" );
}

} //anonymous

} //bv
