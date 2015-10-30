#pragma once

#include "Engine/Events/Events.h"

namespace bv
{
class BVAppLogic;

class SceneEventsHandlers
{
public:
    SceneEventsHandlers( BVAppLogic* appLogic );
    ~SceneEventsHandlers();

    //void SceneStructure      ( bv::IEventPtr evt );
    // SetNodeVisible <- wcze�niej implementowane przez SetParamEvent

    void        AddNode         ();
    void        RemoveNode      ();
};


} //bv
