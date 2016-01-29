#pragma once

// FORWARD DECLARATIONS
namespace boost
{
class thread;
} // boost

namespace bv
{

class Thread
{
private:

    boost::thread * m_thread;

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
