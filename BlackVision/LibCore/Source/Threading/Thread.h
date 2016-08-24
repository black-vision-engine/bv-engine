#pragma once


// FORWARD DECLARATIONS
namespace boost
{
    class thread;
} // boost

namespace bv {

class Thread
{
public:

enum class ThreadPriotity : int 
{
    IDLE = -15,
    LOWEST = -2,
    BELOW_NORMAL = -1,
    NORMAL = 0,
    ABOVE_NORMAL = 1,
    HIGHEST = 2,
    TIME_CRITICAL = 15
};

private:

    ThreadPriotity  m_priority;

    boost::thread * m_thread;

private:

    static void     RunnerFunc  ( Thread * obj );

public:

                    Thread      ();
    virtual         ~Thread     ();

    void            Start       ( ThreadPriotity priority = ThreadPriotity::NORMAL );
    void            Join        ();

protected:

    virtual void    Run         () = 0;

};

} //bv
