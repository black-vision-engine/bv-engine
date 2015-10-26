#pragma once

#include "Engine/Events/Events.h"

namespace bv
{

class SceneEventsHandlers
{
public:
    SceneEventsHandlers();
    ~SceneEventsHandlers();

    static void SceneStructure      ( bv::IEventPtr evt );
    // SetNodeVisible <- wczeœniej implementowane przez SetParamEvent
};


} //bv
