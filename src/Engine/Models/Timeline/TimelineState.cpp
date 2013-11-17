#include "TimelineState.h"

namespace bv { namespace model {

void    TimelineState::Reset   ()
{
    m_paused    = false;
    m_started   = false;
    m_stopped   = false;
}


} //model
} //bv
