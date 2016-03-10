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

    void        NodeStructure       ( bv::IEventPtr evt );
    void        PluginStructure     ( bv::IEventPtr evt );
    void        ProjectStructure    ( bv::IEventPtr evt );
    void        SceneStructure	    ( bv::IEventPtr evt );

    void        SceneVariable       ( bv::IEventPtr evt );

private:
    void        ThumbnailRendered   ( bv::IEventPtr evt );
};


} //bv
