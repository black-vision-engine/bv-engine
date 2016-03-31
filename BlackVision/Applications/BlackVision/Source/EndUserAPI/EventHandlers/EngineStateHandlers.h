#pragma once

#include "Engine/Events/Events.h"

namespace bv
{
class BVAppLogic;


class EngineStateHandlers
{
private:

    BVAppLogic*             m_appLogic;

    bool                    m_enableLockQueue;
    //UInt32                  m_lockWarning;    // Warn if queue is locked for more than m_lockWarning frames.

public:
    EngineStateHandlers( BVAppLogic* logic );
    ~EngineStateHandlers();

    void    EngineStateHandler       ( IEventPtr evt );
    void    MouseInteraction         ( IEventPtr evt );
};

} //bv
