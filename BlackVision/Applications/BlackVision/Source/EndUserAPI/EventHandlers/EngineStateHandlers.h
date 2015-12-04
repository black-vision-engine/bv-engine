#pragma once

#include "Engine/Events/Events.h"

namespace bv
{
class BVAppLogic;


class EngineStateHandlers
{
private:
    BVAppLogic*             m_appLogic;
public:
    EngineStateHandlers( BVAppLogic* logic );
    ~EngineStateHandlers();

    void    RenderingModeEvent       ( IEventPtr evt );
};

} //bv
