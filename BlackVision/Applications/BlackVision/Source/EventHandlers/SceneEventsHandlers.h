#pragma once

#include "Engine/Events/Events.h"

namespace bv
{

class SceneEventsHandlers
{
public:
    SceneEventsHandlers();
    ~SceneEventsHandlers();

    //void SceneStructure      ( bv::IEventPtr evt );
    // SetNodeVisible <- wczeœniej implementowane przez SetParamEvent

    void        AddNode         ();
    void        RemoveNode      ();
};


} //bv
