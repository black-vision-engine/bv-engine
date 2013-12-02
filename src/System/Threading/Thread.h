#pragma once

#include <boost/thread.hpp>
#include <boost/date_time.hpp>


namespace bv
{

class Thread
{
private:

    boost::thread   m_thread;

private:

    static void     RunnerFunc  ( Thread * obj );

public:

                    Thread      ();
            virtual ~Thread     ();

    void            Start       ();
    void            Join        ();

protected:

    virtual void    Run         () = 0;

};

} //bv
