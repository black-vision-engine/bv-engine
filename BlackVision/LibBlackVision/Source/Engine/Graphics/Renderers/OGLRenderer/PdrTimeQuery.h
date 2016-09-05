#pragma once

#include "Engine/Graphics/Renderers/OGLRenderer/PdrConstants.h"


namespace bv
{

#define NUM_QUERY_TIMER_BACK_BUFFERS 2
#define NUM_QUERY_TIMERS 2

class PdrTimeQuery
{
private:
    GLuint      m_timer[ NUM_QUERY_TIMER_BACK_BUFFERS ][ NUM_QUERY_TIMERS ];

    char        m_buffer;       ///< Back buffer or front buffer.

public:
    explicit    PdrTimeQuery    ();
                ~PdrTimeQuery   ();

    void        BeginQuery  ();
    void        EndQuery    ();

    UInt64      QueryResult ();

private:

    void        SwapBuffers ();
};


}   // bv

