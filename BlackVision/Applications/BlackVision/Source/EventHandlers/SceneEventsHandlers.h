#pragma once

#include "Engine/Events/Events.h"

namespace bv
{
class BVAppLogic;

class SceneEventsHandlers
{
private:
    BVAppLogic*         m_appLogic;
public:
    SceneEventsHandlers( BVAppLogic* appLogic );
    ~SceneEventsHandlers();

    void        SceneStructure      ( bv::IEventPtr evt );
    void        ProjectStructure    ( bv::IEventPtr evt );
};


} //bv
