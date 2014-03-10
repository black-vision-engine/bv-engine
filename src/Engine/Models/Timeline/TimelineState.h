#pragma once

#include "Engine/Models/Interfaces/ITimeEvaluator.h"


namespace bv { namespace model {

struct TimelineState
{
    bool    m_started;
    bool    m_stopped;
    bool    m_paused;

    void    Reset   ();

};

} //model
} //bv
